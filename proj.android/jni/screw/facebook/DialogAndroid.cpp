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

USING_NS_CC;

NS_SCREW_JNI_BEGIN

map<int, DialogCallback> DialogAndroid::_callbacks;
long DialogAndroid::_requestCode = 0;

bool DialogAndroid::canPresent(ShareDialogParams *params) {
	JNIEnv *env = JniHelper::getEnv();
	return (bool) env->CallStaticBooleanMethod(Helper::jDialogClassID, Helper::jDialogCanPresentShareDialogMethodID);
}

void DialogAndroid::present(ShareDialogParams *params, const DialogCallback &callback) {
	FB_LOG("DialogAndroid::show - showing dialog #%ld...", _requestCode + 1);
	_requestCode++;
	if (callback) {
		_callbacks[_requestCode] = callback;
	}
	JNIEnv *env = JniHelper::getEnv();
	jobject jparams = Helper::valueMap2jBundle(env, params->getValue().asValueMap());
	env->CallStaticVoidMethod(Helper::jDialogClassID, Helper::jDialogPresentShareDialogMethodID,
			(jlong)_requestCode,
			jparams
		);

	env->DeleteLocalRef(jparams);
}

bool DialogAndroid::canPresent(OpenGraphActionShareDialogParams *params) {
	JNIEnv *env = JniHelper::getEnv();
	return (bool) env->CallStaticBooleanMethod(Helper::jDialogClassID, Helper::jDialogCanPresentShareActionDialogMethodID);
}

void DialogAndroid::present(OpenGraphActionShareDialogParams *params, const DialogCallback &callback) {
	FB_LOG("DialogAndroid::show - showing dialog #%ld...", _requestCode + 1);
	_requestCode++;
	if (callback) {
		_callbacks[_requestCode] = callback;
	}
	JNIEnv *env = JniHelper::getEnv();
	jobject jparams = Helper::valueMap2jBundle(env, params->getValue().asValueMap());
	env->CallStaticVoidMethod(Helper::jDialogClassID, Helper::jDialogPresentShareActionDialogMethodID,
			(jlong)_requestCode,
			jparams
		);

	env->DeleteLocalRef(jparams);
}

void DialogAndroid::onDialogComplete(long requestCode, bool didCancel, const string &completionGesture, const string &postId, int error) {
	auto iter = _callbacks.find(requestCode);
	if (iter != _callbacks.end()) {
		ValueMap m;
		m["didCancel"] = didCancel;
		m["completionGesture"] = completionGesture;
		m["postId"] = postId;
		(iter->second)(GraphObject::create(Value(m)), error);
		_callbacks.erase(iter);
	}
}

NS_SCREW_JNI_END

extern "C" {
//private static native void nativeCompleteDialog(long code, boolean didCancel, String completionGesture, String postId, int error)
JNIEXPORT void JNICALL Java_com_screw_facebook_Dialog_nativeCompleteDialog(JNIEnv *env, jclass jclass, jlong jrequestCode,
													jboolean jdidCancel, jstring jcompletionGesture, jstring jpostId, jint jerror) {
	jni::DialogAndroid::onDialogComplete((long) jrequestCode,
									(bool) jdidCancel,
									jni::Helper::jString2String(env, jcompletionGesture),
									jni::Helper::jString2String(env, jpostId),
									(int) jerror
	);
}

} /* extern "C" { */
