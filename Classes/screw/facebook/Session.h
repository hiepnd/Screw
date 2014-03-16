/*
 * Session.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef SESSION_H_
#define SESSION_H_
#include "../macros.h"
#include "GraphObject.h"
#include "../data/Data.h"
#include "cocos2d.h"

USING_NS_CC;
USING_NS_SCREW_DATA;
using namespace std;

NS_SCREW_FACEBOOK_BEGIN

class Session;

typedef std::function<void(Session *)> SessionStatusCallback;

class SessionImpl {
public:
    virtual ~SessionImpl(){};
    
    virtual void open() = 0;
    virtual void close() = 0;
};

class Session {
public:
	enum State{INVALID = -1, CREATED, CREATED_TOKEN_LOADED, OPENING, OPENED, OPENED_TOKEN_UPDATED, CLOSED_LOGIN_FAILED, CLOSED};

	static Session *getActiveSession();

    void setStatusCallback(const SessionStatusCallback &callback);
    State getState();
    string &getAppId();
    bool isSessionOpened();

    void open();
    void close();

    /* Call only by friends */
	static void initActiveSession(State state, const string &appid);
	void updateState(State state);

private:
	Session();
	virtual ~Session();
	void init(State state, const string &appId);

	static Session  *_activeSession;
	SessionStatusCallback _callback;
	SessionImpl     *_impl;
    State   _state;
	string  _appId;
	bool    _initialized;
    
};

NS_SCREW_FACEBOOK_END

#endif /* SESSION_H_ */
