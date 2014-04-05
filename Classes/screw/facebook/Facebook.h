/*
 * Facebook.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef FACEBOOK_H_
#define FACEBOOK_H_

#include "../macros.h"
#include "../data/Data.h"
#include "cocos2d.h"
#include "GraphObject.h"
#include "Session.h"
#include "Request.h"
#include "Dialog.h"

USING_NS_CC;
USING_NS_SCREW_DATA;
using namespace std;

NS_SCREW_FACEBOOK_BEGIN

enum {
    FacebookFetchingUserDetail  = 1 << 0,
    FacebookFetchingFriends     = 1 << 1,
    FacebookFetchingHighScores  = 1 << 2,
    FacebookFetchingX           = 1 << 3,
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
    int _loadingBits;
    
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

#endif /* FACEBOOK_H_ */
