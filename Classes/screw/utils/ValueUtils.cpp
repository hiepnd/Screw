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

#include "ValueUtils.h"
#include "StringUtils.h"
#include <sstream>

NS_SCREW_UTILS_BEGIN

Value ValueGetter::NonConstNull;

#pragma mark Setter
Value &ValueGetter::get(const Value &data, const string &path) {
    vector<string> coms = StringUtils::split(path, "/");
    
    Value* d = const_cast<Value*>(&data);
    for (vector<string>::iterator it = coms.begin(); it != coms.end(); it++) {
        if (d->getType() != Value::Type::MAP) {
            CCLOG("type = %d", d->getType());
        }
        CCASSERT(d->getType() == Value::Type::MAP, (string("Fix me ") + path).c_str());
        CCASSERT(it->length(), "Fix me - empty key is not allowed");
        
        ValueMap& v = d->asValueMap();
        ValueMap::iterator i = v.find(*it);
        
        if (i == v.end()) {
            return ValueGetter::NonConstNull;
        } else {
            d = &i->second;
        }
    }
    
    return *d;
}

int ValueGetter::getInt(const Value &data, const string &path){
    return ValueGetter::get(data, path).asInt();
}

long ValueGetter::getLong(const Value &data, const string &path){
    //Long not yet supported be Value
    string str = ValueGetter::get(data, path).asString();
    return std::atol(str.c_str());
}

float ValueGetter::getFloat(const Value &data, const string &path){
    return ValueGetter::get(data, path).asFloat();
}

double ValueGetter::getDouble(const Value &data, const string &path){
    return ValueGetter::get(data, path).asDouble();
}

string ValueGetter::getString(const Value &data, const string &path){
    return ValueGetter::get(data, path).asString();
}

#pragma mark Setter
void ValueSetter::set(Value& data, const string &path, const Value &value) {
    vector<string> coms = StringUtils::split(string(path), "/");
    
    CCASSERT(coms.size() > 0, "Fix me - path must be something");
    CCASSERT(data.getType() == Value::Type::MAP, "Fix me - data must be a MAP");
    
    ValueMap* d = &data.asValueMap();
    for (int i = 0; i < coms.size() - 1; i++) {
        ValueMap::iterator it = d->find(coms[i]);
        if (it == d->end()) {
            ValueMap m;
            pair<ValueMap::iterator, bool> newI = d->insert(pair<string, Value>(coms[i], Value(m)));
            d = &(newI.first->second.asValueMap());
        } else {
            CCASSERT(it->second.getType() == Value::Type::MAP, "Fix me");
            d = &it->second.asValueMap();
        }
    }
    
    (*d)[coms[coms.size() - 1]] = value;
}

void ValueSetter::set(Value& data, const string &path, int value){
    set(data, path, Value(value));
}

void ValueSetter::set(Value& data, const string &path, long value){
    set(data, path, Value(StringUtils::toString(value)));
}

void ValueSetter::set(Value& data, const string &path, float value){
    set(data, path, Value(StringUtils::toString(value)));
}

void ValueSetter::set(Value& data, const string &path, double value){
    set(data, path, Value(StringUtils::toString(value)));
}

void ValueSetter::set(Value& data, const string &path, bool value){
    set(data, path, Value(value));
}

void ValueSetter::set(Value& data, const string &path, const string& value){
    set(data, path, Value(value));
}

void ValueSetter::clear(Value& data, const string &path){
    vector<string> coms = StringUtils::split(path, "/");
    if (coms.size() >= 1) {
        vector<string> parent;
        for (int i = 0; i < coms.size() - 1; i++) {
            parent.push_back(coms[i]);
        }
        
        string parentPath = StringUtils::join(parent, "/");
        Value& obj = ValueGetter::get(data, parentPath.c_str());
        if (obj.getType() == Value::Type::MAP) {
            ValueMap& m = obj.asValueMap();
            ValueMap::iterator it = m.find(coms[coms.size() - 1]);
            if (it != m.end()) {
                m.erase(it);
            }
        }
    }
}

NS_SCREW_UTILS_END
