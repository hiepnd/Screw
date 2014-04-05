/*
 * JsonUtils.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef JSONUTILS_H_
#define JSONUTILS_H_

#include "cocos2d.h"
#include <string>
#include "../macros.h"
#include <external/json/rapidjson.h>
#include <external/json/document.h>

using namespace cocos2d;
using namespace std;

NS_SCREW_UTILS_BEGIN

static string map2JsonString(ValueMap &m);
static string vector2JsonString(ValueVector &m);

class JsonUtils {
public:
    static ValueMap parse(const string &jsonString, bool *success = NULL);
    static bool parse(const string &jsonString, ValueMap &vm);
    
    static string toJsonString(ValueMap &m);
};

NS_SCREW_UTILS_END
#endif /* JSONUTILS_H_ */
