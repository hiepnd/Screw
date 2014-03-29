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

	virtual void open(bool allowUi , const list<string> &permission) override;
	virtual void close() override;
	virtual void requestReadPermissions(const list<string> &permission) override;
	virtual void requestPublishPermissions(const list<string> &permission) override;

};

} /* namespace jni */

#endif /* FACEBOOK_H_ */
