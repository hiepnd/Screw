/*
 * GraphObject.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef GRAPHOBJECT_H_
#define GRAPHOBJECT_H_
#include "../macros.h"
#include "../utils/ObjectUtils.h"
#include "cocos2d.h"

USING_NS_CC;
USING_NS_SCREW_UTILS;
using namespace std;

NS_SCREW_FACEBOOK_BEGIN

class GraphObject : public Object {
public:
	GraphObject();
	virtual ~GraphObject();
	virtual bool init(const Value &data);

	Value &get(const string &path);

	template<class T=GraphObject>
	T *getProperty(const string &name) {
		Value &v = DictGetter::get(_data, name);
		T *obj = new T();
		obj->init(v);
		obj->autorelease();
		return obj;
	}

	template<class T>
	Vector<T *> getPropertyAsList(const string &name) {
		Vector<T *> ret;
		Value &v = DictGetter::get(_data, name);
		if (v.getType() == Value::Type::VECTOR) {
			auto vector = v.asValueVector();
			for (auto i = vector.begin(); i != vector.end(); i++) {
				T *obj = new T();
				obj->init(*i);
				ret.pushBack(obj);
				obj->release();
			}
		}
		return ret;
	}

private:
	Value _data;
};

class GraphUser : public GraphObject {

};

class GraphScore : public GraphObject {

};

class GraphRequest : public GraphObject {

};


NS_SCREW_FACEBOOK_END

#endif /* GRAPHOBJECT_H_ */
