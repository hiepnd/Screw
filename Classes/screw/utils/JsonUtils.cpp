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

#include "JsonUtils.h"
#include "StringUtils.h"

using namespace rapidjson;

static void __add_json_to_map(ValueMap &m, GenericValue<UTF8<>> &data);
static void __add_json_to_map(ValueMap &m, GenericValue<UTF8<>> &data);

NS_SCREW_UTILS_BEGIN

#pragma mark JsonValueConverter
JsonValueConverter& JsonValueConverter::Null() {
    return *this;
}

JsonValueConverter& JsonValueConverter::Bool(bool b) {
    this->value(cocos2d::Value(b));
    return *this;
}

JsonValueConverter& JsonValueConverter::Int(int i) {
    this->value(cocos2d::Value(i));
    return *this;
}

JsonValueConverter& JsonValueConverter::Uint(unsigned u) {
    this->value(cocos2d::Value((int)u));
    return *this;
}

JsonValueConverter& JsonValueConverter::Int64(int64_t i64) {
    this->value(cocos2d::Value(StringUtils::toString((long)i64)));
    return *this;
}

JsonValueConverter& JsonValueConverter::Uint64(uint64_t u64) {
    this->value(cocos2d::Value(StringUtils::toString((long)u64)));
    return *this;
}

JsonValueConverter& JsonValueConverter::Double(double d) {
    this->value(cocos2d::Value(d));
    return *this;
}

JsonValueConverter& JsonValueConverter::String(const GenericValue<UTF8<>>::Ch* str, SizeType length, bool copy) {
    string v(str, length);
    this->value(cocos2d::Value(v));
    return *this;
}

JsonValueConverter& JsonValueConverter::StartObject() {
    _stack.push_back(StackItem(true));
    return *this;
}

JsonValueConverter& JsonValueConverter::EndObject(SizeType memberCount) {
    CCASSERT(_stack.back().value.getType() == cocos2d::Value::Type::MAP, "Something wrong here");
    //Keep the first object on stack
    if (_stack.size() > 1) {
        StackItem item = _stack.back();
        _stack.pop_back();
        this->value(item.value);
    }
    return *this;
}

JsonValueConverter& JsonValueConverter::StartArray() {
    _stack.push_back(StackItem(false));
    return *this;
}

JsonValueConverter& JsonValueConverter::EndArray(SizeType elementCount) {
    CCASSERT(_stack.back().value.getType() == cocos2d::Value::Type::VECTOR, "Something wrong here");
    if (_stack.size() > 1) {
        StackItem item = _stack.back();
        _stack.pop_back();
        this->value(item.value);
    }
    return *this;
}

void JsonValueConverter::value(const cocos2d::Value &v) {
    StackItem &current = _stack.back();
    if (current.value.getType() == cocos2d::Value::Type::MAP) {
        if (current.index %2 == 0) {
            CCASSERT(v.getType() == cocos2d::Value::Type::STRING, "Key must be a string");
            current.key = v.asString();
        } else {
            current.value.asValueMap()[current.key] = v;
        }
        current.index++;
    } else {
        current.value.asValueVector().push_back(v);
    }
}

cocos2d::Value &JsonValueConverter::getValue() {
    CCASSERT(_stack.size() == 1, (string("Must be 1 - got ") + utils::StringUtils::toString((long)_stack.size())).c_str());
    return _stack.front().value;
}

#pragma mark ValueJsonStringVisitor
string ValueJsonStringVisitor::visit(const cocos2d::Value &v) {
    switch (v.getType())
    {
        case cocos2d::Value::Type::NONE:
        case cocos2d::Value::Type::BYTE:
        case cocos2d::Value::Type::INTEGER:
        case cocos2d::Value::Type::FLOAT:
        case cocos2d::Value::Type::DOUBLE:
        case cocos2d::Value::Type::BOOLEAN:
            return v.asString();
            break;
        case cocos2d::Value::Type::STRING:
            return string("\"") + v.asString() + string("\"");
            break;
        case cocos2d::Value::Type::VECTOR:
            return visit(v.asValueVector());
            break;
        case cocos2d::Value::Type::MAP:
            return visitMap<ValueMap>(v.asValueMap());
            break;
        case cocos2d::Value::Type::INT_KEY_MAP:
            return visitMap<ValueMapIntKey>(v.asIntKeyMap());
            break;
        default:
            CCASSERT(false, "Invalid type!");
            break;
    }
}

string ValueJsonStringVisitor::visit(const ValueVector &v) {
    std::stringstream ret;
    
    ret << "[";
    int i = v.size();
    for (auto& child : v){
        ret << visit(child);
        if (--i)
            ret << ",";
    }
    ret << "]";
    
    return ret.str();
}

#pragma mark JSON to CC
cocos2d::Value JsonUtils::json2Value(rapidjson::Value &json) {
    JsonValueConverter converter;
    json.Accept(converter);
    return converter.getValue();
}

cocos2d::Value JsonUtils::parse(const string &jsonString, bool *success) {
    rapidjson::Document document;
    document.Parse<0>(jsonString.c_str());
    if (document.HasParseError()) {
        if (success) {
            *success = false;
        }
        return cocos2d::Value();
    }
    
    if (success) {
        *success = true;
    }
    
    JsonValueConverter converter;
    document.Accept(converter);
    return converter.getValue();
}

bool JsonUtils::parse(const string &jsonString, ValueMap &m) {
    bool ok = false;
    auto ret = parse(jsonString, &ok);
    if (ret.getType() == cocos2d::Value::Type::MAP) {
        m = ret.asValueMap();
    }
    
    return ok;
}

string JsonUtils::toJsonString(ValueMap &m) {
    return ValueJsonStringVisitor::visitMap(m);
}

string JsonUtils::toJsonString(cocos2d::Value &v) {
    CCASSERT(v.getType() == cocos2d::Value::Type::INT_KEY_MAP || v.getType() == cocos2d::Value::Type::MAP ||
             v.getType() == cocos2d::Value::Type::VECTOR, "Only map and vector are valid");
    return ValueJsonStringVisitor::visit(v);
}

string JsonUtils::toJsonString(ValueVector &v) {
    return ValueJsonStringVisitor::visit(v);
}

NS_SCREW_UTILS_END