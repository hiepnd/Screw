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

#include "RequestAndroid.h"
#include "screw/utils/JsonUtils.h"
#include <jni.h>
#include "cocos/2d/platform/android/jni/JniHelper.h"
#include "Helper.h"

NS_SCREW_JNI_BEGIN

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

NS_SCREW_JNI_END /* namespace jni */

extern "C" {
JNIEXPORT void JNICALL Java_com_screw_facebook_Request_nativeCallback(JNIEnv *env, jclass jclass, jlong jrequestCode, jint jerror, jstring jerrorMessage, jstring jresult) {
	FB_LOG("Request_nativeCallback - error = %d", (int)jerror);
	FB_LOG_INFO("Request_nativeCallback - Result = %s", jni::Helper::jString2String(env, jresult).c_str());

	string errorMessage = jni::Helper::jString2String(env, jerrorMessage);
	string result = jni::Helper::jString2String(env, jresult);
	jni::RequestAndroid::onRequestComplete((long)jrequestCode, (int)jerror, errorMessage, result);
}

} /* extern "C" { */
