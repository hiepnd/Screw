/*
 * RequestAndroid.h
 *
 *  Created on: Mar 18, 2014
 *      Author: hiepnd
 */

#ifndef REQUESTANDROID_H_
#define REQUESTANDROID_H_

#include "screw/facebook/Request.h"
#include "Helper.h"
#include <map>

using namespace std;
USING_NS_SCREW_FACEBOOK;

namespace jni {

class RequestAndroid : public RequestImpl {
public:
	RequestAndroid();
	virtual ~RequestAndroid();

	virtual void execute(Request *request) override;

	static void onRequestComplete(long requestCode, int errorCode, const string &errorMessage, const string &result);

private:
	static map<int, RequestCallback> _callbacks;
	static long _requestCode;
};

} /* namespace jni */
#endif /* REQUESTANDROID_H_ */
