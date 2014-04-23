#include "HelloWorldScene.h"
#include <functional>
#include "screw.h"

USING_NS_CC;
USING_NS_SCREW_FACEBOOK;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.


//    MenuItemImage::create("", "", )

    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemImage *closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           [this](Object *sender){
                                               this->menuCloseCallback(nullptr);
                                           });
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "fonts/Marker Felt.ttf", 44);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    auto lbl = LabelTTF::create(Session::getActiveSession()->isOpened() ? "Logout" : "Login", "Marker Felt", 40);

    
    Session::getActiveSession()->setStatusCallback([lbl](Session *session, SessionError *error){
        CCLOG("Session status callback - state = %d", session->getState());
        if (session->isOpened()) {
            lbl->setString("Logout");
        } else {
            lbl->setString("Login");
        }
    });
    
    auto login = MenuItemLabel::create(lbl);
    login->setCallback([](Object *sender){
        if (Session::getActiveSession()->isOpened()) {
            Session::getActiveSession()->close();
        } else {
            Session::getActiveSession()->open(true, {}, DefaultAudience::PUBLIC, LoginBehavior::WITH_FALLBACK_TO_WEBVIEW);
        }
    });

    login->setPosition(visibleSize.width/2, visibleSize.height/2);
    menu->addChild(login);
    
    //Request write
    auto request = MenuItemFont::create("Request write", [](Object *sender){
        if (!Session::getActiveSession()->isOpened()) {
            CCLOG("%s - session not opened, login first", __func__);
            return;
        }
        if (!Session::getActiveSession()->hasPermission("publish_actions")) {
            Session::getActiveSession()->requestPublishPermissions({"publish_actions"});
        } else {
            CCLOG("%s - you already have publish permission", __func__);
        }
    });
    request->setPosition(visibleSize.width/2 + 200, visibleSize.height/2 + 50);
    menu->addChild(request);
    
    //Get Friends
    auto fetch = MenuItemFont::create("Fetch Friends", [](Object *sender){
		if (!Session::getActiveSession()->isOpened()) {
			CCLOG("%s - session not opened, login first", __func__);
			return;
		}

		Request *r = Request::requestForFriends([](int error, const Vector<GraphUser *> &friends){
            CCLOG("Fetch friends callback - error = %d", error);
            int i = 0;
            for (GraphUser *user : friends) {
                CCLOG("friend #%d: %s", i++, user->getValue().getDescription().c_str());
            }
        });
        
		
		r->execute();

	});
    fetch->setPosition(visibleSize.width/2 + 200, visibleSize.height/2 - 50);
	menu->addChild(fetch);
    
    //Get App request
    auto fetch2 = MenuItemFont::create("Fetch App requests", [](Object *sender){
		if (!Session::getActiveSession()->isOpened()) {
			CCLOG("%s - session not opened, login first", __func__);
			return;
		}
        
		Request *r = Request::requestForAppRequests([](int error, const Vector<GraphRequest *> &requests){
            CCLOG("Fetch requests call back - error = %d", error);
            for (GraphRequest *r : requests) {
                CCLOG("%s, from %s, to %s, message = %s", r->getId().c_str(), r->getFrom()->getName().c_str(),
                      r->getTo()->getName().c_str(), r->getMessage().c_str());
                
                string rid = r->getId();
                Request *rq = Request::requestForDelete(rid, nullptr);
                rq->setCallback([=](int error, GraphObject *result){
                    CCLOG("Delete %s result, error = %d, response = %s", rid.c_str(), error, result ? result->getValue().getDescription().c_str() : "");
                });
                rq->execute();
            }
        });
		
		r->execute();
        
	});
    fetch2->setPosition(visibleSize.width/2 - 200, visibleSize.height/2 - 50);
	menu->addChild(fetch2);
    
    //request
    auto apprequest = MenuItemFont::create("Request", [](Object *sender){
		if (!Session::getActiveSession()->isOpened()) {
			CCLOG("%s - session not opened, login first", __func__);
			return;
		}
        
        RequestDialogBuilder *rdb = new RequestDialogBuilder();
        rdb->setMessage("Hello");
        rdb->setTitle("title");
        rdb->setType(10);
        rdb->setData("score", "1000");
        
        rdb->setCallback([](int error, const string &rid, const list<string> &recipients){
            CCLOG("App request callback: error = %d, id = %s, recipients = %s", error, rid.c_str(), screw::utils::StringUtils::join(recipients, ",").c_str());
        });
        
        rdb->build()->show();
        delete rdb;
        
	});
    apprequest->setPosition(visibleSize.width/2 - 200, visibleSize.height/2 + 50);
	menu->addChild(apprequest);
    
    
    //Feed
    auto feedDialog = MenuItemFont::create("Feed", [](Object *sender){
		if (!Session::getActiveSession()->isOpened()) {
			CCLOG("%s - session not opened, login first", __func__);
			return;
		}

        FeedDialogBuilder *fdb = new FeedDialogBuilder();
        fdb->setLink("https://github.com/hiepnd");
        fdb->setCaption("Caption");
        fdb->setName("Name");
        fdb->setDescription("Description");
        
        fdb->setCallback([](int error, const string &rid){
            CCLOG("Feed dialog result: error = %d, rid = %s", error, rid.c_str());
            if (rid.length() > 0) {
                Request::requestForDelete(rid, [=](int error, bool success){
                    CCLOG("Delete request %s %s", rid.c_str(), success ? "Success" : "Fail");
                })->execute();
            }
        });
        
        fdb->build()->show();
        delete fdb;
        
//		WebDialog *dialog = new WebDialog();
//        dialog->setParams(feedParams);
//        dialog->setDialog("feed");
//        dialog->setCallback([](int error, const string &requestId, const list<string> &recveivers){
//            CCLOG("App request - error = %d, id = '%s', receivers = [%s]", error, requestId.c_str(), screw::utils::StringUtils::join(recveivers, ",").c_str());
//        });
//        dialog->show();
//        dialog->release();
        
	});
    feedDialog->setPosition(visibleSize.width/2 - 200, visibleSize.height/2 + 150);
	menu->addChild(feedDialog);

    
    //Get App score
    auto scores = MenuItemFont::create("Fetch Scores", [](Object *sender){
		if (!Session::getActiveSession()->isOpened()) {
			CCLOG("%s - session not opened, login first", __func__);
			return;
		}
        
		Request *r = Request::requestForScores([](int error, const Vector<GraphScore *> &scores){
            CCLOG("Fetch scores callback - error = %d", error);
            for (GraphScore *s : scores) {
                CCLOG("(%s, %ld)", s->getUser()->getName().c_str(), s->getScore());
            }
        });
		
		r->execute();
        
	});
    scores->setPosition(visibleSize.width/2, visibleSize.height/2 + 100);
	menu->addChild(scores);
    
    //Deauthorize
    auto deauthorize = MenuItemFont::create("Deauthorize", [](Object *sender){
		if (!Session::getActiveSession()->isOpened()) {
			CCLOG("%s - session not opened, login first", __func__);
			return;
		}
        
		Request *r = Request::requestForDelete("me/permissions", [](int error, bool success){
            CCLOG("Delete request - success = %d", success);
            Session::getActiveSession()->close();
        });
		
		r->execute();
        
	});
    deauthorize->setPosition(visibleSize.width/2, visibleSize.height/2 - 200);
	menu->addChild(deauthorize);
    
    return true;
}


void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
