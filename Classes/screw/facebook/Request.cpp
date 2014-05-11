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

#include "Request.h"
#include "Session.h"
#include "../utils/StringUtils.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "RequestApple.h"
#endif

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

Request *Request::create(const string &graphPath) {
    Request *request = new Request(graphPath);
    request->autorelease();
    return request;
}

Request::Request():
_graphPath(""), _params(ValueMap()), _method(Method::GET), _callback(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _impl = new screw::ios::RequestApple();
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _impl = new screw::jni::RequestAndroid();
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
Request *Request::requestForMe(const MeRequestCallback &callback) {
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
    params["fields"] = "id,name,installed,first_name,last_name";
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
    request->setMethod(Method::DELETE);
    if (callback) {
        RequestCallback wrapper = [=](int error, GraphObject *result){
            callback(error, error == 0);
        };
        request->setCallback(wrapper);
    }
    request->autorelease();
    return request;
}

Request *Request::requestForScores(const ScoresRequestCallback &callback) {
    string appId = Session::getActiveSession()->getAppId();
    CCASSERT(appId.length() > 0, "Request::requestForScores - check your code sir");
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

Request *Request::requestForMyScore(const ScoresRequestCallback &callback) {
    Request *request = new Request("me/score");
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
    /* Work on iOS but not on Android
     Request *request = new Request("me/apprequests?fields=action_type,application,from,to,message,data,object.id,object.title,object.type");
     */
    Request *request = new Request("me/apprequests");
    ValueMap params;
    params["fields"] = "action_type,application,from,to,message,data,object.id,object.title,object.type";
    request->setParams(params);
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

Request *Request::requestForPostScore(long score, const PostScoreRequestCallback &callback) {
    Request *request = new Request("me/scores");
    request->setMethod(Method::POST);
    if (callback) {
        RequestCallback wrapper = [=](int error, GraphObject *result){
            callback(error, !error);
        };
        request->setCallback(wrapper);
    }
    ValueMap params;
    params["score"] = utils::StringUtils::toString(score);
    request->setParams(params);
    
    return request;
}

NS_SCREW_FACEBOOK_END
