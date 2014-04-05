/*
 * GraphObject.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

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

Value &GraphObject::getData() {
    return _data;
}

Value &GraphObject::get(const string &path) {
    return ValueGetter::get(_data, path);
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

void GraphObject::set(const string &path, long value) {
    ValueSetter::set(_data, path, value);
}

void GraphObject::set(const string &path, const string &value) {
    ValueSetter::set(_data, path, value);
}

void GraphObject::set(const string &path, const Value &value) {
    ValueSetter::set(_data, path, value);
}

NS_SCREW_FACEBOOK_END