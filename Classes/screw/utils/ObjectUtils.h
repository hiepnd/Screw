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

class DictGetter {
public:
	static Value &get(Value &data, const string &path);

	static int getInt(Value &data, const string &path);
	static long getLong(Value &data, const string &path);
	static float getFloat(Value &data, const string &path);
	static double getDouble(Value &data, const string &path);
	static string getString(Value &data, const string &path);
};

class DictSetter {
public:
	static void set(Value &data, const string &path, Value &value);
};


NS_SCREW_UTILS_END

#endif /* OBJECTUTILS_H_ */
