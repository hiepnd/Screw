//
//  AppRequests.cpp
//  Screw
//
//  Created by Ngo Duc Hiep on 3/29/14.
//
//

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
AppRequestParamsBuilder::AppRequestParamsBuilder(const string &message): _data() {
    ValueMap extra;
    extra[AppRequestsParamTypeKey] = 0;
    _data["data"] = extra;
    
    this->setMessage(message);
}

AppRequestParamsBuilder *AppRequestParamsBuilder::create(const string &message) {
    AppRequestParamsBuilder *arp = new AppRequestParamsBuilder(message);
    arp->autorelease();
    return arp;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setMessage(const string &message) {
    _data["message"] = message;
    return this;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setTitle(const string &title) {
    _data["title"] = title;
    return this;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setTo(const string &uid) {
    _data["to"] = uid;
    return this;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setTo(vector<string> &uids) {
    _data["to"] = utils::StringUtils::join(uids, ",");
    return this;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setType(int type) {
    ValueMap &d = _data["data"].asValueMap();
    d[AppRequestsParamTypeKey] = type;
    return this;
}

AppRequestParamsBuilder *AppRequestParamsBuilder::setAdditionalData(const string &key, const string &value) {
    ValueMap &d = _data["data"].asValueMap();
    d[key] = value;
    return this;
}

ValueMap &AppRequestParamsBuilder::build() {
    return _data;
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
        if (error == 0) {
            this->didFetchAppRequests(requests);
        }
        if (callback) {
            callback(error, requests);
        }
    });
    
    request->execute();
}

void AppRequests::sendAppRequest(const ValueMap &params, const DialogCallback &callback) {
    Dialog *dialog = Dialog::create("apprequests", params, callback);
    dialog->show();
}

void AppRequests::didFetchAppRequests(const Vector<screw::facebook::GraphRequest *> &requests) {
    for (GraphRequest *request : requests) {
        string dataStr = request->getDataString();
        Value &v = request->getData();
        if (dataStr.length()) {
            ValueMap m;
            if (JsonUtils::parse(dataStr, m)) {
                if (m.find(AppRequestsParamTypeKey) == m.end()) {
                    CCLOG("AppRequests::didFetchAppRequests - request data with no type (be aware) %s", v.getDescription().c_str());
                }
                ValueSetter::set(v, "data", Value(m));
            } else {
                CCLOG("AppRequests::didFetchAppRequests - non JSON request data (cleared) %s", v.getDescription().c_str());
                ValueSetter::clear(v, "data");
            }
        }
        _data->set(PathBuilder::create(AppRequestsRequestsKey)->append(request->getId())->build(), v);
        
        //Clear luon !!
        Request *deleteRequest = Request::requestForDelete(request->getId());
        deleteRequest->execute();
    }
    _data->save();
}

void AppRequests::purgeData() {
    _data->clear(AppRequestsRequestsKey);
    _data->save();
}

NS_SCREW_FACEBOOK_END