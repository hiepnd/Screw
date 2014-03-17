/*
 * Session.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "Session.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/screw/facebook/SessionAndroid.h"
#endif


NS_SCREW_FACEBOOK_BEGIN

#define VALIDATE_STATE(state) (state == CREATED || state == CREATED_TOKEN_LOADED || state == OPENING || state == OPENED || state == OPENED_TOKEN_UPDATED || state == CLOSED_LOGIN_FAILED || state == CLOSED)

Session *Session::_activeSession = nullptr;

Session::Session():_state(INVALID), _appId(""), _initialized(false) {
	// TODO Auto-generated constructor stub
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _impl = new jni::SessionAndroid();
#endif

}

Session::~Session() {
	// TODO Auto-generated destructor stub
    delete _impl;
}

void Session::init(State state, const string &appId, list<string> permissions) {
	CCASSERT(!_initialized, "Must be initialized only once");
	CCASSERT(appId != "", "Application ID must not be empty");
    CCLOG("Session::init - state = %d, appid = %s", state, appId.c_str());
#ifdef COCOS2D_DEBUG
    string pstr;
    for (auto i = permissions.begin(); i != permissions.end(); i++) {
    	pstr += string(" ") + (*i);
    }
    CCLOG("Session::init - permissions = (%s)", pstr.c_str());
#endif

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

void Session::open() {
    _impl->open();
}

void Session::close() {
    _impl->close();
}

void Session::requestReadPermissions(const list<string> &permission) {
    _impl->requestReadPermissions(permission);
}

void Session::requestPublishPermissions(const list<string> &permission) {
    _impl->requestPublishPermissions(permission);
}


Session::State Session::getState() {
	return _state;
}

string &Session::getAppId() {
	return _appId;
}

bool Session::isOpened() {
	return _state == State::OPENED || _state == State::OPENED_TOKEN_UPDATED;
}

bool Session::isClosed() {
    return _state == State::CLOSED || _state == State::CLOSED_LOGIN_FAILED;
}

void Session::requestReadPermission(const string &permission) {
    list<string> l;
    l.push_back(permission);
    this->requestReadPermissions(l);
}

void Session::requestPublishPermission(const string &permission) {
    list<string> l;
    l.push_back(permission);
    this->requestPublishPermissions(l);
}

bool Session::hasPermission(const string &permission) {
    return std::find(_permissions.begin(), _permissions.end(), permission) != _permissions.end();
}

const list<string> &Session::getPermissions() {
    return _permissions;
}

void Session::updateState(Session::State state, const list<string> &permissions) {
	CCLOG("Session::updateState - state = %d", state);
    CCASSERT(VALIDATE_STATE(state), "Invalid state");
#ifdef COCOS2D_DEBUG
    string pstr;
    for (auto i = permissions.begin(); i != permissions.end(); i++) {
    	pstr += string(" ") + (*i);
    }
    CCLOG("Session::updateState - permissions = (%s)", pstr.c_str());
#endif
	_state = state;
	_permissions = permissions;
	if (_callback)
		_callback(this);
}

NS_SCREW_FACEBOOK_END
