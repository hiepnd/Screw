/*
 * Data.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "Data.h"
#include "../utils/ValueUtils.h"

NS_SCREW_DATA_BEGIN

#pragma mark Path Builder
PathBuilder::PathBuilder(const string &s):_internal(s), _seprator("/") {
    
}

PathBuilder *PathBuilder::create(const string &s) {
    PathBuilder *pb = new PathBuilder(s);
    pb->autorelease();
    return pb;
}

PathBuilder *PathBuilder::append(const string &s) {
    _internal.append(_seprator).append(s);
    return this;
}

PathBuilder *PathBuilder::setSeprator(const string &sep) {
    _seprator = sep;
    return this;
}

string &PathBuilder::build() {
    return _internal;
}

#pragma mark Data
Data::Data(): _data(Value(ValueMap())) {
    
}

Data::Data(const string &path):_path(path) {
    _data = Value(cocos2d::FileUtils::getInstance()->getValueMapFromFile(_path));
}

Data::Data(const ValueMap& data): _data(data) {
    
}

Data::Data(const ValueMap& data, const string &path):_path(path) {
    if (cocos2d::FileUtils::getInstance()->isFileExist(_path)) {
        _data = Value(cocos2d::FileUtils::getInstance()->getValueMapFromFile(_path));
    } else {
        _data = Value(ValueMap());
    }
}

Data::~Data() {
    
}

#pragma mark Getter
Value &Data::get(const string &path) {
    return utils::ValueGetter::get(_data, path);
}

int Data::getInt(const string &path) {
    return utils::ValueGetter::getInt(_data, path);
}

long Data::getLong(const string &path) {
    return utils::ValueGetter::getLong(_data, path);
}

float Data::getFloat(const string &path) {
    return utils::ValueGetter::getFloat(_data, path);
}

double Data::getDouble(const string &path) {
    return utils::ValueGetter::getDouble(_data, path);
}

string Data::getString(const string &path) {
    return utils::ValueGetter::getString(_data, path);
}

#pragma mark Setter
void Data::set(const string &path, const Value &value) {
    utils::ValueSetter::set(_data, path, value);
}

void Data::set(const string &path, int value) {
    utils::ValueSetter::set(_data, path, value);
}

void Data::set(const string &path, long value) {
    utils::ValueSetter::set(_data, path, value);
}

void Data::set(const string &path, float value) {
    utils::ValueSetter::set(_data, path, value);
}

void Data::set(const string &path, double value) {
    utils::ValueSetter::set(_data, path, value);
}

void Data::set(const string &path, bool value) {
    utils::ValueSetter::set(_data, path, value);
}

void Data::set(const string &path, const string& value) {
    utils::ValueSetter::set(_data, path, value);
}

void Data::clear(const string &path) {
    utils::ValueSetter::clear(_data, path);
}

void Data::save() {
    this->save(_path);
}

void Data::print() {
    printf("Data at %s:\n", _path.c_str());
    printf("%s\n", _data.getDescription().c_str());
}

#pragma mark Save
void Data::save(const string &path) {
    cocos2d::FileUtils::getInstance()->writeToFile(_data.asValueMap(), _path);
}

NS_SCREW_DATA_END