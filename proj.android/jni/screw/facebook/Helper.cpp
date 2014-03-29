/*
 * Helper.cpp
 *
 *  Created on: Mar 18, 2014
 *      Author: hiepnd
 */

#include "Helper.h"
#include "cocos/2d/platform/android/jni/JniHelper.h"

namespace jni {

bool Helper::_initialized = false;
//JNIEnv *Helper::_env = NULL;

jclass Helper::jStringClassID = NULL;
jclass Helper::jBundleClassID = NULL;
jclass Helper::jSessionClassID = NULL;
jclass Helper::jRequestClassID = NULL;
jclass Helper::jDialogClassID = NULL;

jmethodID Helper::jBundleConstructor = NULL;
jmethodID Helper::jBundlePutStringMethodID = NULL;
jmethodID Helper::jBundlePutBundleMethodID = NULL;
jmethodID Helper::jBundlePutStringArrayMethodID = NULL;
jmethodID Helper::jRequestRequestMethodID = NULL;
jmethodID Helper::jDialogShowMethodID = NULL;

Helper::Helper() {

}

Helper::~Helper() {

}

void Helper::initialize(JNIEnv *env) {
	CCASSERT(!_initialized, "Try to init twice ???");
	CCLOG("jni::Helper::initializing ...");
	_initialized = true;

	jBundleClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("android/os/Bundle")));
	jSessionClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("com/screw/facebook/Session")));
	jRequestClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("com/screw/facebook/Request")));
	jDialogClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("com/screw/facebook/Dialog")));
	jStringClassID = (jclass)env->NewGlobalRef((jobject)(env->FindClass("java/lang/String")));

	jBundleConstructor = env->GetMethodID(jBundleClassID, "<init>", "()V");
	jBundlePutStringMethodID = env->GetMethodID(jBundleClassID, "putString", "(Ljava/lang/String;Ljava/lang/String;)V");
	jBundlePutBundleMethodID = env->GetMethodID(jBundleClassID, "putBundle", "(Ljava/lang/String;Landroid/os/Bundle;)V");
	jBundlePutStringArrayMethodID = env->GetMethodID(jBundleClassID, "putStringArray", "(Ljava/lang/String;[Ljava/lang/String;)V");

	//Java: void request(long requestCode, String graphPath, Bundle parameters, int httpMethod)
	jRequestRequestMethodID = env->GetStaticMethodID(jRequestClassID, "request" ,"(JLjava/lang/String;Landroid/os/Bundle;I)V");

	//Java: static void showRequest(long requestCode, String dialog, Bundle params)
	jDialogShowMethodID = env->GetStaticMethodID(jDialogClassID, "show" ,"(JLjava/lang/String;Landroid/os/Bundle;)V");

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

ValueMap Helper::jBundle2ValueMap(JNIEnv *env, jobject jbundle) {

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

list<string> Helper::valueVector2StringList(const ValueVector &v) {
	list<string> ret;
	for (auto i = v.begin(); i != v.end(); i++) {
		ret.push_back(i->asString());
	}
	return ret;
}


} /* namespace jni */

extern "C" {

jint JNI_OnLoad( JavaVM *vm, void *pvt )
{
	CCLOG("JNI_OnLoad JNI_OnLoad ....");
//	jni::Helper::initialize(vm->GetEnv());
	return JNI_VERSION_1_6;
}

} /* extern "C" */
