/*
 * RequestAndroid.cpp
 *
 *  Created on: Mar 18, 2014
 *      Author: hiepnd
 */

#include "RequestAndroid.h"
#include "screw/utils/JsonUtils.h"
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
	_requestCode++;
	if (request->getCallback()) {
		_callbacks[_requestCode] = request->getCallback();
	}

	JNIEnv *env = JniHelper::getEnv();
	jstring jgraphPath = env->NewStringUTF(request->getGraphPath().c_str());
	jobject jparams = Helper::valueMap2jBundle(env, request->getParams());
	env->CallStaticVoidMethod(Helper::jRequestClassID, Helper::jRequestRequestMethodID,
			(jlong)_requestCode,
			jgraphPath,
			jparams,
			(jint)request->getMethod()
		);

	env->DeleteLocalRef(jgraphPath);
	env->DeleteLocalRef(jparams);
}

void RequestAndroid::onRequestComplete(long requestCode, int errorCode, const string &errorMessage, const string &result) {
	auto iter = _callbacks.find(requestCode);
	if (iter != _callbacks.end()) {
		(iter->second)(errorCode, GraphObject::create(Value(screw::utils::JsonUtils::parse(result))));
		_callbacks.erase(iter);
	}
}

} /* namespace jni */

extern "C" {
JNIEXPORT void JNICALL Java_com_screw_facebook_Request_nativeCallback(JNIEnv *env, jclass jclass, jlong jrequestCode, jint jerror, jstring jerrorMessage, jstring jresult) {
	FB_LOG("Request_nativeCallback - error = %d", (int)jerror);
	FB_LOG_INFO("Request_nativeCallback - Result = %s", jni::Helper::jString2String(env, jresult).c_str());

	string errorMessage = jni::Helper::jString2String(env, jerrorMessage);
	string result = jni::Helper::jString2String(env, jresult);
	jni::RequestAndroid::onRequestComplete((long)jrequestCode, (int)jerror, errorMessage, result);
}

} /* extern "C" { */
