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
	
    //Design size
    Size frameSize = glview->getFrameSize();
    float minAspectRatio = 1.5;
    float maxAspectRatio = 1136.0/640;
    float aspectRatio = frameSize.width / frameSize.height;
    aspectRatio = clampf(aspectRatio, minAspectRatio, maxAspectRatio);
    
    glview->setDesignResolutionSize(640*aspectRatio, 640, ResolutionPolicy::SHOW_ALL);
    Application::Platform platform = Application::getInstance()->getTargetPlatform();
    float resourceHeight;
    if (platform == Application::Platform::OS_IPHONE) {
        resourceHeight = 640;
    } else if (platform == Application::Platform::OS_IPAD) {
        resourceHeight = 640;
    } else if (platform == Application::Platform::OS_ANDROID) {
        resourceHeight = 480;
    }
    director->setContentScaleFactor(resourceHeight / 640);
    
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
