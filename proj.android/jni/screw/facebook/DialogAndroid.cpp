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

#include "DialogAndroid.h"
#include <jni.h>
#include "cocos/2d/platform/android/jni/JniHelper.h"
#include "Helper.h"

NS_SCREW_JNI_BEGIN

map<int, DialogCallback> DialogAndroid::_callbacks;
long DialogAndroid::_requestCode = 0;

DialogAndroid::DialogAndroid() {
	// TODO Auto-generated constructor stub

}

DialogAndroid::~DialogAndroid() {
	// TODO Auto-generated destructor stub
}

void DialogAndroid::show(Dialog *dialog) {
	FB_LOG("DialogAndroid::show - showing dialog #%ld...", _requestCode + 1);
	_requestCode++;
	if (dialog->getCallback()) {
		_callbacks[_requestCode] = dialog->getCallback();
	}
	JNIEnv *env = JniHelper::getEnv();
	jstring jdialog = env->NewStringUTF(dialog->getDialog().c_str());
	jobject jparams = Helper::valueMap2jBundle(env, dialog->getParams());
	env->CallStaticVoidMethod(Helper::jDialogClassID, Helper::jDialogShowMethodID,
			(jlong)_requestCode,
			jdialog,
			jparams
		);

	env->DeleteLocalRef(jdialog);
	env->DeleteLocalRef(jparams);
}

void DialogAndroid::onDialogComplete(long requestCode, int errorCode, const string &errorMessage,
										const string &requestId, const list<string> &toes) {
	auto iter = _callbacks.find(requestCode);
	if (iter != _callbacks.end()) {
		(iter->second)(errorCode, requestId, toes);
		_callbacks.erase(iter);
	}
}

NS_SCREW_JNI_END /* namespace jni { */


extern "C" {
//static native void nativeCompleteAppRequest(long requestCode, int error, String errorMessage, String requestId, String []toes);
JNIEXPORT void JNICALL Java_com_screw_facebook_Dialog_nativeCompleteAppRequest(JNIEnv *env, jclass jclass, jlong jrequestCode,
													jint jerror, jstring jerrorMessage, jstring jrequestId, jobjectArray jreceivers) {

	FB_LOG("Dialog_nativeCompleteAppRequest - error = %d", jerror);
	string errorMessage = jni::Helper::jString2String(env, jerrorMessage);
	string requestId = jni::Helper::jString2String(env, jrequestId);
	list<string> receivers = jni::Helper::jStringArray2StringList(env, jreceivers);
	jni::DialogAndroid::onDialogComplete((long)jrequestCode, (int)jerror, errorMessage, requestId, receivers);
}

} /* extern "C" { */
