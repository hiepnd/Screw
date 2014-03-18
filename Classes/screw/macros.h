/*
 * macros.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef MACROS_H_
#define MACROS_H_

//Log
#if COCOS2D_DEBUG
//Turn debug ON/OFF here
#define FB_DEBUG 1
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



#define NS_SCREW_BEGIN namespace screw {
#define NS_SCREW_END }
#define USING_NS_SCREW using namespace screw;

#define NS_SCREW_UTILS_BEGIN NS_SCREW_BEGIN namespace utils {
#define NS_SCREW_UTILS_END } }
#define USING_NS_SCREW_UTILS using namespace screw::utils;

#define NS_SCREW_FACEBOOK_BEGIN NS_SCREW_BEGIN namespace facebook {
#define NS_SCREW_FACEBOOK_END } }
#define USING_NS_SCREW_FACEBOOK using namespace screw::facebook;

#define NS_SCREW_DATA_BEGIN NS_SCREW_BEGIN namespace data {
#define NS_SCREW_DATA_END } }
#define USING_NS_SCREW_DATA using namespace screw::data;

#endif /* MACROS_H_ */
