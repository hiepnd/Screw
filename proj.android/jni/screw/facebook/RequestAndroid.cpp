/*
 * RequestAndroid.cpp
 *
 *  Created on: Mar 18, 2014
 *      Author: hiepnd
 */

#include "RequestAndroid.h"
#include <jni.h>
#include "cocos/2d/platform/android/jni/JniHelper.h"
#include "Helper.h"

namespace jni {

map<int, RequestCallback> RequestAndroid::_callbacks;
long RequestAndroid::_requestCode = 0;
RequestAndroid::RequestAndroid() {
	// TODO Auto-generated constructor stub

}

RequestAndroid::~RequestAndroid() {
	// TODO Auto-generated destructor stub
}

void RequestAndroid::execute(Request *request) {
	FB_LOG("RequestAndroid::execute - executing request #%ld...", _requestCode + 1);
	if (request->getCallback()) {
		_callbacks[++_requestCode] = request->getCallback();
	}

	//Java: void request(long requestCode, String graphPath, Bundle parameters, int httpMethod) {
	JNIEnv *env = JniHelper::getEnv();
	env->CallStaticVoidMethod(Helper::_jRequestClassID, Helper::_jRequestRequestMethodID,
			_requestCode, //Code
			NULL,//Helper::string2jString(env, request->getGraphPath()), //graphPath
			NULL,//Helper::valueMap2jBundle(env, request->getParams().asValueMap()), //params
			request->getMethod()
			);
}

void RequestAndroid::receiveResponse(long requestCode, int errorCode, const string &errorMessage, const string &result) {
	auto iter = _callbacks.find(requestCode);
	if (iter != _callbacks.end()) {
		(iter->second)(nullptr);
		_callbacks.erase(iter);
	}
}

} /* namespace jni */

extern "C" {
JNIEXPORT void JNICALL Java_com_screw_facebook_Request_nativeCallback(JNIEnv *env, jclass jclass, jlong jrequestCode, jint jerror, jstring jerrorMessage, jstring jresult) {
	FB_LOG("Got result: %s", jni::Helper::jString2String(env, jresult).c_str());

	string errorMessage = jni::Helper::jString2String(env, jerrorMessage);
	string result = jni::Helper::jString2String(env, jresult);
	jni::RequestAndroid::receiveResponse(jrequestCode, jerror, errorMessage, result);
}

} /* extern "C" { */
