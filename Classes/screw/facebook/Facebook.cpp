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

//Data key
static const char *FacebookDataProfilesKey              = "__profiles__";
static const char *FacebookDataProfilesTimestampKey     = "__profiles_timestamp__";
static const char *FacebookDataPhotosTimestampKey       = "__photos_timestamp__";
static const char *FacebookDataRequestTimestampKey      = "__request_timestamp__";
static const char *FacebookDataUserIDKey                = "__user_id__";
static const char *FacebookDataLocalUserKey             = "__local_user__";
static const char *FacebookDataFacebookScoreKey         = "__fb_score__";
static const char *FacebookDataDirtyScoreKey            = "__dirty_score__";
static const char *FacebookDataAllTimeHightScoreKey     = "__all_time_hight_score__";

#define FACEBOOK_PROFILE_KEY(uid)           PathBuilder::create(FacebookDataProfilesKey)->append(uid)->build()
#define FACEBOOK_PROFILE_TIMESTAMP_KEY(uid) PathBuilder::create(FacebookDataProfilesTimestampKey)->append(uid)->build()
#define FACEBOOK_PHOTO_TIMESTAMP_KEY(uid)   PathBuilder::create(FacebookDataPhotosTimestampKey)->append(uid)->build()
#define FACEBOOK_REQUEST_TIMESTAMP_KEY(uid) PathBuilder::create(FacebookDataRequestTimestampKey)->append(uid)->build()

#define RETURN_IF_STATE_SET(bit, msg) if (_fetchingBits & (bit)) {CCLOG("%s", msg); return;}

#define FB_SET_FETCHING_STATE(bit)      _fetchingBits |= bit;
#define FB_CLEAR_FETCHING_STATE(bit)    _fetchingBits &= ~bit;

Facebook::Facebook() {
	// TODO Auto-generated constructor stub
    _data = new data::Data(ValueMap(), utils::FileUtils::getDocumentPath("facebook.plist"));
    _fetchingBits = 0x00;
}

Facebook::~Facebook() {
	// TODO Auto-generated destructor stub
    delete _data;
}


#pragma mark Fetch
void Facebook::fetchUserDetails(const MeRequestCallback &handler) {
    RETURN_IF_STATE_SET(FacebookFetchingUserDetailBit, "Facebook::fetchUserDetails - another request is in progress");
    
    FB_SET_FETCHING_STATE(FacebookFetchingUserDetailBit);
    Request::requestForMe([=](int error, GraphUser *user){
        if (!error && user) {
            this->didFetchUserDetail(user);
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
    })->execute();
}

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

#pragma mark Score
void Facebook::setDirtyScore(long score) {
    _data->set(FacebookDataDirtyScoreKey, score);
}

long Facebook::getDirtyScore() {
    return _data->getLong(FacebookDataDirtyScoreKey);
}

void Facebook::clearDirtyScore() {
    _data->clear(FacebookDataDirtyScoreKey);
}

#pragma mark Private Save
void Facebook::saveUserDetail(GraphUser *user) {
    //Get current score
    Value data = user->getValue();
    long score = _data->getLong(PathBuilder::create(FacebookDataProfilesKey)->append(user->getId())
                                                                    ->append(GraphUser::SCORE)->build());
    if (score != 0) {
        ValueSetter::set(data, GraphUser::SCORE, score);
    }
    ValueSetter::set(data, GraphUser::INSTALLED, true);
    _data->set(FACEBOOK_PROFILE_KEY(user->getId()), data);
    _data->set(FacebookDataUserIDKey, user->getId());
}

void Facebook::saveFriend(GraphUser *user) {
    Value data = user->getValue();
    long score = _data->getLong(PathBuilder::create(FacebookDataProfilesKey)->append(user->getId())
                                                                    ->append(GraphUser::SCORE)->build());
    ValueSetter::set(data, GraphUser::SCORE, score);
    _data->set(FACEBOOK_PROFILE_KEY(user->getId()), data);
}

void Facebook::didFetchUserDetail(GraphUser *user) {
    
    _state = State::LOGIN_WITH_ID;
}

void Facebook::didFetchFriends(const Vector<GraphUser *> &friends) {
    for (auto f : friends) {
        this->saveFriend(f);
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
//            if (getUser()->getId() == user->getId()) {
//                
//            }
        } else {
            Value &newData = user->getValue();
            ValueSetter::set(newData, GraphUser::INSTALLED, true);
            ValueSetter::set(newData, GraphUser::SCORE, s->getScore());
            _data->set(FACEBOOK_PROFILE_KEY(user->getId()), newData);
        }
    }
}

void Facebook::didAuthorizeNewUser(GraphUser *user) {
    
}


NS_SCREW_FACEBOOK_END