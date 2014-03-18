/*
 * Request.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "Request.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "jni/screw/facebook/RequestAndroid.h"
#endif

NS_SCREW_FACEBOOK_BEGIN

Request *Request::create(const string &graphPath, const ValueMap &params, Method method, const RequestCallback &callback) {
    Request *request = new Request(graphPath, params, method, callback);
    request->autorelease();
    return request;
}

Request::Request(const string &graphPath, const ValueMap &params, Method method, const RequestCallback &callback):
_graphPath(graphPath), _params(params), _method(method), _callback(callback)
{
	// TODO Auto-generated constructor stub
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    _impl = new RequestAndroid();
#endif


}

Request::~Request() {
	// TODO Auto-generated destructor stub
    delete _impl;
}

void Request::setCallback(const RequestCallback &callback) {
    _callback = callback;
}

void Request::execute() {
    _impl->execute();
}

NS_SCREW_FACEBOOK_END
