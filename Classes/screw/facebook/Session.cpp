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

void Session::init(State state, const string &appId) {
	CCASSERT(!_initialized, "Must be initialized only once");
	CCASSERT(appId != "", "Application ID must not be empty");
    CCLOG("Session::init - state = %d, appid = %s", state, appId.c_str());
	_initialized = true;
	_state = state;
	_appId = appId;
}

Session *Session::getActiveSession() {
	if (!_activeSession) {
		_activeSession = new Session();
	}

	return _activeSession;
}
void Session::initActiveSession(State state, const string &appid) {
    CCASSERT(VALIDATE_STATE(state), "Invalid state");
	getActiveSession()->init(state, appid);
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

Session::State Session::getState() {
	return _state;
}

string &Session::getAppId() {
	return _appId;
}

bool Session::isOpened() {
	return _state == State::OPENED || _state == State::OPENED_TOKEN_UPDATED;
}

void Session::updateState(Session::State state) {
	CCLOG("Session::updateState - state = %d", state);
    CCASSERT(VALIDATE_STATE(state), "Invalid state");
	_state = state;
	if (_callback)
		_callback(this);
}

NS_SCREW_FACEBOOK_END
