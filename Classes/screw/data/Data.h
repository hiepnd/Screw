/*
 * Data.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef DATA_H_
#define DATA_H_
#include "../macros.h"
#include "../utils/ObjectUtils.h"
#include "cocos2d.h"

/**
 * Key-value data storage
 */

USING_NS_CC;
USING_NS_SCREW_UTILS;

NS_SCREW_DATA_BEGIN

class Data : public Object{
public:
	Data();
	virtual ~Data();
};

NS_SCREW_DATA_END

#endif /* DATA_H_ */
