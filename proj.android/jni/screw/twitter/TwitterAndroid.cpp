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

#include "TwitterAndroid.h"
#include "cocos/2d/platform/android/jni/JniHelper.h"

NS_SCREW_JNI_BEGIN

TwitterTweetCallback TwitterAndroid::_currentCallback = nullptr;

void TwitterAndroid::tweet(const string &message, const string &imagePath, const TwitterTweetCallback &callback) {
	if (_currentCallback) {
		CCLOG("TwitterAndroid::tweet - possible bug");
	}
	_currentCallback = callback;

	JNIEnv *env = JniHelper::getEnv();
	jstring jmessage = env->NewStringUTF(message.c_str());
	jstring jimagePath = env->NewStringUTF(imagePath.c_str());
	env->CallStaticVoidMethod(Helper::jTwitterClassID, Helper::jTwitterTweetMethodID,
			jmessage, jimagePath
		);
}

void TwitterAndroid::onComplete(int error) {
	if (_currentCallback) {
		_currentCallback(error);
	}
	_currentCallback = nullptr;
}

NS_SCREW_JNI_END

extern "C" {
JNIEXPORT void JNICALL Java_com_screw_twitter_Twitter_nativeCallback (JNIEnv *env, jclass jclass, jint jerror) {
	screw::jni::TwitterAndroid::onComplete((int)jerror);
}

} //of extern "C"
