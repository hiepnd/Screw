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

#include "SessionAndroid.h"
#include <jni.h>
#include "cocos/2d/platform/android/jni/JniHelper.h"

NS_SCREW_JNI_BEGIN

SessionAndroid::SessionAndroid() {
	// TODO Auto-generated constructor stub

}

SessionAndroid::~SessionAndroid() {
	// TODO Auto-generated destructor stub
}

void SessionAndroid::open(bool allowUi, const list<string> &permissions, DefaultAudience defaultAudience, LoginBehavior loginBehavior) {
	//Java: public static void open(boolean allowUi, String[] permissions, final int defaultAudience, final int loginBehavior)
	JniMethodInfo methodInfo;
	JniHelper::getStaticMethodInfo(methodInfo, "com/screw/facebook/Session", "open", "(Z[Ljava/lang/String;II)V");
	jobjectArray jpermissions = Helper::stringList2jStringArray(methodInfo.env, permissions);
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,
					(jboolean) allowUi,
					jpermissions,
					(jint)defaultAudience,
					(jint)loginBehavior
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

NS_SCREW_JNI_END /* namespace jni */

extern "C" {
JNIEXPORT void JNICALL Java_com_screw_facebook_Session_nativeInit (JNIEnv *env, jclass jclass, jint jstate, jstring jappid, jobjectArray permissions) {
	/* Init the Helper first */
	screw::jni::Helper::initialize(env);

	const char *appid = env->GetStringUTFChars(jappid, NULL);
	screw::facebook::Session::initActiveSession((screw::facebook::Session::State) jstate, appid, jni::Helper::jStringArray2StringList(env, permissions));
	env->ReleaseStringUTFChars(jappid, appid);
}

JNIEXPORT void JNICALL Java_com_screw_facebook_Session_nativeUpdateState(JNIEnv *env, jclass jclass, jint jstate, jobjectArray permissions) {
	screw::facebook::Session::getActiveSession()->updateState((screw::facebook::Session::State) jstate,
																jni::Helper::jStringArray2StringList(env, permissions), nullptr);
}

} //of extern "C"

