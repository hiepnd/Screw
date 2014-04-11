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

#ifndef SESSION_H_
#define SESSION_H_

#include "../macros.h"
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
        INVALID,
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
