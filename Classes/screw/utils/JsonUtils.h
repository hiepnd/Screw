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

#ifndef JSONUTILS_H_
#define JSONUTILS_H_

#include "cocos2d.h"
#include <string>
#include "../macros.h"
#include <external/json/rapidjson.h>
#include <external/json/document.h>
#include <external/json/stringbuffer.h>
#include <external/json/writer.h>

using namespace cocos2d;
using namespace std;

NS_SCREW_UTILS_BEGIN

class JsonValueConverter {
public:
    //Handler Implementation {
    JsonValueConverter& Null();
	JsonValueConverter& Bool(bool b);
	JsonValueConverter& Int(int i);
	JsonValueConverter& Uint(unsigned u);
	JsonValueConverter& Int64(int64_t i64);
	JsonValueConverter& Uint64(uint64_t u64);
	JsonValueConverter& Double(double d);
    
	JsonValueConverter& String(const rapidjson::GenericValue<rapidjson::UTF8<>>::Ch* str, rapidjson::SizeType length, bool copy = false);
	JsonValueConverter& StartObject();
	JsonValueConverter& EndObject(rapidjson::SizeType memberCount = 0);
	JsonValueConverter& StartArray();
	JsonValueConverter& EndArray(rapidjson::SizeType elementCount = 0);
    // }
    
    cocos2d::Value &getValue();
private:
    struct StackItem {
        StackItem(bool isMap):index(0), key("") {
            if (isMap) {
                value = Value(ValueMap());
            } else {
                value = Value(ValueVector());
            }
        }
        cocos2d::Value value;
        string key;
        int index;
    };
    
    void value(const cocos2d::Value &v);
    
    list<StackItem> _stack;
};

class ValueJsonStringVisitor {
public:
    static string visit(const cocos2d::Value &value);
    static string visit(const ValueVector &value);
    
    template<typename T>
    static string visitMap(const T &v) {
        std::stringstream ret;
        ret << "{";
        int i = v.size();
        for (auto iter = v.begin(); iter != v.end(); ++iter) {
            ret << "\"" << iter->first << "\"" << ":";
            ret << visit(iter->second);
            if (--i)
                ret << ",";
        }
        ret << "}";
        return ret.str();
    }
};

class JsonUtils {
public:
    static cocos2d::Value parse(const string &jsonString, bool *success = NULL);
    static bool parse(const string &jsonString, ValueMap &m);
    
    static string toJsonString(ValueMap &m);
    static string toJsonString(ValueVector &v);
    static string toJsonString(cocos2d::Value &v);
    
    static cocos2d::Value json2Value(rapidjson::Value &json);    
};

NS_SCREW_UTILS_END
#endif /* JSONUTILS_H_ */
