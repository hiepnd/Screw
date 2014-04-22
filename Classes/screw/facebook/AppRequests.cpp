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

USING_NS_SCREW_DATA;
USING_NS_SCREW_UTILS;

NS_SCREW_FACEBOOK_BEGIN

AppRequests *AppRequests::_instance = nullptr;

static const string AppRequestsRequestsKey = "__requests__";

const string AppRequestsParamTypeKey = "_t";

#pragma mark Builder
AppRequestParamsBuilder::AppRequestParamsBuilder(const string &message): _params(), _data() {
    _data[AppRequestsParamTypeKey] = 0;
    this->setMessage(message);
}

AppRequestParamsBuilder *AppRequestParamsBuilder::create(const string &message) {
    AppRequestParamsBuilder *arp = new AppRequestParamsBuilder(message);
    arp->autorelease();
    return arp;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setMessage(const string &message) {
    _params["message"] = message;
    return this;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setTitle(const string &title) {
    _params["title"] = title;
    return this;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setTo(const string &uid) {
    _params["to"] = uid;
    return this;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setTo(vector<string> &uids) {
    _params["to"] = utils::StringUtils::join(uids, ",");
    return this;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setType(int type) {
    _data[AppRequestsParamTypeKey] = type;
    return this;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setData(const string &key, const string &value) {
    _data[key] = value;
    return this;
}

ValueMap &AppRequestParamsBuilder::build() {
    _params["data"] = JsonUtils::toJsonString(_data);
    return _params;
}

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
    ValueMap &raw = _data->get(AppRequestsRequestsKey).asValueMap();
    Vector<GraphRequest *> ret;
    for (auto i : raw) {
        GraphRequest *r = GraphRequest::create(i.second);
        ret.pushBack(r);
    }
    return ret;
}

Vector<GraphRequest *> AppRequests::getRequests(int type) {
    Vector<GraphRequest *> all = getRequests();
    Vector<GraphRequest *> ret;
    for (GraphRequest *r : all) {
        GraphObject *data = r->getDataObject();
        if (data && data->getInt(AppRequestsParamTypeKey) == type) {
            ret.pushBack(r);
        }
    }
    return ret;
}

GraphRequest *AppRequests::getRequest(const string &rid) {
    Value &data = _data->get(PathBuilder::create(AppRequestsRequestsKey)->append(rid)->build());
    if (!data.isNull()) {
        return GraphRequest::create(data);
    }
    
    return nullptr;
}

void AppRequests::clearRequest(GraphRequest *request) {
    this->clearRequest(request->getId());
}

void AppRequests::clearRequest(const string &rid) {
    _data->clear(PathBuilder::create(AppRequestsRequestsKey)->append(rid)->build());
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
    });
    
    request->execute();
}

void AppRequests::sendAppRequest(const ValueMap &params, const WebDialogCallback &callback) {
    WebDialog *dialog = WebDialog::create("apprequests", params, callback);
    dialog->show();
}

void AppRequests::didFetchAppRequests(const Vector<screw::facebook::GraphRequest *> &requests) {
    for (GraphRequest *request : requests) {
        string dataStr = request->getDataString();
        Value &v = request->getValue();
        CCLOG("AppRequests::didFetchAppRequests - data str = %s", dataStr.c_str());
        if (dataStr.length()) {
            ValueMap m;
            if (JsonUtils::parse(dataStr, m)) {
                if (m.find(AppRequestsParamTypeKey) == m.end()) {
                    CCLOG("AppRequests::didFetchAppRequests - request data with no type (be aware) %s", v.getDescription().c_str());
                }
                ValueSetter::set(v, "data", Value(m));
                CCLOG("AppRequests::didFetchAppRequests - parsed data = %s", Value(m).getDescription().c_str());
            } else {
                CCLOG("AppRequests::didFetchAppRequests - non JSON request data (cleared) %s", v.getDescription().c_str());
                ValueSetter::clear(v, "data");
            }
        }
        _data->set(PathBuilder::create(AppRequestsRequestsKey)->append(request->getId())->build(), v);
        
        //Clear luon !!
        Request *deleteRequest = Request::requestForDelete(request->getId(), nullptr);
        deleteRequest->execute();
    }
    _data->save();
}

void AppRequests::purgeData() {
    _data->clear(AppRequestsRequestsKey);
    _data->save();
}

NS_SCREW_FACEBOOK_END