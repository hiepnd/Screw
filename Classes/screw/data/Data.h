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

#ifndef _SCREW_DATA_H_
#define _SCREW_DATA_H_

#include "../macros.h"
#include "../utils/ValueUtils.h"
#include "cocos2d.h"

/**
 * Key-value data storage
 */

USING_NS_CC;
USING_NS_SCREW_UTILS;

NS_SCREW_DATA_BEGIN

class PathBuilder : public Object {
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

#endif /* _SCREW_DATA_H_ */
