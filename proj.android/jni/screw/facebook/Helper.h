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

	static void initialize();

	static jobjectArray stringList2jStringArray(JNIEnv *env, const list<string> &items);
	static list<string> jStringArray2StringList(JNIEnv *env, jobjectArray array);

	static jobject valueMap2jBundle(JNIEnv *env, const ValueMap &m);

private:
	static jclass _jStringClassID;
	static jclass _jBundleClassID;
	static jclass _jSessionClassID;
	static jclass _jRequestClassID;
};

} /* namespace jni */
#endif /* HELPER_H_ */
