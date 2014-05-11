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

#ifndef _SCREW_VALUEUTILS_H_
#define _SCREW_VALUEUTILS_H_

#include "../macros.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

NS_SCREW_UTILS_BEGIN

class VectorBuilder : public Object {
public:
    
    VectorBuilder *add(const string &value);
    ValueVector &build();
    
private:
    ValueVector _internal;
};

class ValueGetter {
public:
	static Value &get(const Value &data, const string &path);
    static Value &get(const Value &data, const vector<string> &paths);
    
    static bool getBool(const Value &data, const string &path);
	static int getInt(const Value &data, const string &path);
	static long getLong(const Value &data, const string &path);
	static float getFloat(const Value &data, const string &path);
	static double getDouble(const Value &data, const string &path);
	static string getString(const Value &data, const string &path);
    
private:
    static Value NonConstNull;
};

class ValueSetter {
public:
    static void set(Value& data, const string &path, const Value &value);
    static void set(Value& data, const string &path, int value);
    static void set(Value& data, const string &path, long value);
    static void set(Value& data, const string &path, float value);
    static void set(Value& data, const string &path, double value);
    static void set(Value& data, const string &path, bool value);
    static void set(Value& data, const string &path, const string& value);
    static void set(Value& data, const string &path, const char *value);
    
    static void clear(Value& data, const string &path);
};


NS_SCREW_UTILS_END

#endif /* _SCREW_VALUEUTILS_H_ */
