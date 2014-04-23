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

#include "Session.h"
#include "../utils/StringUtils.h"
#include <map>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "SessionApple.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/screw/facebook/SessionAndroid.h"
#endif


NS_SCREW_FACEBOOK_BEGIN

#define VALIDATE_STATE(state)   (state == CREATED || state == CREATED_TOKEN_LOADED \
                                || state == OPENING || state == OPENED || state == OPENED_TOKEN_UPDATED \
                                || state == CLOSED_LOGIN_FAILED || state == CLOSED)

static map<int, const char *> __stateString { {Session::State::CREATED, "CREATED"},
                                                    {Session::State::CREATED_TOKEN_LOADED, "CREATED_TOKEN_LOADED"},
                                                    {Session::State::OPENING, "OPENING"},
                                                    {Session::State::OPENED, "OPENED"},
                                                    {Session::State::OPENED_TOKEN_UPDATED, "OPENED_TOKEN_UPDATED"},
                                                    {Session::State::CLOSED_LOGIN_FAILED, "CLOSED_LOGIN_FAILED"},
                                                    {Session::State::CLOSED, "CLOSED"}
                                                  };

Session *Session::_activeSession = nullptr;

Session::Session():_state(INVALID), _appId(""), _initialized(false) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _impl = new screw::jni::SessionAndroid();
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _impl = new screw::ios::SessionApple();
#endif
}

Session::~Session() {
    delete _impl;
}


void Session::start() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	screw::jni::SessionAndroid::start();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	screw::ios::SessionApple::start();
#endif
}

void Session::init(State state, const string &appId, list<string> permissions) {
	CCASSERT(!_initialized, "Must be initialized only once");
	CCASSERT(appId != "", "Application ID must not be empty");
    FB_LOG("Session::init - state = %s, appid = %s", __stateString[state], appId.c_str());
    FB_LOG("Session::init - permissions = [%s]", utils::StringUtils::join(permissions, ",").c_str());

	_initialized = true;
	_state = state;
	_appId = appId;
	_permissions = permissions;
}

Session *Session::getActiveSession() {
    CCASSERT(_activeSession, "Try to get activeSession before it is initialized ?");
	return _activeSession;
}
void Session::initActiveSession(State state, const string &appid, const list<string> &permissions) {
    CCASSERT(VALIDATE_STATE(state), "Invalid state");
    CCASSERT(!_activeSession, "It must be null");
    _activeSession = new Session();
	_activeSession->init(state, appid, permissions);
}

void Session::setStatusCallback(const SessionStatusCallback &callback) {
	_callback = callback;
}

void Session::open(bool allowUi, const list<string> &permissions, DefaultAudience defaultAudience, LoginBehavior loginBehavior) {
    _impl->open(allowUi, permissions, defaultAudience, loginBehavior);
}

void Session::close() {
    _impl->close();
}

void Session::requestReadPermissions(const list<string> &permissions) {
    _impl->requestReadPermissions(permissions);
}

void Session::requestPublishPermissions(const list<string> &permissions) {
    _impl->requestPublishPermissions(permissions);
}

Session::State Session::getState() {
	return _state;
}

const string &Session::getAppId() {
	return _appId;
}

bool Session::isOpened() {
	return _state == State::OPENED || _state == State::OPENED_TOKEN_UPDATED;
}

bool Session::isClosed() {
    return _state == State::CLOSED || _state == State::CLOSED_LOGIN_FAILED;
}

bool Session::hasPermission(const string &permission) {
    return std::find(_permissions.begin(), _permissions.end(), permission) != _permissions.end();
}

const list<string> &Session::getPermissions() {
    return _permissions;
}

void Session::updateState(Session::State state, const list<string> &permissions, SessionError *error) {
	CCASSERT(VALIDATE_STATE(state), "Invalid state");
    FB_LOG("Session::updateState - state = %s", __stateString[state]);
    FB_LOG("Session::init - permissions = (%s)", utils::StringUtils::join(permissions, ",").c_str());

	_state = state;
	_permissions = permissions;
	if (_callback)
		_callback(this, error);
}

NS_SCREW_FACEBOOK_END
