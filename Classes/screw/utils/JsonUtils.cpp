/*
 * JsonUtils.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "JsonUtils.h"

using namespace rapidjson;

static void __add_json_to_map(ValueMap &m, GenericValue<UTF8<>> &data);
static void __add_json_to_map(ValueMap &m, GenericValue<UTF8<>> &data);

NS_SCREW_UTILS_BEGIN

bool JsonUtils::parse(const string &jsonString, ValueMap &vm) {
    rapidjson::Document document;
    document.Parse<0>(jsonString.c_str());
    if (document.HasParseError()) {
        return false;
    }
    
    if (!document.IsObject()) {
        return false;
    }
    
    __add_json_to_map(vm, document);
    
    return true;
}

ValueMap JsonUtils::parse(const string &jsonString, bool *success) {
    ValueMap m;
    bool ok = parse(jsonString, m);
    if (success) {
        *success = ok;
    }
    return m;
}

string JsonUtils::toJsonString(ValueMap &m) {
    return map2JsonString(m);
}

static string map2JsonString(ValueMap &m) {
    string s("{");
    int size = m.size();
    for (auto i : m) {
        switch (i.second.getType()) {
            case cocos2d::Value::Type::MAP:
            {
                
                s.append("\"").append(i.first).append("\": ");
                s.append(map2JsonString(i.second.asValueMap()));
                if (--size)
                    s.append(",");
                break;
            }
            case cocos2d::Value::Type::VECTOR:
            {
                s.append("\"").append(i.first).append("\": ").append(vector2JsonString(i.second.asValueVector()));
                if (--size)
                    s.append(",");
                break;
            }
            default:
            {
                s.append("\"").append(i.first).append("\": ");
                s.append("\"").append(i.second.asString()).append("\"");
                if (--size)
                    s.append(",");
                break;
            }
        }
    }
    s.append("}");
    
    return s;
}

static string vector2JsonString(ValueVector &m) {
    string s("[");
    int size = m.size();
    for (auto i : m) {
        switch (i.getType()) {
            case cocos2d::Value::Type::MAP:
            {
                s.append(map2JsonString(i.asValueMap()));
                if (--size)
                    s.append(",");
                break;
            }
            case cocos2d::Value::Type::VECTOR:
            {
                s.append(screw::utils::vector2JsonString(i.asValueVector()));
                if (--size)
                    s.append(",");
                break;
            }
            default:
            {
                s.append(i.asString());
                if (--size)
                    s.append(",");
                break;
            }
        }
    }
    s.append("]");
    
    return s;
}

NS_SCREW_UTILS_END

#pragma mark OOO
static void __add_json_to_vector(ValueVector &vector, GenericValue<UTF8<>> &data) {
    for (int i = 0; i < data.Size(); i++) {
        GenericValue<UTF8<>> &v = data[i];
        
        if (v.IsArray()) {
            ValueVector vector;
            __add_json_to_vector(vector, v);
            vector.push_back(cocos2d::Value(vector));
        } else if (v.IsObject()) {
            ValueMap m;
            __add_json_to_map(m, v);
            vector.push_back(cocos2d::Value(m));
        } else if (v.IsString()) {
            vector.push_back(cocos2d::Value(v.GetString()));
        } else if (v.IsInt()) {
            vector.push_back(cocos2d::Value(v.GetInt()));
        } else if (v.IsUint()) {
            vector.push_back(cocos2d::Value((int)v.GetUint()));
        } else if (v.IsInt64()) {
            CCLOG("__add_json_to_vector - treat long as int");
            vector.push_back(cocos2d::Value((int)v.GetInt64()));
        } else if (v.IsUint64()) {
            CCLOG("__add_json_to_vector - treat long as int");
            vector.push_back(cocos2d::Value((int)v.GetUint64()));
        } else if (v.IsDouble()) {
            vector.push_back(cocos2d::Value(v.GetDouble()));
        } else if (v.IsBool()) {
            vector.push_back(cocos2d::Value(v.GetBool()));
        } else {
            CCLOG("__add_json_to_vector - non supported type");
        }
    }
}

static void __add_json_to_map(ValueMap &m, GenericValue<UTF8<>> &data) {
    for (auto it = data.MemberonBegin(); it != data.MemberonEnd(); it++) {
        GenericValue<UTF8<>> &v = it->value;
        
        if (v.IsArray()) {
            ValueVector vector;
            __add_json_to_vector(vector, v);
            m[it->name.GetString()] = cocos2d::Value(vector);
        } else if (v.IsObject()) {
            ValueMap child;
            __add_json_to_map(child, v);
            m[it->name.GetString()] = cocos2d::Value(child);
        } else if (v.IsString()) {
            m[it->name.GetString()] = cocos2d::Value(v.GetString());
        } else if (v.IsInt()) {
            m[it->name.GetString()] = cocos2d::Value(v.GetInt());
        } else if (v.IsUint()) {
            m[it->name.GetString()] = cocos2d::Value((int)v.GetUint());
        } else if (v.IsInt64()) {
            CCLOG("__add_json_to_vector - treat long as int");
            m[it->name.GetString()] = cocos2d::Value((int)v.GetInt64());
        } else if (v.IsUint64()) {
            CCLOG("__add_json_to_vector - treat long as int");
            m[it->name.GetString()] = cocos2d::Value((int)v.GetUint64());
        } else if (v.IsDouble()) {
            m[it->name.GetString()] = cocos2d::Value(v.GetDouble());
        } else if (v.IsBool()) {
            m[it->name.GetString()] = cocos2d::Value(v.GetBool());
        } else {
            CCLOG("__add_json_to_map - non supported type");
        }
    }
}