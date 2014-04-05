/*
 * Helper.h
 *
 *  Created on: Mar 18, 2014
 *      Author: hiepnd
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <jni.h>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

namespace jni {

class Helper {
public:
	Helper();
	virtual ~Helper();

	static void initialize(JNIEnv *env);

	static jstring string2jString(JNIEnv *env, const string &str);
	static string jString2String(JNIEnv *env, jstring jstr);

	static jobjectArray stringList2jStringArray(JNIEnv *env, const list<string> &items);
	static list<string> jStringArray2StringList(JNIEnv *env, jobjectArray array);

	/* ValueMap of string value */
	static jobject valueMap2jBundle(JNIEnv *env, const ValueMap &m);

	/* Bundle to ValueMap */
	static ValueMap jBundle2ValueMap(JNIEnv *env, jobject jbundle);

	static list<string> valueVector2StringList(const ValueVector &v);

public:
//	static JNIEnv *_env;

	/* Class IDs */
	static jclass jStringClassID;
	static jclass jBundleClassID;
	static jclass jSessionClassID;
	static jclass jRequestClassID;
	static jclass jDialogClassID;
	static jclass jAdsClassID;
	static jclass jUtilsClassID;

	/* Method IDs */
	static jmethodID jBundleConstructor;
	static jmethodID jBundlePutStringMethodID;
	static jmethodID jBundlePutBundleMethodID;
	static jmethodID jBundlePutStringArrayMethodID;
	static jmethodID jRequestRequestMethodID;
	static jmethodID jDialogShowMethodID;
	static jmethodID jAdsShowMethodID;
	static jmethodID jAdsHideMethodID;
	static jmethodID jAdsCreateMethodID;
	static jmethodID jUtilsNetWorkStatusMethodID;

	static bool _initialized;
};

} /* namespace jni */
#endif /* HELPER_H_ */
