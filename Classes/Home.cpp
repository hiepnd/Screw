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

#include "Home.h"
#include "Notification.h"
#include "RequestScene.h"

Home *Home::create() {
    Home *home = new Home();
    home->init();
    home->autorelease();
    return home;
}

bool Home::init() {
    Scene::init();
    
    EventListenerCustom *listener;
    EventDispatcher *dispatcher = Director::getInstance()->getEventDispatcher();
    
    listener = EventListenerCustom::create(FacebookLoginStatusChangedNotification, [=](EventCustom *event){
        this->onSessionStateChanged(event);
    });
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    listener = EventListenerCustom::create(FacebookUserDetailDidFetchNotification, [=](EventCustom *event){
        this->onUserDetailFetched(event);
    });
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    listener = EventListenerCustom::create(FacebookFriendsDidFetchNotification, [=](EventCustom *event){
        this->onFriendsFetched(event);
    });
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    listener = EventListenerCustom::create(PhotoLoaderLoadedNotification, [=](EventCustom *event){
        this->onPhotoDownloaded(event);
    });
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    Size size = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
    
    _greetingLbl = Label::createWithTTF("Hello", "fonts/Marker Felt.ttf", 40);
    _greetingLbl->setPosition(size.width/2, size.height - 44);
    this->addChild(_greetingLbl);
    
    // Avatar
    _avatar = Sprite::create("info-box.png");
    _avatar->setPosition(size.width - 80, size.height - 80);
    this->addChild(_avatar);
    
    // Fish Count
    _fishCount = Label::createWithTTF(" ", "fonts/Marker Felt.ttf", 30);
    _fishCount->setPosition(size.width/2, _greetingLbl->getPositionY() - 50);
    this->addChild(_fishCount);
    this->updateFishCount();
    
    Menu *menu = Menu::create();
    menu->setPosition(Point(0, 0));
    this->addChild(menu);
    
    MenuItem *item;
    MenuItemFont::setFontSize(40);
    
    float offset = 300;
    // Share link
    item = MenuItemFont::create("Share Link", [=](Ref *sender){
        this->shareLink();
    });
    item->setPosition(size.width/2, size.height/2 + offset);
    menu->addChild(item);
    
    // Share Status
    offset -= 75;
    item = MenuItemFont::create("Share Status", [=](Ref *sender){
        this->shareStatus();
    });
    item->setPosition(size.width/2, size.height/2 + offset);
    menu->addChild(item);
    
    // Share Open Graph
    offset -= 75;
    item = MenuItemFont::create("Catch a fish", [=](Ref *sender){
        this->shareOpenGraph();
    });
    item->setPosition(size.width/2, size.height/2 + offset);
    menu->addChild(item);
    
    // Fetch User Detail
    offset -= 75;
    item = MenuItemFont::create("Fetch User Detail", [=](Ref *sender){
        this->fetchUserDetail();
    });
    item->setPosition(size.width/2, size.height/2 + offset);
    menu->addChild(item);
    
    // Fetch Friends
    offset -= 75;
    item = MenuItemFont::create("Fetch Friends", [=](Ref *sender){
        this->fetchFriends();
    });
    item->setPosition(size.width/2, size.height/2 + offset);
    menu->addChild(item);
    
    // Fetch Invitable Friends
    offset -= 75;
    item = MenuItemFont::create("Fetch Invitable Friends", [=](Ref *sender){
        this->fetchInvitableFriends();
    });
    item->setPosition(size.width/2, size.height/2 + offset);
    menu->addChild(item);
    
    // Fetch Permissions
    offset -= 75;
    item = MenuItemFont::create("Fetch Permissions", [=](Ref *sender){
        this->fetchPermissions();
    });
    item->setPosition(size.width/2, size.height/2 + offset);
    menu->addChild(item);
    
    // Request publish
    offset -= 75;
    item = MenuItemFont::create("Request publish", [=](Ref *sender){
        this->requestPublish();
    });
    item->setPosition(size.width/2, size.height/2 + offset);
    menu->addChild(item);

    // Tweet
    offset -= 75;
    item = MenuItemFont::create("Tweet", [=](Ref *sender){
        screw::twitter::Twitter::tweet("Checkout the Screw https://github.com/hiepnd/Screw/",
                                       screw::utils::FileUtils::getResourcePath("HelloWorld.png"));
    });
    item->setPosition(size.width/2, size.height/2 + offset);
    menu->addChild(item);
    
    // Login/Logout
    _loginLbl = Label::createWithTTF("Login ", "fonts/Marker Felt.ttf", 40);
    item = MenuItemLabel::create(_loginLbl, [=](Ref *sender){
        this->login();
    });
    item->setPosition(size.width/2, 120);
    menu->addChild(item);
    
    // Deautorize
    item = MenuItemFont::create("Deauthorize", [=](Ref *sender){
        this->deauthorize();
    });
    item->setPosition(size.width/2, 50);
    menu->addChild(item);
    
    // Next
    item = MenuItemFont::create("Next>>", [=](Ref *sender){
        if (Session::getActiveSession()->isOpened()) {
            Director::getInstance()->replaceScene(RequestScene::create());
        } else {
            Notification::getInstance()->show("Login first");
        }
    });
    item->setPosition(size.width - item->getContentSize().width/2, 50);
    menu->addChild(item);
    
    return true;
}

void Home::onEnter() {
    Scene::onEnter();
    
    this->updateUI();
}

void Home::onExit() {
    Scene::onExit();
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

void Home::updateUI() {
    updateGreeting();
    updateAvatar();
    
    if (Session::getActiveSession()->isOpened()) {
        _loginLbl->setString("Logout");
    } else {
        _loginLbl->setString("Login ");
    }
}

void Home::updateGreeting() {
    if (Session::getActiveSession()->isOpened()) {
        GraphUser *user = Facebook::getInstance()->getUser();
        if (user) {
            _greetingLbl->setString("Hello " + user->getName());
        } else {
            _greetingLbl->setString("Hello Facebook User");
        }
    } else {
        _greetingLbl->setString("Hello guest!");
    }
}

void Home::updateAvatar() {
    if (Session::getActiveSession()->isOpened()) {
        auto user = Facebook::getInstance()->getUser();
        Texture2D *texture;
        texture = PhotoLoader::getInstance()->loadTexture(user ? user->getId() : "zz");
        if (texture) {
            _avatar->setTexture(texture);
            _avatar->setTextureRect(Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
            _avatar->setScale(128.0/texture->getContentSize().width);
        }
        _avatar->setVisible(true);
    } else {
        _avatar->setVisible(false);
    }
}

void Home::updateFishCount() {
    _fishCount->setString(string("You have ") + utils::StringUtils::toString(Settings::getInstance()->totalFish()) + string(" fish(es)"));
}

void Home::login() {
    if (Session::getActiveSession()->isOpened()) {
        Session::getActiveSession()->close();
    } else {
        Session::getActiveSession()->open(true, {"user_friends"},
                                          DefaultAudience::PUBLIC,
                                          LoginBehavior::WITH_FALLBACK_TO_WEBVIEW);
    }
}

void Home::fetchUserDetail() {
    if (Session::getActiveSession()->isOpened()) {
        Facebook::getInstance()->fetchUserDetails(nullptr);
    } else {
        Notification::getInstance()->show("Login first");
    }
}

void Home::fetchFriends() {
    if (Session::getActiveSession()->isOpened()) {
        Facebook::getInstance()->fetchFriends(nullptr);
    } else {
        Notification::getInstance()->show("Login first");
    }
}

void Home::fetchInvitableFriends() {
    if (Session::getActiveSession()->isOpened()) {
        Request *request = Request::create("me/invitable_friends");
        request->setCallback([](int error, GraphObject *result){
            CCLOG("%s - error = %d", __func__, error);
            if (!error && result) {
                auto friends = result->getPropertyAsList<GraphUser>("data");
                
            }
        });
        request->execute();
    } else {
        Notification::getInstance()->show("Login first");
    }
}

void Home::fetchPermissions() {
    if (Session::getActiveSession()->isOpened()) {
        Request *r = Request::create("me/permissions");
        r->setCallback([](int error, GraphObject *result){
            CCLOG("Permissions = %s", result ? result->getValue().getDescription().c_str() : "(error)");
        });
        r->execute();
    } else {
        Notification::getInstance()->show("Login first");
    }
}

void Home::requestPublish() {
    if (Session::getActiveSession()->isOpened()) {
        if (Session::getActiveSession()->hasPermission("publish_actions")) {
            Notification::getInstance()->show("Publish action already granted");
        } else {
            Session::getActiveSession()->requestPublishPermissions({"publish_actions"});
        }
    } else {
        Notification::getInstance()->show("Login first");
    }
}

void Home::shareLink() {
    ShareDialogParams *params = ShareDialogParams::create();
    params->setLink("http://wingup.nopowerup.com");
//    params->setName("A name");
//    params->setCaption("A caption");
//    params->setDescription("Screw aims to bring native Facebook SDK functionalities to cocos2d-x");
//    params->setFriends({"100008289311268"});
//    params->setDataFailuresFatal(false);
    
    if (Dialog::canPresent(params)) {
        Dialog::present(params, [](GraphObject *result, int error){
            CCLOG("Share link dialog callback: result = %s\nerror = %d", result ? result->getValue().getDescription().c_str() : "(null)", error);
        });
    } else {
        CCLOG("Cannot show share dialog, fallback to webview");
        FeedDialogBuilder *fbd = new FeedDialogBuilder();
        fbd->setLink(params->getLink())->setDescription(params->getDescription());
        fbd->setTo("100008289311268");
        
        fbd->setCallback([](int error, const string &rid){
            CCLOG("Share link web dialog result: error = %d, rid = %s", error, rid.c_str());
        });
        
        fbd->build()->show();
        delete fbd;
    }
}

void Home::shareStatus() {
    ShareDialogParams *params = ShareDialogParams::create();
//    params->setDescription("Some description");
//    params->setFriends({"100008211700580", "100008307900261"});
//    params->setDataFailuresFatal(false);
    
    if (Dialog::canPresent(params)) {
        Dialog::present(params, [](GraphObject *result, int error){
            CCLOG("Share status dialog callback: result = %s\nerror = %d", result ? result->getValue().getDescription().c_str() : "(null)", error);
        });
    } else {
        CCLOG("Cannot show share dialog, fallback to webview");
        FeedDialogBuilder *fbd = new FeedDialogBuilder();
        
        fbd->setCallback([](int error, const string &rid){
            CCLOG("Share status web dialog result: error = %d, rid = %s", error, rid.c_str());
        });
        
        fbd->build()->show();
        delete fbd;
    }
}

/* @doc https://developers.facebook.com/docs/ios/open-graph/ */
void Home::shareOpenGraph() {
    OpenGraphActionShareDialogParams *params = OpenGraphActionShareDialogParams::create();
    params->setActionType("screwfb:catch");
    params->setPreviewPropertyName("fish");
 
    GraphObject *fish = GraphObject::createForPost();
    fish->set("type", "screwfb:fish");
    fish->set("title", "Selfish");
    fish->set("description", "The biggest fish in the sea!!!");
//    fish->set("url", "http://samples.ogp.me/344468272304428");
//    fish->set("image", "http://i.imgur.com/g3Qc1HN.png");
    
    OpenGraphAction *action = OpenGraphAction::create();
    action->set("fish", fish->getValue());
//    action->set("fish", "773781669301483"); //For catching a fish with its id
    params->setAction(action);
    if (Dialog::canPresent(params)) {
        Dialog::present(params, [=](GraphObject *result, int error){
            CCLOG("Share status dialog callback: result = %s\nerror = %d", result ? result->getValue().getDescription().c_str() : "(null)", error);
            if (result && result->getString("completionGesture") == "post") {
                Settings::getInstance()->addFish(1);
                this->updateFishCount();
            }
        });
    }
}

void Home::deauthorize() {
    if (!Session::getActiveSession()->isOpened()) {
        Notification::getInstance()->show("Login first");
        return;
    }
    
    Request *r = Request::requestForDelete("me/permissions", [](int error, bool success){
        CCLOG("Delete request - success = %d", success);
        Session::getActiveSession()->close();
    });
    
    r->execute();
}

#pragma mark Event
void Home::onSessionStateChanged(EventCustom *event) {
    // If session is opened and no user detail found
    if (Session::getActiveSession()->isOpened() && !Facebook::getInstance()->getUser()) {
        this->fetchUserDetail();
    }
    
    this->updateUI();
}

void Home::onUserDetailFetched(EventCustom *event) {
    this->updateGreeting();
    this->updateAvatar();
    auto user = Facebook::getInstance()->getUser();
    PhotoLoader::getInstance()->download(user->getId());
}

void Home::onFriendsFetched(EventCustom *event) {
    auto friends = Facebook::getInstance()->getFriends();
    for (auto i : friends) {
        CCLOG("%s", i->getName().c_str());
    }
}

void Home::onPhotoDownloaded(EventCustom *event) {
    this->updateAvatar();
}