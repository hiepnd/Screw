/*
 * Request.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "Request.h"
#include "Session.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/screw/facebook/RequestAndroid.h"
#endif

NS_SCREW_FACEBOOK_BEGIN

Vector<Request *> Request::_requests;

Request *Request::create(const string &graphPath, const ValueMap &params, Method method, const RequestCallback &callback) {
    Request *request = new Request(graphPath, params, method, callback);
    request->autorelease();
    return request;
}

Request::Request():
_graphPath(""), _params(ValueMap()), _method(Method::GET), _callback(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _impl = new jni::RequestAndroid();
#endif
    
}

Request::Request(const string &graphPath):Request() {
    _graphPath = graphPath;
}

Request::Request(const string &graphPath, const ValueMap &params, Method method, const RequestCallback &callback):
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

ValueMap &Request::getParams() {
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

void Request::setParams(const ValueMap &params) {
    _params = params;
}

void Request::setMethod(Method method) {
    _method = method;
}

void Request::setCallback(const RequestCallback &callback) {
    _callback = callback;
}

void Request::execute() {
	FB_LOG("Request::execute - executing %ld request(s)", _requests.size() + 1);
    CCASSERT(!_requests.contains(this), "Fix me");
    _requests.pushBack(this);
    RequestCallback callback = this->getCallback();
    this->setCallback([=](int error, GraphObject *result){
        if (callback) {
            callback(error, result);
        }
        _requests.eraseObject(this);
    });
    
    _impl->execute(this);
}

#pragma mark Common Requests
Request *requestForMe(const MeRequestCallback &callback) {
    Request *request = new Request("me");
    request->autorelease();
    if (callback) {
        RequestCallback wrapper = [=](int error, GraphObject *result){
            callback(error, error == 0 ? GraphObject::castTo<GraphObject, GraphUser>(result) : nullptr);
        };
        
        request->setCallback(wrapper);
    }
    return request;
}

Request *Request::requestForFriends(const FriendsRequestCallback &callback) {
    Request *request = new Request("me/friends");
    ValueMap params;
    params["fields"] = "id,name,username,installed,first_name,last_name";
    request->setParams(params);
    if (callback) {
        RequestCallback wrapper = [=](int error, GraphObject *result){
            Vector<GraphUser *> friends;
            if (error == 0) {
                friends = result->getPropertyAsList<GraphUser>("data");
            }
            callback(error, friends);
        };
        request->setCallback(wrapper);
    }
    request->autorelease();
    return request;
}

Request *Request::requestForDelete(const string &objectId, const DeleteRequestCallback &callback) {
    Request *request = new Request(objectId);
    request->setMethod(screw::facebook::Request::Method::DELETE);
    if (callback) {
        
    }
    request->autorelease();
    return request;
}

Request *Request::requestForScores(const ScoresRequestCallback &callback) {
    string appId = Session::getActiveSession()->getAppId();
    CCASSERT(appId.length() > 0, "Request::requestForScores - check your code boss");
    Request *request = new Request(appId + "/scores");
    if (callback) {
        RequestCallback wrapper = [=](int error, GraphObject *result){
            Vector<GraphScore *> scores;
            if (error == 0) {
                scores = result->getPropertyAsList<GraphScore>("data");
            }
            callback(error, scores);
        };
        request->setCallback(wrapper);
    }
    request->autorelease();
    return request;
}

Request *Request::requestForAppRequests(const ApprequestsRequestCallback &callback) {
    Request *request = new Request("me/apprequests");
    if (callback) {
        RequestCallback wrapper = [=](int error, GraphObject *result){
            Vector<GraphRequest *> requests;
            if (error == 0) {
                requests = result->getPropertyAsList<GraphRequest>("data");
            }
            callback(error, requests);
        };
        request->setCallback(wrapper);
    }
    request->autorelease();
    return request;
}

NS_SCREW_FACEBOOK_END
