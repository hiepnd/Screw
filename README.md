Screw
=====
Screw aims to bring native Facebook SDK functionalities to cocos2d-x.

Run the Sample
--------------


Requirements
------------
* cocos2d-x v3.0beta or higher (Not compatible with cocos2d-x 2.x)
* Screw is built against Facebook SDK 3.14 (both iOS and Android), but older SDKs may work

Setup
-----
###Integrate Screw into your iOS cocos2d-x app###
* Copy `Classes/screw` and `proj.ios_mac/ios/screw` to the coresponding folders and them as groups into your Xcode project
* Modify your `AppDelegate.cpp` to include `screw/screw.h` and add `screw::facebook::Session::start();` as the first command in `AppDelegate::applicationDidFinishLaunching()`
* Modify `- (BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation ` in your `AppControler.mm` to invoke `[FBAppCall handleOpenURL:url sourceApplication:sourceApplication];`

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

Tutorial
--------
The codes below assume that you have  `#include "screw/screw.h"` and `using namespace screw::facebook`.
###Login/Logout
```
// Open loaded session without UI
if (Session::getActiveSession()->getState() == Session::State::CREATED_TOKEN_LOADED) {
        Session::getActiveSession()->open(false);
}

// Login 
Session::getActiveSession()->open(true, {"user_friends"},
                                          DefaultAudience::PUBLIC,
                                          LoginBehavior::WITH_FALLBACK_TO_WEBVIEW);

// Set status callback                                          
Session::getActiveSession()->setStatusCallback([=](Session *session, SessionError *error){
        this->sessionStatusCallback(session, error);
});
                                          
// Logout 
Session::getActiveSession()->close();
```

###Request new permissions
```
Session::getActiveSession()->requestPublishPermissions({"publish_actions"});
```

###Fetch User Details
```
/* Using screw::facebook::Request */
Request::requestForMe([](int error, GraphUser *user){
    // Log it out
    CCLOG("Fetch User Details Callback: error = %d, user = %s", error, 
        user ? user->getValue()->getDescription().c_str() : "(null)");
})->execute();

/* Using screw::facebook::Facebook as helper */
// You first need to add an Event Listener to listen to FacebookUserDetailDidFetchNotification event (assume that these codes are executed inside an instance method of a Node)
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
```
/* Using screw::facebook::Request */
Request::requestForFriends([](int error, const Vector<GraphUser *> &friends){
    // Do something with the friends list
})->execute();

/* Using screw::facebook::Facebook as helper (the same pattern as with fetching user details) */
EventListenerCustom *listener = EventListenerCustom::create(FacebookFriendsDidFetchNotification, [=](EventCustom *event){
    // Get the friend list from Facebook
    Vector<GraphUser *> friends = Facebook::getInstance()->getFriends();
    // Do something with the friends list
});
_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

Facebook::getInstance()->fetchFriends();
```

###Fetch Scores
```
/* Using screw::facebook::Request */
Request::requestForScores([](int error, const Vector<GraphScore *> &scores){
    for (auto s : scores) {
        CCLOG("(user %s, score %ld)", s->getUser()->getName().c_str(), s->getScore());
    }
})->execute();

/* Using screw::facebook::Facebook as helper (the same pattern as with fetching user details) */
EventListenerCustom *listener = EventListenerCustom::create(FacebookScoresDidFetchNotification, [=](EventCustom *event){
    // Get the friend list from Facebook
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
Screw helps support 'typed' app requests by implementing the 'data' extra field of a request as a JSON string and provides helper functions to manipulate this data
```
/* Send app request using screw::facebook::RequestDialogBuilder */
// Simplest form
RequestDialogBuilder *builder = new RequestDialogBuilder();
builder->setMessage("I'm sending an app request");
builder->build()->show();
builder->release();

// Need a callback, add this prior to show()
builder->setCallback([](int error, const string &requestId, const list<string> &recipients){
    if (requestId.length()) {
        // Request succeeded
    }
});

// Send to specific friend(s)
builder->setTo({"id1", "id2"})

//Explicitly state an object and action - the request will appear in notification as "Mark sent you a fish" rather than boring "Mark sent you a request"
builder->setActionType("send");
builder->setObjectId("773781669301483"); // A fish object id

// Support 'typed' requests (Screw specific, not official facebook functionality)
// First declare some types
enum {
    RequestTypeSendFish = 1,
    RequestTypeAskforFish,
};

// To send a 'SendFish' request, add following. This results in a 'data' extra field as "{_t:1}"
builder->setType(RequestTypeSendFish);

// You can add more to 'data'
builder->setData("count", "10"); // ==> data = "{_t:1,count:10}", ie. send 10 fishes

/* Fetch app requests - use screw::facebook::AppRequests as app request manager */
// Be awared: upon fetched app requests, AppRequests removes non JSON 'data' fields and parses JSON 'data' into ValueMap

EventListenerCustom *listener = EventListenerCustom::create(FacebookRequestsDidFetchNotification, [=](EventCustom *event){
    // 
});
_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

AppRequests::getInstance()->fetchAppRequests();

// Get the app requests from AppRequests
Vector<GraphRequest *> getRequests(); // Get all requests
Vector<GraphRequest *> getRequests(int type); // Get requests with type as specified in builder->setType(type)
GraphRequest *getRequest(const string &rid); // Get request with specific id

// Examine the request
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

// Clear app requests
AppRequests::getInstance()->clearRequest(requestId);
```
