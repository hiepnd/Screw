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

}

void SessionAndroid::close() {

}

void SessionAndroid::initActiveSession(Session::State state, const string &appid) {
	screw::facebook::Session::initActiveSession(state, "");
}
void SessionAndroid::updateState(Session::State state) {
	screw::facebook::Session::getActiveSession()->updateState(state);
}

} /* namespace jni */

extern "C" {

JNIEXPORT void JNICALL Java_screw_facebook_Facebook_nativeInitSession (JNIEnv *env, jclass jclass, jint jstate, jstring jappid) {
	screw::facebook::Session::initActiveSession((screw::facebook::Session::State) jstate, "");
}

JNIEXPORT void JNICALL Java_screw_facebook_Facebook_nativeUpdateSessionState(JNIEnv *enc, jclass jclass, jint jstate) {
	screw::facebook::Session::getActiveSession()->updateState((screw::facebook::Session::State) jstate);
}

JNIEXPORT void JNICALL Java_screw_facebook_Facebook_nativeFoo(JNIEnv *enc, jclass jclass, jint jstate) {
	printf("Call Foo from JAVA");
}

}

