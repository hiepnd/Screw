/*
 * Request.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "Request.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/screw/facebook/RequestAndroid.h"
#endif

NS_SCREW_FACEBOOK_BEGIN

Vector<Request *> Request::_requests;

Request *Request::create(const string &graphPath, const Value &params, Method method, const RequestCallback &callback) {
    Request *request = new Request(graphPath, params, method, callback);
    request->autorelease();
    return request;
}

Request::Request():
_graphPath(""), _params(Value(ValueMap())), _method(Method::GET), _callback(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _impl = new jni::RequestAndroid();
#endif
    
}

Request::Request(const string &graphPath, const Value &params, Method method, const RequestCallback &callback):
Request()
{
    _graphPath = graphPath;
    _params = params;
    _method = method;
    _callback = callback;
}

Request::~Request() {
    delete _impl;
}

const string &Request::getGraphPath() {
    return _graphPath;
}
const Value &Request::getParams() {
    return _params;
}

Request::Method Request::getMethod() {
    return _method;
}

const RequestCallback &Request::getCallback() {
    return _callback;
}

void Request::setGraphPath(const string &graphPath) {
    _graphPath = graphPath;
}

void Request::setParams(const Value &params) {
    _params = params;
}

void Request::setMethod(Method method) {
    _method = method;
}

void Request::setCallback(const RequestCallback &callback) {
    _callback = callback;
}

void Request::execute() {
	FB_LOG("Request::execute - executing ...");
    CCASSERT(!_requests.contains(this), "Fix me");
    _requests.pushBack(this);
    RequestCallback callback = this->getCallback();
    this->setCallback([=](Object *result){
        if (callback) {
            callback(result);
        }
        _requests.eraseObject(this);
    });
    
    _impl->execute(this);
}

NS_SCREW_FACEBOOK_END
