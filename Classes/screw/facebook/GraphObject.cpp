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

#include "GraphObject.h"

NS_SCREW_FACEBOOK_BEGIN

const string GraphUser::ID("id");
const string GraphUser::NAME("name");
const string GraphUser::FIRST_NAME("first_name");
const string GraphUser::LAST_NAME("last_name");
const string GraphUser::SCORE("score");
const string GraphUser::INSTALLED("installed");

GraphObject::GraphObject():
_data(Value(ValueMap()))
{
	// TODO Auto-generated constructor stub

}

GraphObject::~GraphObject() {
	// TODO Auto-generated destructor stub
}

bool GraphObject::init(const cocos2d::Value &data) {
//    CCASSERT(data.getType() == Value::Type::MAP || data.getType() == Value::Type::VECTOR, "Fix me");
    _data = data;
    return true;
}

bool GraphObject::hasProperty(const string &name) {
    return !get(name).isNull();
}

Value &GraphObject::getValue() {
    return _data;
}

Value &GraphObject::get(const string &path) {
    return ValueGetter::get(_data, path);
}

bool GraphObject::getBool(const string &path) {
    return ValueGetter::getBool(_data, path);
}

int GraphObject::getInt(const string &path) {
    return ValueGetter::getInt(_data, path);
}

long GraphObject::getLong(const string &path) {
    return ValueGetter::getLong(_data, path);
}

float GraphObject::getFloat(const string &path) {
    return ValueGetter::getFloat(_data, path);
}

double GraphObject::getDouble(const string &path) {
    return ValueGetter::getDouble(_data, path);
}

string GraphObject::getString(const string &path) {
    return ValueGetter::getString(_data, path);
}

void GraphObject::set(const string &path, int value) {
    ValueSetter::set(_data, path, value);
}

void GraphObject::set(const string &path, bool value) {
    ValueSetter::set(_data, path, value);
}

void GraphObject::set(const string &path, long value) {
    ValueSetter::set(_data, path, value);
}

void GraphObject::set(const string &path, const string &value) {
    ValueSetter::set(_data, path, value);
}

void GraphObject::set(const string &path, const Value &value) {
    ValueSetter::set(_data, path, value);
}

void GraphObject::clear(const string &path) {
    ValueSetter::clear(_data, path);
}

NS_SCREW_FACEBOOK_END