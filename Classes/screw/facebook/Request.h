/*
 * Request.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef REQUEST_H_
#define REQUEST_H_
#include "../macros.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

NS_SCREW_FACEBOOK_BEGIN

typedef std::function<void(Object *)> RequestCallback;

class RequestImpl {
public:
    RequestImpl(){};
    virtual ~RequestImpl(){}
    
    virtual void execute() = 0;
};

class Request : public Object {
public:
    enum Method{POST, GET, DELETE};
    
	Request(const string &graphPath, const ValueMap &params, Method method = GET, const RequestCallback &callback = nullptr);
    static Request *create(const string &graphPath, const ValueMap &params, Method method = GET, const RequestCallback &callback = nullptr);
	virtual ~Request();
    
    void setCallback(const RequestCallback &callback);
    void execute();
    
protected:
    Method _method;
    string _graphPath;
    ValueMap _params;
    RequestCallback _callback;
    RequestImpl *_impl;
};

NS_SCREW_FACEBOOK_END

#endif /* REQUEST_H_ */
