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

#ifndef HELPER_H_
#define HELPER_H_

#include <jni.h>
#include "cocos2d.h"
#include "screw/screw.h"

using namespace cocos2d;
using namespace std;

NS_SCREW_JNI_BEGIN

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

	static list<string> valueVector2StringList(const ValueVector &v);

public:
//	static JNIEnv *_env;

	/* Class IDs */
	static jclass jStringClassID;
	static jclass jBundleClassID;
	static jclass jSessionClassID;
	static jclass jRequestClassID;
	static jclass jWebDialogClassID;

	/* Method IDs */
	static jmethodID jBundleConstructor;
	static jmethodID jBundlePutStringMethodID;
	static jmethodID jBundlePutBundleMethodID;
	static jmethodID jBundlePutStringArrayMethodID;
	static jmethodID jRequestRequestMethodID;
	static jmethodID jWebDialogShowMethodID;

	static bool _initialized;
};

NS_SCREW_JNI_END /* namespace jni */

#endif /* HELPER_H_ */
