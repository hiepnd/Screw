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

jclass Helper::_jStringClassID = NULL;
jclass Helper::_jBundleClassID = NULL;
jclass Helper::_jSessionClassID = NULL;
jclass Helper::_jRequestClassID = NULL;

jmethodID Helper::_jBundleConstructor = NULL;
jmethodID Helper::_jBundlePutStringMethodID = NULL;
jmethodID Helper::_jBundlePutBundleMethodID = NULL;
jmethodID Helper::_jBundlePutStringArrayMethodID = NULL;
jmethodID Helper::_jRequestRequestMethodID = NULL;

Helper::Helper() {

}

Helper::~Helper() {

}

void Helper::initialize(JNIEnv *env) {
	CCASSERT(!_initialized, "Try to init twice ???");
	CCLOG("jni::Helper::initializing ...");
	_initialized = true;

	_jBundleClassID = env->FindClass("android/os/Bundle");
	_jSessionClassID = env->FindClass("com/screw/facebook/Session");
	_jRequestClassID = env->FindClass("com/screw/facebook/Request");
	_jStringClassID = env->FindClass("java/lang/String");

	_jBundleConstructor = env->GetMethodID(_jBundleClassID, "<init>", "()V");
	_jBundlePutStringMethodID = env->GetMethodID(_jBundleClassID, "putString", "(Ljava/lang/String;Ljava/lang/String;)V");
	_jBundlePutBundleMethodID = env->GetMethodID(_jBundleClassID, "putBundle", "(Ljava/lang/String;Landroid/os/Bundle;)V");
	_jBundlePutStringArrayMethodID = env->GetMethodID(_jBundleClassID, "putStringArray", "(Ljava/lang/String;[Ljava/lang/String;)V");

	//Java: void request(long requestCode, String graphPath, Bundle parameters, int httpMethod)
	_jRequestRequestMethodID = env->GetStaticMethodID(_jRequestClassID, "request" ,"(JLjava/lang/String;Landroid/os/Bundle;I)V");

	//Test
	ValueMap mm;
	mm["k1"] = "v1";
	mm["k2"] = "v2";
	ValueVector vv;
	vv.push_back(Value("e1"));
	vv.push_back(Value("e2"));
	vv.push_back(Value("e3"));
	mm["k3"] = vv;

	ValueMap mmm;
	mmm["k4"] = "v4";
	mmm["k5"] = "v5";

	mm["k4"] = mmm;
	jobject jBundle = valueMap2jBundle(env, mm);
	jmethodID jTestMethod = env->GetStaticMethodID(_jRequestClassID, "test", "(Landroid/os/Bundle;)V");
	env->CallStaticVoidMethod(_jRequestClassID, jTestMethod, jBundle);
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
	jobjectArray jitems = (jobjectArray)env->NewObjectArray(items.size(), _jStringClassID, env->NewStringUTF(""));
	int i = 0;
	for (auto iter = items.begin(); iter != items.end(); iter++) {
		jstring s = env->NewStringUTF(iter->c_str());
		env->SetObjectArrayElement(jitems, i++, s);
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
	jobject jBundle = env->NewObject(_jBundleClassID, _jBundleConstructor);

//	jstring k1 = env->NewStringUTF("hello");
//	jstring v1 = env->NewStringUTF("world");
//
//	jstring k2 = env->NewStringUTF("I'm");
//	jstring v2 = env->NewStringUTF("Hiepnd - The Awesome !!!");
//
//	env->CallVoidMethod(jBundle, _jBundlePutStringMethodID, k1, v1);
//	env->CallVoidMethod(jBundle, _jBundlePutStringMethodID, k2, v2);

	for (auto i = m.begin(); i != m.end(); i++) {
		jstring jkey = env->NewStringUTF(i->first.c_str());
		const Value &v = i->second;
		switch (v.getType()) {
			case Value::Type::MAP:
			{
				jobject jvalue = valueMap2jBundle(env, v.asValueMap());
				env->CallVoidMethod(jBundle, _jBundlePutBundleMethodID, jkey, jvalue);
			}
				break;
			case Value::Type::VECTOR:
			{
				list<string> ll = valueVector2StringList(v.asValueVector());
				jobjectArray jvalue = stringList2jStringArray(env, ll);
				env->CallVoidMethod(jBundle, _jBundlePutStringArrayMethodID, jkey, jvalue);
			}
				break;

			default:
			{
				jstring jvalue = env->NewStringUTF(v.asString().c_str());
				env->CallVoidMethod(jBundle, _jBundlePutStringMethodID, jkey, jvalue);
			}
				break;
		}
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
