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

#ifndef _SCREW_FACEBOOK_H_
#define _SCREW_FACEBOOK_H_

#include "../macros.h"
#include "../data/Data.h"
#include "cocos2d.h"
#include "GraphObject.h"
#include "Session.h"
#include "Request.h"
#include "WebDialog.h"

USING_NS_CC;
USING_NS_SCREW_DATA;
using namespace std;

NS_SCREW_FACEBOOK_BEGIN

enum {
    FacebookFetchingUserDetailBit  = 1 << 0,
    FacebookFetchingFriendsBit     = 1 << 1,
    FacebookFetchingHighScoresBit  = 1 << 2,
    FacebookFetchingXBit           = 1 << 3,
};

class Facebook {
public:
	enum class State {
        INVALID,
        NOT_LOGIN,
        LOGIN_NO_ID,
        LOGIN_WITH_ID
    };

	static Facebook *getInstance();

	State getState();
    int getFetchingBits();
    
#pragma mark Get
    GraphUser *getUser();
    Vector<GraphUser *> getFriends();
    GraphUser *getFriend(const string &uid);
    
#pragma mark Fetch
    void fetchUserDetails(const MeRequestCallback &handler);
    void fetchFriends(const FriendsRequestCallback &handler);
    void fetchScores(const ScoresRequestCallback &handler);
    
    void postScore(long score);
private:
    Facebook();
	virtual ~Facebook();
    
	State _state;
	screw::data::Data *_data;
    int _fetchingBits;
    
#pragma mark Private Save
    void saveUserDetail(GraphUser *user);
    void saveFriend(GraphUser *user);
    
    void didFetchUserDetail(GraphUser *user);
    void didFetchFriends(const Vector<GraphUser *> &friends);
    void didFetchScores(const Vector<GraphScore *> &scores);
    
    void didAuthorizeNewUser(GraphUser *user);
    
#pragma mark Score
    void setDirtyScore(long score);
    long getDirtyScore();
    void clearDirtyScore();
    void doPostScore(long score);
};

NS_SCREW_FACEBOOK_END

#endif /* _SCREW_FACEBOOK_H_ */
