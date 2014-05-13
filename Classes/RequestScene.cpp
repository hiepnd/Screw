/****************************************************************************
 Copyright (c) hiepndhut@gmail.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "RequestScene.h"
#include "Notification.h"
#include "Home.h"

static const Size __table_size__ = Size(640, 600);
static const Size __cell_size__ = Size(640, 100);
static const float __gap = 5.0;
static const float __avatar_width = 64.0;
static const float __action_width = 100.0;

#pragma mark Request Table Cell
RequestCell::RequestCell() {
    // [gap][Avatar][gap][Message][Action]
    _message = Label::createWithTTF(" ", "fonts/Marker Felt.ttf", 30,
                                    Size(__cell_size__.width - __gap * 2 - __action_width - __avatar_width, __cell_size__.height),
                                    TextHAlignment::LEFT,
                                    TextVAlignment::CENTER
                                    );
    _action = Label::createWithTTF(" ", "fonts/Marker Felt.ttf", 30,
                                    Size(__action_width, __cell_size__.height),
                                    TextHAlignment::CENTER,
                                    TextVAlignment::CENTER
                                    );
    
    _avatar = Sprite::create();
    _avatar->setPosition(__gap + __avatar_width/2, __cell_size__.height/2);
    this->addChild(_avatar);
    
    _message->setAnchorPoint(Point(1.0, 0.5));
    _message->setPosition((__cell_size__.width - __action_width), __cell_size__.height/2);
    this->addChild(_message);
    
    _action->setAnchorPoint(Point(1.0, 0.5));
    _action->setPosition(__cell_size__.width, __cell_size__.height/2);
    this->addChild(_action);
}

void RequestCell::update(GraphRequest *request) {
    int type = request->getType();
    switch (type) {
        case RequestTypeAskforFish:
            _message->setString(request->getFrom()->getName() + " need a fish");
            _action->setString("Send");
            break;
        case RequestTypeSendFish:
            _message->setString(request->getFrom()->getName() + " gave you a fish");
            _action->setString("Claim");
            break;
        default:
            
            break;
    }
    _request = request;
    this->updateAvatar();
}

void RequestCell::updateAvatar() {
    Texture2D *texture = PhotoLoader::getInstance()->loadTexture(_request->getFrom()->getId());
    _avatar->setTexture(texture);
    _avatar->setTextureRect(Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
    _avatar->setScale(__avatar_width/texture->getContentSize().width);
}

GraphRequest *RequestCell::getRequest() {
    return _request;
}

#pragma mark Request Table
RequestTable::RequestTable() {
    _table = TableView::create(this, __table_size__);
    _table->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    this->addChild(_table);
    
    _fishCount = Label::createWithTTF(" ", "fonts/Marker Felt.ttf", 30);
    _fishCount->setPosition(Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width/2, -40);
    this->addChild(_fishCount);
    this->updateFishCount();
    
    _empty = Label::createWithTTF("No app request", "fonts/Marker Felt.ttf", 30);
    _empty->setPosition(_table->getPositionX() + __table_size__.width/2, _table->getPositionY() + __table_size__.height/2);
    _empty->setVisible(false);
    this->addChild(_empty);
    
    MenuItem *item = MenuItemFont::create("<Eat>", [=](Ref *sender){
        this->eateAFish();
    });
    item->setPosition(Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width - item->getContentSize().width, -40);
    Menu *menu = Menu::create(item, NULL);
    menu->setPosition(0, 0);
    this->addChild(menu);
    
    // Listener
    EventListenerCustom *listener;
    EventDispatcher *dispatcher = Director::getInstance()->getEventDispatcher();
    
    listener = EventListenerCustom::create(FacebookLoginStatusChangedNotification, [=](EventCustom *event){
        this->onPhotoDownloaded(event);
    });
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void RequestTable::onExit() {
    Node::onExit();
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

void RequestTable::onPhotoDownloaded(EventCustom *event) {
    PhotoLoaderEvent *ev = (PhotoLoaderEvent *) event;
    for (int i = 0; i < _requests.size(); i++) {
        RequestCell *cell = (RequestCell *) _table->cellAtIndex(i);
        if (cell && cell->getRequest()->getFrom()->getId() == ev->getUid()) {
            cell->updateAvatar();
        }
    }
}

void RequestTable::updateFishCount() {
    _fishCount->setString(string("You have ") + utils::StringUtils::toString(Settings::getInstance()->totalFish()) + string(" fish(es)"));
}

Size RequestTable::cellSizeForTable(TableView *table) {
    return __cell_size__;
}

TableViewCell *RequestTable::tableCellAtIndex(TableView *table, ssize_t idx) {
    RequestCell *cell = (RequestCell *) table->dequeueCell();
    GraphRequest *request = _requests.at(idx);
    if (!cell) {
        cell = new RequestCell();
        cell->autorelease();
    }
    
    cell->update(request);
    PhotoLoader::getInstance()->download(request->getFrom()->getId());
    return cell;
}

ssize_t RequestTable::numberOfCellsInTableView(TableView *table) {
    return _requests.size();
}

void RequestTable::tableCellTouched(TableView *table, TableViewCell *cell) {
    GraphRequest *r = ((RequestCell *) cell)->getRequest();
    int type = r->getType();
    switch (type) {
        case RequestTypeAskforFish:
            this->sendAFish(r->getFrom()->getId(), r->getId());
            break;
        case RequestTypeSendFish:
            Settings::getInstance()->addFish(1);
            AppRequests::getInstance()->clearRequest(r);
            this->updateFishCount();
            this->reload();
            break;
        default:
            
            break;
    }
}

void RequestTable::reload() {
    _requests.clear();
    for (auto r : AppRequests::getInstance()->getRequests(RequestTypeSendFish)) {
        _requests.pushBack(r);
    }
    for (auto r : AppRequests::getInstance()->getRequests(RequestTypeAskforFish)) {
        _requests.pushBack(r);
    }
    
    _table->reloadData();
    _empty->setVisible(_requests.size() == 0);
}

void RequestTable::sendAFish(const string &uid, const string &rid) {
    RequestDialogBuilder *builder = new RequestDialogBuilder();
    builder->setType(RequestTypeSendFish);
    builder->setMessage("Enjoy the fish!");
    builder->setTo(uid);
    
    //Explicitly state an object and action
    builder->setActionType("send");
    builder->setObjectId("773781669301483");
    
    builder->setCallback([=](int error, const string &requestId, const list<string> &recipients){
        CCLOG("Send fish result: error = %d, rid = %s", error, requestId.c_str());
        if (!error && requestId.length()) {
            AppRequests::getInstance()->clearRequest(rid);
            this->reload();
        }
    });
    
    builder->build()->show();
    builder->release();
}

void RequestTable::eateAFish() {
    if (Settings::getInstance()->totalFish()) {
        OpenGraphActionShareDialogParams *params = OpenGraphActionShareDialogParams::create();
        params->setActionType("screwfb:eat");
        params->setPreviewPropertyName("fish");
        
        OpenGraphAction *action = OpenGraphAction::create();
        
        GraphObject *fish = GraphObject::createForPost();
        fish->set("type", "screwfb:fish");
        fish->set("title", "Selfish");
        fish->set("description", "It's delicious!!");
        action->set("fish", fish->getValue());
        params->setAction(action);
        if (Dialog::canPresent(params)) {
            Dialog::present(params, [=](GraphObject *result, int error){
                CCLOG("Share status dialog callback: result = %s\nerror = %d", result ? result->getValue().getDescription().c_str() : "(null)", error);
                if (result && result->getString("completionGesture") == "post") {
                    Settings::getInstance()->eatFish(1);
                    this->updateFishCount();
                }
            });
        } else {
            Notification::getInstance()->show("Update facebook app to eat fish");
        }
    } else {
        Notification::getInstance()->show("Catch/ask for a fish to eat");
    }
}

#pragma mark Request Scene
bool RequestScene::init() {
    Scene::init();
    
    //
    EventListenerCustom *listener;
    EventDispatcher *dispatcher = Director::getInstance()->getEventDispatcher();
    
    listener = EventListenerCustom::create(FacebookRequestsDidFetchNotification, [=](EventCustom *event){
        this->onRequestsFetched();
    });
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    Menu *menu = Menu::create();
    menu->setPosition(0,0);
    this->addChild(menu);
    
    MenuItem *item;
    Size size = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

    // Table
    _table = new RequestTable();
    _table->setPosition(0, size.height - __table_size__.height);
    this->addChild(_table);
    
    float offset = 50;
    // Fetch requests
    item = MenuItemFont::create("Fetch app requests", [=](Ref *sender){
        this->fetchRequests();
    });
    item->setPosition(size.width/2, offset);
    menu->addChild(item);
    
    // Ask for a fish
    offset += 75;
    item = MenuItemFont::create("Ask for fish", [=](Ref *sender){
        this->askforAFish();
    });
    item->setPosition(size.width/2, offset);
    menu->addChild(item);
    
    // Send a fish
    offset += 75;
    item = MenuItemFont::create("Send a fish", [=](Ref *sender){
        this->sendAFish();
    });
    item->setPosition(size.width/2, offset);
    menu->addChild(item);
    
    // Back
    item = MenuItemFont::create("<<Back", [=](Ref *sender){
        Director::getInstance()->replaceScene(Home::create());
    });
    item->setPosition(item->getContentSize().width/2, 50);
    menu->addChild(item);
    
    return true;
}

void RequestScene::onEnter() {
    Scene::onEnter();
    
    _table->reload();
}

void RequestScene::onExit() {
    Scene::onExit();
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

void RequestScene::fetchRequests() {
    AppRequests::getInstance()->fetchAppRequests();
}

/* @doc https://developers.facebook.com/docs/games/requests/v2.0 */
void RequestScene::sendAFish() {
    RequestDialogBuilder *builder = new RequestDialogBuilder();
    builder->setType(RequestTypeSendFish);
    builder->setMessage("Enjoy the fish!");
    
    //Explicitly state an object and action
    builder->setActionType("send");
    builder->setObjectId("773781669301483");
    
    builder->build()->show();
    builder->release();
}

void RequestScene::askforAFish() {
    RequestDialogBuilder *builder = new RequestDialogBuilder();
    builder->setType(RequestTypeAskforFish);
    builder->setMessage("Give me a fish!");
//    builder->setTo("100008283105473");
    
    //Explicitly state an object and action
    builder->setActionType("askfor");
    builder->setObjectId("773781669301483");
    
    builder->build()->show();
    builder->release();
}

void RequestScene::onRequestsFetched() {
    _table->reload();
}
