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

class Request;

class RequestImpl {
public:
    RequestImpl(){};
    virtual ~RequestImpl(){}
    
    virtual void execute(Request *request) = 0;
};

class Request : public Object {
public:
    enum Method {GET, POST, DELETE};
    
    Request();
	Request(const string &graphPath, const Value &params, Method method, const RequestCallback &callback);
    static Request *create(const string &graphPath, const Value &params, Method method, const RequestCallback &callback);
	virtual ~Request();
    
    void setGraphPath(const string &graphPath);
    void setParams(const Value &params);
    void setMethod(Method method);
    void setCallback(const RequestCallback &callback);
    
    const string &getGraphPath();
    const Value &getParams();
    Method getMethod();
    const RequestCallback &getCallback();
    
    void execute();
    
protected:
    Method _method;
    string _graphPath;
    Value _params;
    RequestCallback _callback;
    RequestImpl *_impl;
    
    /* Requests in progress */
    static Vector<Request *> _requests;
};

NS_SCREW_FACEBOOK_END

#endif /* REQUEST_H_ */
