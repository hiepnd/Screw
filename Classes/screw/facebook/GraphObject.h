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

/** Macros */

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
 * GO_SET_GRAPH_OBJECT(GraphUser, User, "from") ==> void setUser(GraphUser *user) {....}
 * Setting nullptr resulted in clearing property
 */
#define GO_SET_GRAPH_OBJECT(class, name, path) \
            void set##name(class *value) { \
                if (value) { \
                    set(path, value->getValue()); \
                } else { \
                    clear(path); \
                } \
            }


#define GO_PROPERTY_OBJECT(class, name, path) \
            GO_GET_GRAPH_OBJECT(class, name, path); \
            GO_SET_GRAPH_OBJECT(class, name, path);

/**
 * GO_GET_GRAPH_OBJECT_LIST(GraphUser, Friends, "data") ==> Vector<GraphUser *> getFriends() {...}
 */
#define GO_GET_GRAPH_OBJECT_LIST(class, name, path) \
            Vector<class *> get##name() { \
                return getPropertyAsList<class>(path);\
            }

/**
 * GO_SET_GRAPH_OBJECT_LIST(GraphUser, Friends, "data") ==> void setFriends(Vector<GraphUser *>) {...}
 */
#define GO_SET_GRAPH_OBJECT_LIST(class, name, path) \
            void set##name(const Vector<class *> &l) { \
                ValueVector vv; \
                for (auto i : l) { \
                    vv.push_back(i->getValue());\
                } \
                _data.asValueMap()[path] = vv; \
            }

#define GO_PROPERTY_GRAPH_OBJECT_LIST(class, name, path) \
            GO_GET_GRAPH_OBJECT_LIST(class, name, path); \
            GO_SET_GRAPH_OBJECT_LIST(class, name, path); 


#define GO_HAS_PROPERTY(name, path) \
            bool has##name() { \
                return hasProperty(path);   \
            } \

#define GO_CREATE(class) \
            static class *create(const Value &v) { \
                class *obj = new class(); \
                obj->init(v); \
                obj->autorelease(); \
                return obj; \
            }

#define GO_CREATE_EMPTY(class) \
            static class *create() { \
                class *obj = new class(); \
                obj->init(Value(ValueMap())); \
                obj->autorelease(); \
                return obj; \
            }

/* Wrapper of a cocos2d::Value. Provide convenient getters/setters */

class GraphObject : public Object {
public:
    GO_CREATE(GraphObject);
	GraphObject();
	virtual ~GraphObject();
	virtual bool init(const Value &data);

    bool hasProperty(const string &name);
    
    Value &getValue();
	Value &get(const string &path);
    
    bool    getBool(const string &path);
    int     getInt(const string &path);
	long    getLong(const string &path);
	float   getFloat(const string &path);
	double  getDouble(const string &path);
	string  getString(const string &path);

    void set(const string &path, bool value);
    void set(const string &path, int value);
    void set(const string &path, long value);
    void set(const string &path, const string &value);
    void set(const string &path, const Value &value);
    
    void clear(const string &path);
    
    template<class From, class To>
    static To *castTo(From *from) {
        Value &v = from->getValue();
        To *t = new To();
        t->init(v);
        t->autorelease();
        return t;
    }
    
	template<class T=GraphObject>
	T *getProperty(const string &name) {
		Value &v = ValueGetter::get(_data, name);
        if (!v.isNull()) {
            T *obj = new T();
            obj->init(v);
            obj->autorelease();
            return obj;
        }
        return nullptr;
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
    
    Value &operator[](const string &path) {
        return _data.asValueMap()[path];
    }
    
protected:
	Value _data;
};

class GraphUser : public GraphObject {
public:
    GO_CREATE(GraphUser);
    
    GO_GET_STRING(Id, ID);
    GO_GET_STRING(Name, NAME);
    GO_GET_STRING(FirstName, FIRST_NAME);
    GO_GET_STRING(LastName, LAST_NAME);
    GO_GET_LONG(Score, SCORE);
    
    bool isInstalled() {
        return hasProperty(INSTALLED);
    }
    
    static const string ID;
    static const string NAME;
    static const string FIRST_NAME;
    static const string LAST_NAME;
    static const string SCORE;
    static const string INSTALLED;
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
    GO_GET_GRAPH_OBJECT(GraphObject, DataObject, "data");
};


NS_SCREW_FACEBOOK_END

#endif /* GRAPHOBJECT_H_ */
