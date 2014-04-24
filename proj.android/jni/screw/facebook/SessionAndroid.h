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

#ifndef FACEBOOK_H_
#define FACEBOOK_H_

#include "Helper.h"

USING_NS_SCREW;
USING_NS_SCREW_FACEBOOK;
using namespace std;

NS_SCREW_JNI_BEGIN

class SessionAndroid : public SessionImpl{
public:
	SessionAndroid();
	virtual ~SessionAndroid();

	virtual void open(bool allowUi, const list<string> &permissions, DefaultAudience defaultAudience, LoginBehavior loginBehavior) override;
	virtual void close() override;
	virtual void requestReadPermissions(const list<string> &permission) override;
	virtual void requestPublishPermissions(const list<string> &permission) override;

	static void start();
};

NS_SCREW_JNI_END /* namespace jni */

#endif /* FACEBOOK_H_ */
