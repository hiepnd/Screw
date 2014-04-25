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

#include "WebDialogAndroid.h"
#include <jni.h>
#include "cocos/2d/platform/android/jni/JniHelper.h"
#include "screw/utils/JsonUtils.h"

NS_SCREW_JNI_BEGIN

map<int, WebDialogCallback> WebDialogAndroid::_callbacks;
long WebDialogAndroid::_requestCode = 0;

WebDialogAndroid::WebDialogAndroid() {
	// TODO Auto-generated constructor stub

}

WebDialogAndroid::~WebDialogAndroid() {
	// TODO Auto-generated destructor stub
}

void WebDialogAndroid::show(WebDialog *dialog) {
	FB_LOG("WebDialogAndroid::show - showing dialog #%ld...", _requestCode + 1);
	_requestCode++;
	if (dialog->getCallback()) {
		_callbacks[_requestCode] = dialog->getCallback();
	}
	JNIEnv *env = JniHelper::getEnv();
	jstring jdialog = env->NewStringUTF(dialog->getDialog().c_str());
	jobject jparams = Helper::valueMap2jBundle(env, dialog->getParams());
	env->CallStaticVoidMethod(Helper::jWebDialogClassID, Helper::jWebDialogShowMethodID,
			(jlong)_requestCode,
			jdialog,
			jparams
		);

	env->DeleteLocalRef(jdialog);
	env->DeleteLocalRef(jparams);
}

void WebDialogAndroid::onDialogComplete(long requestCode, int errorCode, const string &errorMessage, const string &json) {
	auto iter = _callbacks.find(requestCode);
	if (iter != _callbacks.end()) {
		(iter->second)(errorCode, JsonUtils::parse(json).asValueMap());
		_callbacks.erase(iter);
	}
}

NS_SCREW_JNI_END /* namespace jni { */

extern "C" {
//private static native void nativeCompleteAppRequest(long requestCode, int error, String errorMessage, String jsonResponse);
JNIEXPORT void JNICALL Java_com_screw_facebook_WebDialog_nativeCompleteAppRequest(JNIEnv *env, jclass jclass, jlong jrequestCode,
													jint jerror, jstring jerrorMessage, jstring jjson) {

	FB_LOG("Dialog_nativeCompleteAppRequest - request code = %ld, error = %d", (long)jrequestCode, jerror);
	FB_LOG_INFO("Dialog_nativeCompleteAppRequest - json response = %s", jni::Helper::jString2String(env, jjson).c_str());
	string errorMessage = jni::Helper::jString2String(env, jerrorMessage);
	string json = jni::Helper::jString2String(env, jjson);
	jni::WebDialogAndroid::onDialogComplete((long)jrequestCode, (int)jerror, errorMessage, json);
}

} /* extern "C" { */
