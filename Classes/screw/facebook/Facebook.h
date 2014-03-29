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

class Facebook {
public:
	enum class State{INVALID, NOT_LOGIN, LOGIN_NO_ID, LOGIN_WITH_ID};

	Facebook();
	virtual ~Facebook();

	static Facebook *getInstance();

	State getState();
	GraphUser *getUser();
	Vector<GraphUser *> getFriends();
    
    void fetchUserDetails(std::function<void(bool, GraphUser *)> handler);
    

private:
	State _state;
	screw::data::Data *_data;
};

NS_SCREW_FACEBOOK_END

#endif /* FACEBOOK_H_ */
