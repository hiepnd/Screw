/*
 * Facebook.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "SessionAndroid.h"
#include <jni.h>
#include "cocos/2d/platform/android/jni/JniHelper.h"
#include "Helper.h"

extern "C" {
jobjectArray clist2jstringArray(JNIEnv *env, const list<string> &items);
jobject ccValueMap2AndroidBundle(JNIEnv *env, const ValueMap &vmap);
}

namespace jni {

SessionAndroid::SessionAndroid() {
	// TODO Auto-generated constructor stub

}

SessionAndroid::~SessionAndroid() {
	// TODO Auto-generated destructor stub
}

void SessionAndroid::open() {
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "com/screw/facebook/Session", "open", "()V");
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
}

void SessionAndroid::close() {
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "com/screw/facebook/Session", "close", "()V");
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
}

void SessionAndroid::requestReadPermissions(const list<string> &permissions) {
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "com/screw/facebook/Session", "requestReadPermissions", "([Ljava/lang/String;)V");
	jobjectArray jpermissions = Helper::stringList2jStringArray(methodInfo.env, permissions);// clist2jstringArray(methodInfo.env, permissions);
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jpermissions);
}

void SessionAndroid::requestPublishPermissions(const list<string> &permissions) {
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "com/screw/facebook/Session", "requestPublishPermissions", "([Ljava/lang/String;)V");
	jobjectArray jpermissions = Helper::stringList2jStringArray(methodInfo.env, permissions);// clist2jstringArray(methodInfo.env, permissions);
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jpermissions);
}


} /* namespace jni */

extern "C" {
JNIEXPORT void JNICALL Java_com_screw_facebook_Session_nativeInit (JNIEnv *env, jclass jclass, jint jstate, jstring jappid, jobjectArray permissions) {
	/* Init the Helper first */
	jni::Helper::initialize(env);

	const char *appid = env->GetStringUTFChars(jappid, NULL);
	screw::facebook::Session::initActiveSession((screw::facebook::Session::State) jstate, appid, jni::Helper::jStringArray2StringList(env, permissions));
	env->ReleaseStringUTFChars(jappid, appid);
}

JNIEXPORT void JNICALL Java_com_screw_facebook_Session_nativeUpdateState(JNIEnv *env, jclass jclass, jint jstate, jobjectArray permissions) {
	screw::facebook::Session::getActiveSession()->updateState((screw::facebook::Session::State) jstate, jni::Helper::jStringArray2StringList(env, permissions));
}

jobject ccValueMap2AndroidBundle(JNIEnv *env, const ValueMap &vmap) {
	jclass jBundleClass = env->FindClass("android/os/Bundle");
	jclass jStringClass = env->FindClass("java/lang/String");
	jmethodID jBundleConstructor = env->GetMethodID(jBundleClass, "<init>", "()V");

	jobject jBundle = env->NewObject(jBundleClass, jBundleConstructor);

	jmethodID jBundlePutStringMethod = env->GetMethodID(jBundleClass, "putString", "(Ljava/lang/String;Ljava/lang/String;)V");

	jstring k1 = env->NewStringUTF("hello");
	jstring v1 = env->NewStringUTF("world");

	jstring k2 = env->NewStringUTF("I'm");
	jstring v2 = env->NewStringUTF("Hiepnd - The Awesome !!!");

	env->CallVoidMethod(jBundle, jBundlePutStringMethod, k1, v1);
	env->CallVoidMethod(jBundle, jBundlePutStringMethod, k2, v2);

	jclass jRequestClass = env->FindClass("com/screw/facebook/Request");
	jmethodID jTestMethod = env->GetStaticMethodID(jRequestClass, "test", "(Landroid/os/Bundle;)V");
	env->CallStaticVoidMethod(jRequestClass, jTestMethod, jBundle);
}

} //of extern "C"

