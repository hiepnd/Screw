/*
 * DialogAndroid.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: hiepnd
 */

#include "DialogAndroid.h"
#include <jni.h>
#include "cocos/2d/platform/android/jni/JniHelper.h"
#include "Helper.h"

namespace jni {

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

} /* namespace jni { */


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
