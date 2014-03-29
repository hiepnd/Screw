/*
 * Data.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef DATA_H_
#define DATA_H_
#include "../macros.h"
#include "../utils/ValueUtils.h"
#include "cocos2d.h"

/**
 * Key-value data storage
 */

USING_NS_CC;
USING_NS_SCREW_UTILS;

NS_SCREW_DATA_BEGIN

class PathBuilder: Object {
    string _internal;
    string _seprator;
public:
    PathBuilder(const string &s);
    static PathBuilder *create(const string &s);
    
    PathBuilder *append(const string &s);
    PathBuilder *setSeprator(const string &sep);
    string &build();
};

class Data : public Object{
public:
	Data();
	Data(const string &path);
    Data(const ValueMap& data);
    Data(const ValueMap& data, const string &path);
	virtual ~Data();
    
   	virtual void save();
    virtual void save(const string &path);
    
	virtual void set(const string &path, const Value &value);
	virtual Value &get(const string &path);
    virtual void clear(const string &path);
    
	int     getInt(const string &path);
	long    getLong(const string &path);
	float   getFloat(const string &path);
	double  getDouble(const string &path);
	string  getString(const string &path);
    
	void set(const string &path, int value);
    void set(const string &path, long value);
    void set(const string &path, float value);
    void set(const string &path, double value);
    void set(const string &path, bool value);
    void set(const string &path, const string& value);
    
    void print();
    
protected:
    string _path;
	Value _data;
    
};

NS_SCREW_DATA_END

#endif /* DATA_H_ */
