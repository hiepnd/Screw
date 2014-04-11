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

#ifndef MACROS_H_
#define MACROS_H_

//Log
#if COCOS2D_DEBUG
//Turn debug ON/OFF here
#define FB_DEBUG 2
#else
#define FB_DEBUG 0
#endif

#if !COCOS2D_DEBUG && FB_DEBUG
#error "Turn off FB_DEBUG in Release Mode"
#endif

#if FB_DEBUG
#define FB_LOG(format, ...) cocos2d::log(format, ##__VA_ARGS__)
#else
#define FB_LOG(...) do {} while (0)
#endif

#if FB_DEBUG
#define FB_LOG_WARN(format, ...) cocos2d::log(format, ##__VA_ARGS__)
#else
#define FB_LOG_WARN(...) do {} while (0)
#endif

#if FB_DEBUG >= 2
#define FB_LOG_INFO(format, ...) cocos2d::log(format, ##__VA_ARGS__)
#else
#define FB_LOG_INFO(...) do {} while (0)
#endif

#define NS_SCREW_BEGIN namespace screw {
#define NS_SCREW_END }
#define USING_NS_SCREW using namespace screw;

#define NS_SCREW_UTILS_BEGIN NS_SCREW_BEGIN namespace utils {
#define NS_SCREW_UTILS_END NS_SCREW_END }
#define USING_NS_SCREW_UTILS using namespace screw::utils;

#define NS_SCREW_FACEBOOK_BEGIN NS_SCREW_BEGIN namespace facebook {
#define NS_SCREW_FACEBOOK_END NS_SCREW_END }
#define USING_NS_SCREW_FACEBOOK using namespace screw::facebook;

#define NS_SCREW_DATA_BEGIN NS_SCREW_BEGIN namespace data {
#define NS_SCREW_DATA_END NS_SCREW_END }
#define USING_NS_SCREW_DATA using namespace screw::data;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#   define NS_SCREW_IOS_BEGIN NS_SCREW_BEGIN namespace ios {
#   define NS_SCREW_IOS_END NS_SCREW_END }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#   define NS_SCREW_JNI_BEGIN NS_SCREW_BEGIN namespace jni {
#   define NS_SCREW_JNI_END NS_SCREW_END }
#endif


#endif /* MACROS_H_ */
