//
//  AppRequests.h
//  Screw
//
//  Created by Ngo Duc Hiep on 3/29/14.
//
//

#ifndef __Screw__AppRequests__
#define __Screw__AppRequests__

#include <iostream>
#include <vector>
#include "../macros.h"
#include "../data/Data.h"
#include "GraphObject.h"
#include "Dialog.h"
#include "Request.h"
#include "cocos2d.h"

USING_NS_CC;
USING_NS_SCREW;;

using namespace std;

NS_SCREW_FACEBOOK_BEGIN

extern const string AppRequestsParamTypeKey;

class AppRequestParamsBuilder : public Object {
public:
    AppRequestParamsBuilder(const string &message);
    static AppRequestParamsBuilder *create(const string &message);
    
    AppRequestParamsBuilder *setMessage(const string &message);
    AppRequestParamsBuilder *setTitle(const string &title);
    AppRequestParamsBuilder *setTo(const string &uid);
    AppRequestParamsBuilder *setTo(vector<string> &uids);
    AppRequestParamsBuilder *setType(int type);
    AppRequestParamsBuilder *setAdditionalData(const string &key, const string &value);
    
    ValueMap &build();
    
protected:
    ValueMap _params;
    ValueMap _data;
};

class AppRequests {
    
public:
    static AppRequests *getInstance();
    
    Vector<GraphRequest *> getRequests();
    Vector<GraphRequest *> getRequests(int type);
    GraphRequest *getRequest(const string &rid);
    
    void clearRequest(GraphRequest *request);
    void clearRequest(const string &rid);
    
    void fetchAppRequests(const ApprequestsRequestCallback &callback = nullptr);
    void sendAppRequest(const ValueMap &params, const DialogCallback &callback = nullptr);
    
    void purgeData();
    
private:
    AppRequests();
    virtual ~AppRequests();
    
    void didFetchAppRequests(const Vector<GraphRequest *> &request);
    
    static AppRequests *_instance;
    data::Data *_data;
};

NS_SCREW_FACEBOOK_END

#endif /* defined(__Screw__AppRequests__) */
