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

#include "AppRequests.h"
#include "../utils/StringUtils.h"
#include "../utils/ValueUtils.h"
#include "../utils/JsonUtils.h"
#include "../utils/FileUtils.h"
#include "GraphObject.h"

USING_NS_SCREW_DATA;
USING_NS_SCREW_UTILS;

NS_SCREW_FACEBOOK_BEGIN

AppRequests *AppRequests::_instance = nullptr;

static const string AppRequestsRequestsKey = "__requests__";

const string FacebookRequestsDidFetchNotification = "FacebookRequestsDidFetchNotification";

#pragma mark AppRequests
AppRequests *AppRequests::getInstance() {
    if (!_instance) {
        _instance = new AppRequests();
    }
    
    return _instance;
}

AppRequests::AppRequests() {
    _data = new data::Data(ValueMap(), utils::FileUtils::getDocumentPath("apprequests.plist"));
}

AppRequests::~AppRequests() {
    delete _data;
}

Vector<GraphRequest *> AppRequests::getRequests() {
    Vector<GraphRequest *> ret;
    if (!_data->get(AppRequestsRequestsKey).isNull()) {
        ValueMap &raw = _data->get(AppRequestsRequestsKey).asValueMap();
        for (auto i : raw) {
            GraphRequest *r = GraphRequest::create(i.second);
            ret.pushBack(r);
        }
    }
    
    return ret;
}

Vector<GraphRequest *> AppRequests::getRequests(int type) {
    Vector<GraphRequest *> all = getRequests();
    Vector<GraphRequest *> ret;
    for (GraphRequest *r : all) {
        GraphObject *data = r->getDataObject();
        if (data && data->getInt(AppRequestsDataTypeKey) == type) {
            ret.pushBack(r);
        }
    }
    return ret;
}

GraphRequest *AppRequests::getRequest(const string &rid) {
    Value &data = _data->get(AppRequestsRequestsKey + "/" + rid);
    if (!data.isNull()) {
        return GraphRequest::create(data);
    }
    
    return nullptr;
}

void AppRequests::clearRequest(GraphRequest *request) {
    this->clearRequest(request->getId());
}

void AppRequests::clearRequest(const string &rid) {
    _data->clear(AppRequestsRequestsKey + "/" + rid);
    _data->save();
}

void AppRequests::fetchAppRequests(const ApprequestsRequestCallback &callback) {
    Request *request = Request::requestForAppRequests([=](int error, const Vector<GraphRequest *> &requests){
        FB_LOG("AppRequests::fetchAppRequests - callback error = %d", error);
        if (error == 0) {
            this->didFetchAppRequests(requests);
        }
        if (callback) {
            callback(error, requests);
        }
        //Fire notification
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(FacebookRequestsDidFetchNotification);
    });
    
    request->execute();
}

void AppRequests::didFetchAppRequests(const Vector<screw::facebook::GraphRequest *> &requests) {
    for (GraphRequest *request : requests) {
        string dataStr = request->getDataString();
        Value &v = request->getValue();
        FB_LOG_INFO("AppRequests::didFetchAppRequests - data str = %s", dataStr.c_str());
        if (dataStr.length()) {
            ValueMap m;
            if (JsonUtils::parse(dataStr, m)) {
                if (m.find(AppRequestsDataTypeKey) == m.end()) {
                    FB_LOG("AppRequests::didFetchAppRequests - request data with no type (be aware) %s", v.getDescription().c_str());
                }
                ValueSetter::set(v, "data", Value(m));
                FB_LOG("AppRequests::didFetchAppRequests - parsed data = %s", Value(m).getDescription().c_str());
            } else {
                FB_LOG("AppRequests::didFetchAppRequests - non JSON request data (cleared) %s", v.getDescription().c_str());
                ValueSetter::clear(v, "data");
            }
        }
        _data->set(AppRequestsRequestsKey + "/" + request->getId(), v);
        
        //Delete request
        Request::requestForDelete(request->getId(), nullptr)->execute();
    }
    
    if (requests.size()) {
        _data->save();
    }
}

void AppRequests::purgeData() {
    _data->clear(AppRequestsRequestsKey);
    _data->save();
}

NS_SCREW_FACEBOOK_END