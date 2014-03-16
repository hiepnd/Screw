/*
 * GraphObject.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "GraphObject.h"

NS_SCREW_FACEBOOK_BEGIN

GraphObject::GraphObject() {
	// TODO Auto-generated constructor stub

}

GraphObject::~GraphObject() {
	// TODO Auto-generated destructor stub
}

bool GraphObject::init(const cocos2d::Value &data) {
    CCASSERT(data.getType() == Value::Type::MAP, "Fix me");
    _data = data;
    return true;
}

NS_SCREW_FACEBOOK_END