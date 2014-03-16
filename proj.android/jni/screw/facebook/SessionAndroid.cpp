/*
 * Facebook.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "SessionAndroid.h"
#include <jni.h>
#include "cocos/2d/platform/android/jni/JniHelper.h"

namespace jni {

SessionAndroid::SessionAndroid() {
	// TODO Auto-generated constructor stub

}

SessionAndroid::~SessionAndroid() {
	// TODO Auto-generated destructor stub
}

void SessionAndroid::open() {
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "screw/facebook/Facebook", "openSession", "()V");
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
}

void SessionAndroid::close() {
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "screw/facebook/Facebook", "closeSession", "()V");
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
}

} /* namespace jni */

extern "C" {

JNIEXPORT void JNICALL Java_screw_facebook_Facebook_nativeInitSession (JNIEnv *env, jclass jclass, jint jstate, jstring jappid) {
	const char *appid = env->GetStringUTFChars(jappid, NULL);
	screw::facebook::Session::initActiveSession((screw::facebook::Session::State) jstate, appid);
	env->ReleaseStringUTFChars(jappid, appid);
}

JNIEXPORT void JNICALL Java_screw_facebook_Facebook_nativeUpdateSessionState(JNIEnv *enc, jclass jclass, jint jstate) {
	screw::facebook::Session::getActiveSession()->updateState((screw::facebook::Session::State) jstate);
}

}

