/*
 * ObjectUtils.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef OBJECTUTILS_H_
#define OBJECTUTILS_H_

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
    
    static void clear(Value& data, const string &path);
};


NS_SCREW_UTILS_END

#endif /* OBJECTUTILS_H_ */
