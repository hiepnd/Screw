Screw
-----
Screw aims to bring native Facebook SDK functionalities to cocos2d-x.

Run the Project
--------------
The project accompanies with a simple sample for demo. You should run the project to explore the basic functionalities of Screw. Besides, examining the sample files (`Home.cpp` and `RequestScene.cpp`) is a good reference.
In Android, due to the requirement of adding a Key Hash (which is developer specific) in App Dashboard, you must use your own Facebook app and follow Facebook instruction to setup your app.

**Note to use your own Facebook app:** 
You must configure your app to include:

* A *Fish* object type
* A *Catch fish* action type
* An *Eat fish* action type
* A sample *Fish* id

And then scan `Home.cpp` and `RequestScene.cpp` to update these info.

Requirements
------------
* iOS/Android
* cocos2d-x v3.0beta or higher (Not compatible with cocos2d-x 2.x)
* Screw is built against Facebook SDK 3.14 (both iOS and Android), but older SDKs may work

Features
--------
###Basic
* Facebook Login with permissions, default audience, login behavior parameters
* Request new read/publish permissions
* Facebook logout/deauthorize
* 'Nearly' full native Graph API calls
* WebDialog's functionalities include send app requests, update status
* Dialog's functionalities include share links, status update and publish Open Graph stories (share photo is not yet supported)
* Various builders to send app requests, publish Open Graph stories and share link/status
* Callback as `std::function`

###Extended
* App Requests Manager (`screw::facebook::AppRequests`)
* `screw::PhotoLoader` to download profile photos and load textures
* `screw::Data` to read/write plist file with convenient path based key getters/setters
* `screw::facebook::Facebook` to fetch/cache Facebook profiles/scores and more
* Many other utils

Setup
-----
All the sources lie under `Classes/screw`, `proj.ios_mac/screw`, `proj.android/jni/screw` and `proj.android/src/com/screw`, so basically you should mimic this structure in your project.
###Integrate Screw into your iOS cocos2d-x app###
* Copy `Classes/screw` and `proj.ios_mac/ios/screw` to the corresponding folders and add them as groups into your Xcode project
* Modify your `AppDelegate.cpp` to include `screw/screw.h` and add `screw::facebook::Session::start();` as the first command in `AppDelegate::applicationDidFinishLaunching()`
* Modify `- (BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation ` in your `AppControler.mm` to invoke `[FBAppCall handleOpenURL:url sourceApplication:sourceApplication];`
* Copy `fb-default.png` to `Resources` folder and add it to Xcode project

###Integrate Screw into your Android cocos2d-x app###
* Copy `Classes/screw` to your project if you haven't
* Modify `AppDelegate.cpp` as with iOS project
* Copy `proj.android/jni/screw` and `proj.android/src/com/screw` to corresponding locations in your project
* Make `jni/screw` part of NDK_MODULE_PATH
* Update your Android.mk to have `LOCAL_WHOLE_STATIC_LIBRARIES += screw_static` and `$(call import-module,screw)`
* In `AppActivity.onCreate` add `Facebook.onActivityCreate(this, savedInstanceState);`
* In `AppActivity.onPause` add `Facebook.onActivityPause();`
* In `AppActivity.onResume` add `Facebook.onActivityResume();`
* In `AppActivity.onActivityResult` add `Facebook.onActivityResult(requestCode, resultCode, data)`
* Copy `fb-default.png` to `Resources` folder (if you haven't)

Tutorial
--------
The codes below assume that you have  `#include "screw/screw.h"` and `using namespace screw::facebook`.
###Login/Logout
Open loaded session without UI
```
if (Session::getActiveSession()->getState() == Session::State::CREATED_TOKEN_LOADED) {
        Session::getActiveSession()->open(false);
}
```
Login
```
Session::getActiveSession()->open(true, {"user_friends"},
                                          DefaultAudience::PUBLIC,
                                          LoginBehavior::WITH_FALLBACK_TO_WEBVIEW);
```

Set status callback                                          
```
Session::getActiveSession()->setStatusCallback([=](Session *session, SessionError *error){
        this->sessionStatusCallback(session, error);
});
```
Logout 
```
Session::getActiveSession()->close();
```

###Request new permissions
```
Session::getActiveSession()->requestPublishPermissions({"publish_actions"});
```

###Fetch User Details
Using `screw::facebook::Request` directly
```
Request::requestForMe([](int error, GraphUser *user){
    // Log it out
    CCLOG("Fetch User Details Callback: error = %d, user = %s", error, 
        user ? user->getValue()->getDescription().c_str() : "(null)");
    // user does not persist anywhere, so this is the only chance to deal with it
})->execute();
```

Using `screw::facebook::Facebook`. Every results of `fetch*` by `screw::facebook::Facebook` are store locally for further `get*`
```
// You first need to add an Event Listener to listen to FacebookUserDetailDidFetchNotification event 
// (Assume that these codes are executed inside an instance method of a Node)
EventListenerCustom *listener = EventListenerCustom::create(FacebookUserDetailDidFetchNotification, [=](EventCustom *event){
    // Get the user object from Facebook
    GraphUser *user = Facebook::getInstance()->getUser();
    // Always make this check
    if (user) {
        // Update UI, etc
    }
});
_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

// And then call, the event is fired upon request finishes
Facebook::getInstance()->fetchUserDetail();
```

###Fetch Friends
Using `screw::facebook::Request`
```
Request::requestForFriends([](int error, const Vector<GraphUser *> &friends){
    // This friends list is only available in this callback
})->execute();
```

Using `screw::facebook::Facebook` as helper (the same pattern as with fetching user details)
```
EventListenerCustom *listener = EventListenerCustom::create(FacebookFriendsDidFetchNotification, [=](EventCustom *event){
    // Get the friend list from Facebook
    Vector<GraphUser *> friends = Facebook::getInstance()->getFriends();
    // Do something with the friends list
});
_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

// Friends fetched with this call is stored 
Facebook::getInstance()->fetchFriends();
```

###Fetch Scores
Using `screw::facebook::Request`
```
Request::requestForScores([](int error, const Vector<GraphScore *> &scores){
    for (auto s : scores) {
        CCLOG("(user %s, score %ld)", s->getUser()->getName().c_str(), s->getScore());
    }
})->execute();
```
Using `screw::facebook::Facebook` as helper (the same pattern as with fetching user details
```
EventListenerCustom *listener = EventListenerCustom::create(FacebookScoresDidFetchNotification, [=](EventCustom *event){
    // Get the friend list from Facebook
    // Friends along with scores are stored persistently if you fetch them via screw::facebook::Facebook so you can also get them outside callback 
    Vector<GraphUser *> friends = Facebook::getInstance()->getFriends();
    for (auto f : friends) {
        CCLOG("(user %s, score %ls)", f->getName(), f->getScore());
    }
});
_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

Facebook::getInstance()->fetchScores();
```

###Post Score
```
// This call first fetches score from facebook, and if the fetched score  is less than the passed score it then posts the new score
Facebook::getInstance()->postScore(score);
```

###App requests
Screw helps support 'typed' app requests by implementing the 'data' extra field of a request as a JSON string and provides helper functions to manipulate this data.

Simplest form
```
RequestDialogBuilder *builder = new RequestDialogBuilder();
builder->setMessage("I'm sending an app request");
builder->build()->show();
builder->release();
```
Need a callback, add this prior to `builder->build()->show()`
```
builder->setCallback([](int error, const string &requestId, const list<string> &recipients){
    if (requestId.length()) {
        // Request succeeded
    }
});
```
Send to specific friend(s)
```
builder->setTo({"id1", "id2"})
```
Explicitly state an object and action - the request will appear in notification as "Mark sent you a fish" instead of boring "Mark sent you a request"
```
builder->setActionType("send"); // May be 'askfor' if you want to ask for a fish
builder->setObjectId("773781669301483"); // A fish object id
```
Support 'typed' requests (*Screw specific, not official facebook functionality*)
```
// First declare some types
enum {
    RequestTypeSendFish = 1,
    RequestTypeAskforFish,
};

// To send a 'SendFish' request, add following. This results in a 'data' extra field as "{_t:1}"
builder->setType(RequestTypeSendFish);

// You can add more to 'data'
builder->setData("count", "10"); // ==> data = "{_t:1,count:10}", ie. send 10 fishes
```
Fetch app requests - use `screw::facebook::AppRequests` as app request manager.
Upon app requests fetched, `AppRequests`:

* Removes non-JSON 'data' fields and parses JSON 'data' into `ValueMap`
* Deletes requests from facebook (read `AppRequests::didFetchAppRequests` for details)
* Store the requests locally for further `get*`
```
EventListenerCustom *listener = EventListenerCustom::create(FacebookRequestsDidFetchNotification, [=](EventCustom *event){
    // 
});
_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

AppRequests::getInstance()->fetchAppRequests();
```
Get the fetched app requests from `AppRequests`
```
Vector<GraphRequest *> getRequests(); // Get all requests
Vector<GraphRequest *> getRequests(int type); // Get requests with type as specified in builder->setType(type)
GraphRequest *getRequest(const string &rid); // Get request with specific id
```
Examine the request
```
Vector<GraphRequest *> requests = AppRequests::getInstance()->getRequests();
for (GraphRequest *r : requests) {
    CCLOG("Request %s", r->getId().c_str());
    CCLOG("    Type = %d", r->getType()); // O if not set
    CCLOG("    Data = %s", r->getDataObject() ? r->getDataObject()->getDescription().c_str() : "(null)");
    // If the request was sent with action type and object specified
    CCLOG("    Action type = %s", r->getString("action_type").c_str());
    if (!r->get("object").isNull()) {
        CCLOG("    ObjectID = %s", r->getString("object/id").c_str());
    }
}
```
Clear app request
```
AppRequests::getInstance()->clearRequest(requestId);
```

###Sharing
Share a link
```
ShareDialogParams *params = ShareDialogParams::create();
params->setLink("http://www.cocos2d-x.org/");
//    params->setName("A name");
//    params->setCaption("A caption");
//    params->setDescription("A description");
//    params->setFriends({"100008289311268"});
//    params->setDataFailuresFatal(false);
    
// Check if can share via Share Dialog 
if (Dialog::canPresent(params)) {
    Dialog::present(params, [](GraphObject *result, int error){
        if (result && result->getString("completionGesture") == "post") {
            // Share succeeded
        }
    });
} else {
    // Fallback to web view
    CCLOG("Cannot show share dialog, fallback to webview");
    FeedDialogBuilder *fbd = new FeedDialogBuilder();
    fbd->setLink(params->getLink())->setDescription(params->getDescription());
    //fbd->setTo("100008289311268");
        
    fbd->setCallback([](int error, const string &rid){
        if (rid.length()) {
            // Share succeeded
        }
    });
        
    fbd->build()->show();
    delete fbd;
}
```
Share status: identical to share link without providing a link parameter
Share Open Graph Story
```
// Share a "Catch Fish" story
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
        if (result && result->getString("completionGesture") == "post") {
            // Share succeeded
        }
    });
} else {
    // Fallback to app request: require "publish_actions" permission and a litte more work
}
```

###Photo Loader
Download profile picture
```
PhotoLoader::getInstance()->download(uid, size);
```
Upon donload finished, `PhotoLoader` saves image under `facebook-images` folder inside writable directory.
Handle download finish event and load texture
```
EventListenerCustom *listener = EventListenerCustom::create(PhotoLoaderLoadedNotification, [=](EventCustom *event){
    PhotoLoaderEvent *ev = (PhotoLoaderEvent *) event;
    Texture2D *texture = PhotoLoader::getInstance()->loadTexture(ev->getUid());
});
_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
```
**Notes**

* `PhotoLoader::getInstance()->loadTexture(uid)` guarantees to return a non-nil texture (if the image for uid doesn't exist, it loads the default texture, so don't forget to copy `fb-default.png` to your `Resources` folder)
* Textures returned by `PhotoLoader::getInstance()->loadTexture` are not managed by `TextureCache` (except the defaut texture)

Notes
-----
* If you decide to use `screw::facebook::Facebook`
  * You must NOT set up your own session callback via `screw::facebook::Session::setStatusCallback`, instead observing status changes via `FacebookLoginStatusChangedNotification` event.
  * You should use `screw::facebook::Facebook::fetch*` methods, observe fetch completion via event dispatcher and get the data via `screw::facebook::Facebook::get*`
  * Although all `fetch*` method accept a callback, you should not use it to reduce the overhead of managing objects life cycle
* If you decide to use `screw::facebook::AppRequests`
  * You MUST use `screw::facebook::AppRequests::fetchAppRequests` instead of `Request::requestForAppRequests`

Contact
-------
* Email: hiepndhut@gmail.com
* Twitter: @hiepnd
