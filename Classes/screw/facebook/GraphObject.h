/*
 * GraphObject.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef GRAPHOBJECT_H_
#define GRAPHOBJECT_H_

#include "../macros.h"
#include "../utils/ValueUtils.h"
#include "../utils/ValueUtils.h"
#include "cocos2d.h"

USING_NS_CC;
USING_NS_SCREW_UTILS;
using namespace std;

NS_SCREW_FACEBOOK_BEGIN

/** Macroes */
/**
 * Implement getter for string
 * GO_GET_STRING(Name, "name") ==> string getName() {....}
 */
#define GO_GET_STRING(name, path) \
            string get##name () { \
                return ValueGetter::getString(_data, path); \
            }

/**
 * Implement setter for string
 * GO_GET_STRING(Name, "name") ==> string getName() {....}
 */
#define GO_SET_STRING(name, path) \
            void set##name (const string &value) { \
                ValueSetter::set(_data, path, value); \
            }

#define GO_PROPERTY_STRING(name, path) \
            GO_GET_STRING(name, path); \
            GO_SET_STRING(name, path);

/** Int property */
#define GO_GET_INT(name, path) \
            int get##name () { \
                return ValueGetter::getInt(_data, path); \
            }

#define GO_SET_INT(name, path) \
            void set##name (int value) { \
                ValueSetter::set(_data, path, value); \
            }

#define GO_PROPERTY_INT(name, path) \
            GO_GET_INT(name, path); \
            GO_SET_INT(name, path);


/** Bool property */
#define GO_GET_BOOL(name, path) \
            bool get##name () { \
                return ValueGetter::getBool(_data, path); \
            }

#define GO_SET_BOOL(name, path) \
            void set##name (bool value) { \
                ValueSetter::set(_data, path, value); \
            }

#define GO_PROPERTY_BOOL(name, path) \
            GO_GET_BOOL(name, path); \
            GO_SET_BOOL(name, path);

/** LONG property */
#define GO_GET_LONG(name, path) \
            long get##name () { \
                return ValueGetter::getLong(_data, path); \
            }

#define GO_SET_LONG(name, path) \
            void set##name (long value) { \
                ValueSetter::set(_data, path, value); \
            }

#define GO_PROPERTY_LONG(name, path) \
            GO_GET_LONG(name, path); \
            GO_SET_LONG(name, path);

/**
 * GO_GET_GRAPH_OBJECT(GraphUser, User, "from") ==> GraphUser *getUser() {....}
 */
#define GO_GET_GRAPH_OBJECT(class, name, path) \
            class *get##name() { \
                return getProperty<class>(path); \
            }

/**
 * GO_GET_GRAPH_OBJECT_LIST(GraphUser, Friends, "data") ==> Vector<GraphUser *> getFriends() {...}
 */
#define GO_GET_GRAPH_OBJECT_LIST(class, name, path) \
            Vector<class *> get##name() { \
                return getPropertyAsList<class>(path);\
            }

#define GO_CREATE(class) \
            static class *create(const Value &v) { \
                class *obj = new class(); \
                obj->init(v); \
                obj->autorelease(); \
                return obj; \
            }

class GraphObject : public Object {
public:
    GO_CREATE(GraphObject);
	GraphObject();
	virtual ~GraphObject();
	virtual bool init(const Value &data);

    bool hasProperty(const string &name);
    
    Value &getData();
	Value &get(const string &path);
    int     getInt(const string &path);
	long    getLong(const string &path);
	float   getFloat(const string &path);
	double  getDouble(const string &path);
	string  getString(const string &path);

    void set(const string &path, int value);
    void set(const string &path, long value);
    void set(const string &path, const string &value);
    void set(const string &path, const Value &value);
    
    template<class From, class To>
    static To *castTo(From *from) {
        Value &v = from->getData();
        To *t = new To();
        t->init(v);
        t->autorelease();
        return t;
    }
    
	template<class T=GraphObject>
	T *getProperty(const string &name) {
		Value &v = ValueGetter::get(_data, name);
		T *obj = new T();
		obj->init(v);
		obj->autorelease();
		return obj;
	}

	template<class T=GraphObject>
	Vector<T *> getPropertyAsList(const string &name) {
		Vector<T *> ret;
		Value &v = ValueGetter::get(_data, name);
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
    
    // ???
    Value &operator[](const string &path) {
        return _data.asValueMap()[path];
    }
    
    void test() {
        (*this)["name"] = "hiepnd";
        (*this)["age"] = 31;
    }
    
protected:
	Value _data;
};

class GraphUser : public GraphObject {
public:
    GO_CREATE(GraphUser);
    
    GO_GET_STRING(Id, "id");
    GO_GET_STRING(Name, "name");
    GO_GET_STRING(FirstName, "first_name");
    GO_GET_STRING(LastName, "last_name");
    GO_GET_LONG(Score, "score");
    
    bool isInstalled() {
        return hasProperty("installed");
    }
};

class GraphApplication : public GraphObject {
public:
    GO_CREATE(GraphApplication);
    
    GO_GET_STRING(Id, "id");
    GO_GET_STRING(Name, "name");
    GO_GET_STRING(Namespace, "namespace");
};

class GraphScore : public GraphObject {
public:
    GO_CREATE(GraphScore);
    
    GO_GET_GRAPH_OBJECT(GraphUser, User, "user");
    GO_GET_GRAPH_OBJECT(GraphApplication, Application, "application");
    GO_GET_LONG(Score, "score");
};

class GraphRequest : public GraphObject {
public:
    GO_CREATE(GraphRequest);
    
    GO_GET_STRING(Id, "id");
    GO_GET_GRAPH_OBJECT(GraphUser, To, "to");
    GO_GET_GRAPH_OBJECT(GraphUser, From, "from");
    GO_GET_GRAPH_OBJECT(GraphApplication, Application, "application");
    GO_GET_STRING(DataString, "data");
    GO_GET_STRING(Message, "message");
};


NS_SCREW_FACEBOOK_END

#endif /* GRAPHOBJECT_H_ */
