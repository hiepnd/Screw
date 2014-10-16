/****************************************************************************
 Copyright (c) hiepndhut@gmail.com
 Copyright (c) 2014 No PowerUp Games
 
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

#include "Facebook.h"
#include "../utils/FileUtils.h"

NS_SCREW_FACEBOOK_BEGIN

#define FACEBOOK_KEEP_ALL_FRIENDS 1

const string FacebookLoginStatusChangedNotification = "FacebookLoginStatusChangedNotification";
const string FacebookUserDetailDidFetchNotification = "FacebookUserDetailDidFetchNotification";
const string FacebookFriendsDidFetchNotification = "FacebookFriendsDidFetchNotification";
const string FacebookScoresDidFetchNotification = "FacebookScoresDidFetchNotification";


//Data key
static const string FacebookDataProfilesKey              = "__profiles__";
static const string FacebookDataUserIDKey                = "__user_id__";
static const string FacebookDataDirtyScoreKey            = "__dirty_score__";

#define FACEBOOK_PROFILE_KEY(uid)           (FacebookDataProfilesKey + "/" + uid)
#define PATH_JOIN(s1, s2) (s1 + string("/") + s2)

#define RETURN_IF_STATE_SET(bit, msg) if (_fetchingBits & (bit)) {CCLOG("%s", msg); return;}

#define FB_SET_FETCHING_STATE(bit)      _fetchingBits |= bit;
#define FB_CLEAR_FETCHING_STATE(bit)    _fetchingBits &= ~bit;

Facebook *Facebook::_instance = nullptr;

Facebook *Facebook::getInstance() {
    if (!_instance) {
        _instance = new Facebook();
    }
    
    return _instance;
}

Facebook::Facebook() {
	// TODO Auto-generated constructor stub
    _data = new data::Data(ValueMap(), utils::FileUtils::getDocumentPath("facebook.plist"));
    _fetchingBits = 0x00;
    
    //Facebook instance is the exclusive status callback
    Session::getActiveSession()->setStatusCallback([=](Session *session, SessionError *error){
        this->sessionStatusCallback(session, error);
    });
}

Facebook::~Facebook() {
	// TODO Auto-generated destructor stub
    Session::getActiveSession()->setStatusCallback(nullptr);
    delete _data;
}

void Facebook::start() {
    if (Session::getActiveSession()->getState() == Session::State::CREATED_TOKEN_LOADED) {
        Session::getActiveSession()->open(false);
    }
}

#pragma mark Get
GraphUser *Facebook::getUser() {
    string uid = _data->getString(FacebookDataUserIDKey);
    if (uid.length()) {
        Value &userData = _data->get(FACEBOOK_PROFILE_KEY(uid));
        if (!userData.isNull()) {
            return GraphUser::create(userData);
        }
    }
    
    return nullptr;
}

Vector<GraphUser *> Facebook::getFriends() {
    Vector<GraphUser *> friends;
    if (!_data->get(FacebookDataProfilesKey).isNull()) {
        ValueMap &friendData = _data->get(FacebookDataProfilesKey).asValueMap();
        string uid = _data->getString(FacebookDataUserIDKey);
        for (auto i : friendData) {
            if (i.first != uid) {
                friends.pushBack(GraphUser::create(i.second));
            }
        }
    }
    
    return friends;
}

GraphUser *Facebook::getFriend(const string &uid) {
    Value &userData = _data->get(FACEBOOK_PROFILE_KEY(uid));
    if (!userData.isNull()) {
        return GraphUser::create(userData);
    }
    return nullptr;
}

#pragma mark Fetch
void Facebook::fetchUserDetails(const MeRequestCallback &handler) {
    RETURN_IF_STATE_SET(FacebookFetchingUserDetailBit, "Facebook::fetchUserDetails - another request is in progress");
    
    FB_SET_FETCHING_STATE(FacebookFetchingUserDetailBit);
    Request::requestForMe([=](int error, GraphUser *user){
        if (!error && user) {
            this->didFetchUserDetail(user);
            //Fire notification
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(FacebookUserDetailDidFetchNotification);
        }
        if (handler) {
            handler(error, user);
        }
        FB_CLEAR_FETCHING_STATE(FacebookFetchingUserDetailBit);
    })->execute();
}

void Facebook::fetchFriends(const FriendsRequestCallback &handler) {
    RETURN_IF_STATE_SET(FacebookFetchingFriendsBit, "Facebook::fetchFriends - another request is in progress");
    
    FB_SET_FETCHING_STATE(FacebookFetchingFriendsBit);
    Request::requestForFriends([=](int error, const Vector<GraphUser *> &friends){
        if (!error) {
            this->didFetchFriends(friends);
        }
        if (handler) {
            handler(error, friends);
        }
        FB_CLEAR_FETCHING_STATE(FacebookFetchingFriendsBit);
        //Fire notification
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(FacebookFriendsDidFetchNotification);
    })->execute();
}

void Facebook::fetchScores(const ScoresRequestCallback &handler){
    RETURN_IF_STATE_SET(FacebookFetchingHighScoresBit, "Facebook::fetchScores - another request is in progress");
    
    FB_SET_FETCHING_STATE(FacebookFetchingHighScoresBit);
    Request::requestForScores([=](int error, const Vector<GraphScore *> &scores){
        if (!error) {
            this->didFetchScores(scores);
        }
        if (handler) {
            handler(error, scores);
        }
        FB_CLEAR_FETCHING_STATE(FacebookFetchingHighScoresBit);
        //Fire notification
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(FacebookScoresDidFetchNotification);
    })->execute();
}

#pragma mark Score
void Facebook::postScore(long score) {
    this->setDirtyScore(score);
    Request::requestForMyScore([=](int error, const Vector<GraphScore *> &scores){
        if (!error) {
            if (scores.size()) {
                if (scores.at(0)->getScore() < score) {
                    this->doPostScore(score);
                } else {
                    this->clearDirtyScore();
                }
            } else {
                //No score
                this->doPostScore(score);
            }
        }
    })->execute();
}

void Facebook::doPostScore(long score) {
    Request::requestForPostScore(score, [=](int error, bool success){
        if (success) {
            this->clearDirtyScore();
        }
    })->execute();
}

void Facebook::setDirtyScore(long score) {
    _data->set(FacebookDataDirtyScoreKey, score);
}

long Facebook::getDirtyScore() {
    return _data->getLong(FacebookDataDirtyScoreKey);
}

void Facebook::clearDirtyScore() {
    _data->clear(FacebookDataDirtyScoreKey);
}

#pragma mark Status Callback
void Facebook::sessionStatusCallback(Session *session, SessionError *error) {
    if (session->isOpened()) {
        //Check if user detail has fetched
        string uid = _data->getString(FacebookDataUserIDKey);
        if (uid.length()) {
            _state = State::LOGIN_WITH_ID;
        } else {
            _state = State::LOGIN_NO_ID;
            //Try to fecth user detail
            Request::requestForMe([=](int error, GraphUser *user){
                if (!error && user) {
                    this->didFetchUserDetail(user);
                }
            })->execute();
        }
    } else {
        _state = State::NOT_LOGIN;
    }
    
    if (session->isClosed()) {
        this->purgeData();
    }
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(FacebookLoginStatusChangedNotification);
}

#pragma mark Private Save
void Facebook::saveUserDetail(GraphUser *user) {
    //Get current score
    Value data = user->getValue();
    long score = _data->getLong(FACEBOOK_PROFILE_KEY(user->getId()) + "/" + GraphUser::SCORE);
    if (score != 0) {
        ValueSetter::set(data, GraphUser::SCORE, score);
    }
    ValueSetter::set(data, GraphUser::INSTALLED, true);
    _data->set(FACEBOOK_PROFILE_KEY(user->getId()), data);
    //Record the user id
    _data->set(FacebookDataUserIDKey, user->getId());
    
    _data->save();
}

void Facebook::saveFriend(GraphUser *user) {
    Value data = user->getValue();
    long score = _data->getLong(FACEBOOK_PROFILE_KEY(user->getId()) + "/" + GraphUser::SCORE);
    ValueSetter::set(data, GraphUser::SCORE, score);
    _data->set(FACEBOOK_PROFILE_KEY(user->getId()), data);
}

void Facebook::didFetchUserDetail(GraphUser *user) {
    this->saveUserDetail(user);
    _state = State::LOGIN_WITH_ID;
}

void Facebook::didFetchFriends(const Vector<GraphUser *> &friends) {
    for (auto f : friends) {
#if FACEBOOK_KEEP_ALL_FRIENDS
        this->saveFriend(f);
#else
        if (f->isInstalled()) {
            this->saveFriend(f);
        }
#endif
    }
    _data->save();
}

void Facebook::didFetchScores(const Vector<GraphScore *> &scores) {
    for (auto s : scores) {
        GraphUser *user = s->getUser();
        Value &data = _data->get(FACEBOOK_PROFILE_KEY(user->getId()));
        if (!data.isNull()) {
            //User exists, update score only
            ValueSetter::set(data, GraphUser::SCORE, s->getScore());
            if (getUser()->getId() == user->getId()) {
                
            }
        } else {
            Value &newData = user->getValue();
            ValueSetter::set(newData, GraphUser::INSTALLED, true);
            ValueSetter::set(newData, GraphUser::SCORE, s->getScore());
            _data->set(FACEBOOK_PROFILE_KEY(user->getId()), newData);
        }
    }
    
    _data->save();
}

void Facebook::purgeData() {
    _data->clear(FacebookDataProfilesKey);
    _data->clear(FacebookDataUserIDKey);
    _data->save();
}

NS_SCREW_FACEBOOK_END