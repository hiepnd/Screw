/*
 * Facebook.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "SessionAndroid.h"
#include <jni.h>
#include "cocos/2d/platform/android/jni/JniHelper.h"

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
	jobjectArray jpermissions = clist2jstringArray(methodInfo.env, permissions);
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jpermissions);
}

void SessionAndroid::requestPublishPermissions(const list<string> &permissions) {
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "com/screw/facebook/Session", "requestPublishPermissions", "([Ljava/lang/String;)V");
	jobjectArray jpermissions = clist2jstringArray(methodInfo.env, permissions);
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jpermissions);
}


} /* namespace jni */

extern "C" {
list<string> jstringArray2clist(JNIEnv *env, jobjectArray array) ;

JNIEXPORT void JNICALL Java_com_screw_facebook_Session_nativeInit (JNIEnv *env, jclass jclass, jint jstate, jstring jappid, jobjectArray permissions) {
	const char *appid = env->GetStringUTFChars(jappid, NULL);
	screw::facebook::Session::initActiveSession((screw::facebook::Session::State) jstate, appid, jstringArray2clist(env, permissions));
	env->ReleaseStringUTFChars(jappid, appid);

	ValueMap m;
	ccValueMap2AndroidBundle(env, m);
}

JNIEXPORT void JNICALL Java_com_screw_facebook_Session_nativeUpdateState(JNIEnv *env, jclass jclass, jint jstate, jobjectArray permissions) {
	screw::facebook::Session::getActiveSession()->updateState((screw::facebook::Session::State) jstate, jstringArray2clist(env, permissions));
}

jobjectArray clist2jstringArray(JNIEnv *env, const list<string> &items) {
	jclass jstringClass = env->FindClass("java/lang/String");
	jobjectArray jitems = (jobjectArray)env->NewObjectArray(items.size(), jstringClass, env->NewStringUTF(""));
	int i = 0;
	for (auto iter = items.begin(); iter != items.end(); iter++) {
		jstring s = env->NewStringUTF(iter->c_str());
		env->SetObjectArrayElement(jitems, i++, s);
	}
	return jitems;
}

list<string> jstringArray2clist(JNIEnv *env, jobjectArray array) {
	list<string> ll;
	jsize count = env->GetArrayLength(array);
	for (int i = 0; i < count; i++) {
		jstring jstr = (jstring) env->GetObjectArrayElement(array, i);
		const char *cstr = env->GetStringUTFChars(jstr, NULL);
		ll.push_back(cstr);
		env->ReleaseStringUTFChars(jstr, cstr);
	}

	return ll;
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

