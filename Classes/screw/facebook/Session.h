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
#include "GraphObject.h"
#include "cocos2d.h"

USING_NS_CC;
USING_NS_SCREW_DATA;
using namespace std;

NS_SCREW_FACEBOOK_BEGIN

class Session;
class SessionError;
typedef std::function<void(Session *session, SessionError *error)> SessionStatusCallback;

class SessionError : public GraphObject {
public:
    GO_CREATE_EMPTY(SessionError);
    
    GO_PROPERTY_INT(Code, "code");
//    GO_PROPERTY_INT(SubCode, "subCode");
    GO_PROPERTY_INT(Category, "category");
    GO_PROPERTY_STRING(UserMessage, "message");
    GO_PROPERTY_BOOL(ShouldNotifyUser, "shouldNotifyUser");
};

enum LoginBehavior {
    WITH_FALLBACK_TO_WEBVIEW,
    WITH_NO_FALLBACK_TO_WEBVIEW,
    FORCE_WEBVIEW,
    SYSTEM_IF_PRESENT
};

enum DefaultAudience {
    NONE,
    ONLY_ME,
    FRIENDS,
    PUBLIC
};

class SessionImpl {
public:
    virtual ~SessionImpl(){};
    
    virtual void open(bool allowUi, const list<string> &permissions, DefaultAudience defaultAudience, LoginBehavior loginBehavior) = 0;
    virtual void close() = 0;
    virtual void requestReadPermissions(const list<string> &permissions) = 0;
    virtual void requestPublishPermissions(const list<string> &permissions) = 0;
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
    
    static void start();
	static Session *getActiveSession();

    void setStatusCallback(const SessionStatusCallback &callback);
    State getState();
    const list<string> &getPermissions();
    const string &getAppId();
    bool isOpened();
    bool isClosed();

    /* Platform dependent implementation */
    
    /**
     * Open Facebook Session
     * @allowUi if false then no UI will be presented
     * @permissions: 
     */
    void open(bool allowUi, const list<string> &permissions = {},
              DefaultAudience defaultAudience = DefaultAudience::PUBLIC,
              LoginBehavior loginBehavior = LoginBehavior::SYSTEM_IF_PRESENT);
    void close();
    void requestReadPermissions(const list<string> &permissions);
    void requestPublishPermissions(const list<string> &permissions);
    
    /* Called soly by iOS/Android implementations */
	static void initActiveSession(State state, const string &appid, const list<string> &permissions);
	void updateState(State state, const list<string> &permissions, SessionError *error = nullptr);

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
