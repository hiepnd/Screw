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

namespace jni {

SessionAndroid::SessionAndroid() {
	// TODO Auto-generated constructor stub

}

SessionAndroid::~SessionAndroid() {
	// TODO Auto-generated destructor stub
}

void SessionAndroid::open(bool allowUi , const list<string> &permissions) {
	//Java: public static void open(boolean allowUi, String[] permissions)
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "com/screw/facebook/Session", "open", "(Z[Ljava/lang/String;)V");
	jobjectArray jpermissions = Helper::stringList2jStringArray(methodInfo.env, permissions);
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,
					(jboolean) allowUi,
					jpermissions
				);

	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	methodInfo.env->DeleteLocalRef(jpermissions);
}

void SessionAndroid::close() {
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "com/screw/facebook/Session", "close", "()V");
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void SessionAndroid::requestReadPermissions(const list<string> &permissions) {
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "com/screw/facebook/Session", "requestReadPermissions", "([Ljava/lang/String;)V");
	jobjectArray jpermissions = Helper::stringList2jStringArray(methodInfo.env, permissions);// clist2jstringArray(methodInfo.env, permissions);
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jpermissions);

	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	methodInfo.env->DeleteLocalRef(jpermissions);
}

void SessionAndroid::requestPublishPermissions(const list<string> &permissions) {
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "com/screw/facebook/Session", "requestPublishPermissions", "([Ljava/lang/String;)V");
	jobjectArray jpermissions = Helper::stringList2jStringArray(methodInfo.env, permissions);// clist2jstringArray(methodInfo.env, permissions);
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jpermissions);

	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	methodInfo.env->DeleteLocalRef(jpermissions);
}

void SessionAndroid::start() {
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "com/screw/facebook/Session", "start", "()V");
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
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
	screw::facebook::Session::getActiveSession()->updateState((screw::facebook::Session::State) jstate,
																jni::Helper::jStringArray2StringList(env, permissions));
}

} //of extern "C"

