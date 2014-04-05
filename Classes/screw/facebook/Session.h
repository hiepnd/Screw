/*
 * Session.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef SESSION_H_
#define SESSION_H_

#include "../macros.h"
//#include "GraphObject.h"
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
    
    virtual void open(bool allowUi , const list<string> &permission) = 0;
    virtual void close() = 0;
    virtual void requestReadPermissions(const list<string> &permission) = 0;
    virtual void requestPublishPermissions(const list<string> &permission) = 0;
};

class Session {
public:
	enum State {
        INVALID = -1,
        CREATED,
        CREATED_TOKEN_LOADED,
        OPENING,
        OPENED,
        OPENED_TOKEN_UPDATED,
        CLOSED_LOGIN_FAILED,
        CLOSED
    };

	static Session *getActiveSession();

    void setStatusCallback(const SessionStatusCallback &callback);
    State getState();
    const list<string> &getPermissions();
    const string &getAppId();
    bool isOpened();
    bool isClosed();

    static void start();

    /* Need platform dependent implementation */
    void open(bool allowUi = false, const list<string> &permission = list<string>());
    void close();
    void requestReadPermissions(const list<string> &permission);
    void requestPublishPermissions(const list<string> &permission);
    
    /* Called soly by iOS/Android implementations */
	static void initActiveSession(State state, const string &appid, const list<string> &permissions);
	void updateState(State state, const list<string> &permissions);

    void requestReadPermission(const string &permission);
    void requestPublishPermission(const string &permission);
    bool hasPermission(const string &permission);
    
private:
	Session();
	virtual ~Session();
	void init(State state, const string &appId, list<string> permissions);

	static Session  *_activeSession;
	SessionStatusCallback _callback;
	SessionImpl     *_impl;
	list<string>	_permissions;
    State   _state;
	string  _appId;
	bool    _initialized;
    
};

NS_SCREW_FACEBOOK_END

#endif /* SESSION_H_ */
