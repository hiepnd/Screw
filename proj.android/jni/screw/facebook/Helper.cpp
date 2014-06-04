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

#include "Helper.h"
#include "cocos/2d/platform/android/jni/JniHelper.h"

NS_SCREW_JNI_BEGIN

bool Helper::_initialized = false;
//JNIEnv *Helper::_env = NULL;

jclass Helper::jStringClassID = NULL;
jclass Helper::jBundleClassID = NULL;
jclass Helper::jSessionClassID = NULL;
jclass Helper::jRequestClassID = NULL;
jclass Helper::jWebDialogClassID = NULL;
jclass Helper::jDialogClassID = NULL;
jclass Helper::jObjectClassID = NULL;
jclass Helper::jTwitterClassID = NULL;

jmethodID Helper::jBundleConstructor = NULL;
jmethodID Helper::jBundlePutStringMethodID = NULL;
jmethodID Helper::jBundlePutBundleMethodID = NULL;
jmethodID Helper::jBundlePutStringArrayMethodID = NULL;
jmethodID Helper::jRequestRequestMethodID = NULL;
jmethodID Helper::jWebDialogShowMethodID = NULL;
jmethodID Helper::jDialogCanPresentShareDialogMethodID = NULL;
jmethodID Helper::jDialogCanPresentShareActionDialogMethodID = NULL;
jmethodID Helper::jDialogPresentShareDialogMethodID = NULL;
jmethodID Helper::jDialogPresentShareActionDialogMethodID = NULL;
jmethodID Helper::jTwitterTweetMethodID = NULL;

Helper::Helper() {

}

Helper::~Helper() {

}

void Helper::initialize(JNIEnv *env) {
	CCASSERT(!_initialized, "Try to init twice ???");
	CCLOG("screw::jni::Helper::initializing ...");
	_initialized = true;

	jObjectClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("java/lang/Object")));
	jBundleClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("android/os/Bundle")));
	jSessionClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("com/screw/facebook/Session")));
	jRequestClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("com/screw/facebook/Request")));
	jWebDialogClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("com/screw/facebook/WebDialog")));
	jStringClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("java/lang/String")));
	jDialogClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("com/screw/facebook/Dialog")));
	jTwitterClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("com/screw/twitter/Twitter")));

	jBundleConstructor = env->GetMethodID(jBundleClassID, "<init>", "()V");
	jBundlePutStringMethodID = env->GetMethodID(jBundleClassID, "putString", "(Ljava/lang/String;Ljava/lang/String;)V");
	jBundlePutBundleMethodID = env->GetMethodID(jBundleClassID, "putBundle", "(Ljava/lang/String;Landroid/os/Bundle;)V");
	jBundlePutStringArrayMethodID = env->GetMethodID(jBundleClassID, "putStringArray", "(Ljava/lang/String;[Ljava/lang/String;)V");

	//Java: void request(long requestCode, String graphPath, Bundle parameters, int httpMethod)
	jRequestRequestMethodID = env->GetStaticMethodID(jRequestClassID, "request", "(JLjava/lang/String;Landroid/os/Bundle;I)V");

	//Java: static void showRequest(long requestCode, String dialog, Bundle params)
	jWebDialogShowMethodID = env->GetStaticMethodID(jWebDialogClassID, "show", "(JLjava/lang/String;Landroid/os/Bundle;)V");

	//public static boolean canPresentShareDialog()
	jDialogCanPresentShareDialogMethodID = env->GetStaticMethodID(jDialogClassID, "canPresentShareDialog" ,"()Z");

	//public static boolean canSharePresentShareOpenGraphAction()
	jDialogCanPresentShareActionDialogMethodID = env->GetStaticMethodID(jDialogClassID, "canPresentShareDialog" ,"()Z");

	//public static void presentShareDialog(final long requestCode, final Bundle data)
	jDialogPresentShareDialogMethodID = env->GetStaticMethodID(jDialogClassID, "presentShareDialog",
																"(JLandroid/os/Bundle;)V");

	//public static void presentShareOpenGraphActionDialog(final long requestCode, final Bundle data)
	jDialogPresentShareActionDialogMethodID = env->GetStaticMethodID(jDialogClassID, "presentShareOpenGraphActionDialog",
																"(JLandroid/os/Bundle;)V");

	//public static void tweet(final String message)
	jTwitterTweetMethodID = env->GetStaticMethodID(jTwitterClassID, "tweet", "(Ljava/lang/String;)V");
}

jstring Helper::string2jString(JNIEnv *env, const string &str) {
	return env->NewStringUTF(str.c_str());
}

string Helper::jString2String(JNIEnv *env, jstring jstr) {
	const char *cs = env->GetStringUTFChars(jstr, NULL);
	string ret(cs);
	env->ReleaseStringUTFChars(jstr, cs);
	return ret;
}

jobjectArray Helper::stringList2jStringArray(JNIEnv *env, const list<string> &items) {
	jobjectArray jitems = (jobjectArray)env->NewObjectArray(items.size(), jStringClassID, env->NewStringUTF(""));
	int i = 0;
	for (auto iter = items.begin(); iter != items.end(); iter++) {
		jstring s = env->NewStringUTF(iter->c_str());
		env->SetObjectArrayElement(jitems, i++, s);
		env->DeleteLocalRef(s);
	}
	return jitems;
}

list<string> Helper::jStringArray2StringList(JNIEnv *env, jobjectArray array) {
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

jobject Helper::valueMap2jBundle(JNIEnv *env, const ValueMap &m) {
	jobject jBundle = env->NewObject(jBundleClassID, jBundleConstructor);

	for (auto i = m.begin(); i != m.end(); i++) {
		jstring jkey = env->NewStringUTF(i->first.c_str());
		const Value &v = i->second;
		switch (v.getType()) {
			case Value::Type::MAP:
			{
				jobject jvalue = valueMap2jBundle(env, v.asValueMap());
				env->CallVoidMethod(jBundle, jBundlePutBundleMethodID, jkey, jvalue);
				env->DeleteLocalRef(jvalue);
			}
				break;
			case Value::Type::VECTOR:
			{
				list<string> ll = valueVector2StringList(v.asValueVector());
				jobjectArray jvalue = stringList2jStringArray(env, ll);
				env->CallVoidMethod(jBundle, jBundlePutStringArrayMethodID, jkey, jvalue);
				env->DeleteLocalRef(jvalue);
			}
				break;

			default:
			{
				jstring jvalue = env->NewStringUTF(v.asString().c_str());
				env->CallVoidMethod(jBundle, jBundlePutStringMethodID, jkey, jvalue);
				env->DeleteLocalRef(jvalue);
			}
				break;
		}
		env->DeleteLocalRef(jkey);
	}

	return jBundle;
}

jobjectArray Helper::valueVector2jObjectArray(JNIEnv *env, const ValueVector &vector) {
	jobjectArray jitems = (jobjectArray)env->NewObjectArray(vector.size(), jStringClassID, env->NewStringUTF(""));
	int i = 0;
	for (auto iter = vector.begin(); iter != vector.end(); iter++) {
		switch (iter->getType()) {
			case Value::Type::MAP:
			{
				jobject jvalue = valueMap2jBundle(env, iter->asValueMap());
				env->SetObjectArrayElement(jitems, i++, jvalue);
				env->DeleteLocalRef(jvalue);
			}
				break;
			case Value::Type::VECTOR:
			{
				jobject jvalue = valueVector2jObjectArray(env, iter->asValueVector());
				env->SetObjectArrayElement(jitems, i++, jvalue);
				env->DeleteLocalRef(jvalue);
			}
				break;

			default:
			{
				jstring jvalue = env->NewStringUTF(iter->asString().c_str());
				env->SetObjectArrayElement(jitems, i++, jvalue);
				env->DeleteLocalRef(jvalue);
			}
				break;
		}
	}
	return jitems;
}

list<string> Helper::valueVector2StringList(const ValueVector &v) {
	list<string> ret;
	for (auto i = v.begin(); i != v.end(); i++) {
		ret.push_back(i->asString());
	}
	return ret;
}


NS_SCREW_JNI_END /* namespace jni */

extern "C" {

} /* extern "C" */
