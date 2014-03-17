#include "HelloWorldScene.h"
#include <functional>
#include "screw/facebook/Session.h"

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
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
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
    
    auto lbl = LabelTTF::create(Session::getActiveSession()->isOpened() ? "Logout" : "Login", "Arial", 40);

    
    Session::getActiveSession()->setStatusCallback([lbl](Session *session){
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
            Session::getActiveSession()->open();
        }
    });

    login->setPosition(visibleSize.width/2, visibleSize.height/2);
    menu->addChild(login);
    
    auto request = MenuItemFont::create("Request write", [](Object *sender){
        if (!Session::getActiveSession()->isOpened()) {
            CCLOG("%s - session not opened, login first", __func__);
            return;
        }
        if (!Session::getActiveSession()->hasPermission("publish_actions")) {
            Session::getActiveSession()->requestPublishPermission("publish_actions");
        } else {
            CCLOG("%s - you already have publish permission", __func__);
        }
    });
    request->setPosition(visibleSize.width/2 + 200, visibleSize.height/2 + 50);
    menu->addChild(request);
    
    
    return true;
}


void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
