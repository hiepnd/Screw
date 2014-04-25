#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "screw/screw.h"

USING_NS_CC;
USING_NS_SCREW_FACEBOOK;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
	CCLOG("AppDelegate::applicationDidFinishLaunching");
    screw::facebook::Session::start();
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
	
    //
    glview->setDesignResolutionSize(960, 640, ResolutionPolicy::SHOW_ALL);
    Application::Platform platform = Application::getInstance()->getTargetPlatform();
    float resourceWidth;
    if (platform == Application::Platform::OS_IPHONE) {
        resourceWidth = 960;
    } else if (platform == Application::Platform::OS_IPAD) {
        resourceWidth = 1024;
    } else if (platform == Application::Platform::OS_ANDROID) {
        resourceWidth = 800;
    }
    director->setContentScaleFactor(resourceWidth / 960.0);
    
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    if (Session::getActiveSession()->getState() == Session::State::CREATED_TOKEN_LOADED) {
        Session::getActiveSession()->open(false);
    }
    
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
