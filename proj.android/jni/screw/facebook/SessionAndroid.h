/*
 * Facebook.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef FACEBOOK_H_
#define FACEBOOK_H_
#include "screw/facebook/Session.h"

USING_NS_SCREW_FACEBOOK;
using namespace std;

namespace jni {

class SessionAndroid : public SessionImpl{
public:
	SessionAndroid();
	virtual ~SessionAndroid();

	virtual void open() override;
	virtual void close() override;

};

} /* namespace jni */
#endif /* FACEBOOK_H_ */
