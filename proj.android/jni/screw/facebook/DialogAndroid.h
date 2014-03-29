/*
 * DialogAndroid.h
 *
 *  Created on: Mar 28, 2014
 *      Author: hiepnd
 */

#ifndef DIALOGANDROID_H_
#define DIALOGANDROID_H_

#include "screw/facebook/Dialog.h"
#include "Helper.h"
#include "cocos2d.h"
#include <map>

USING_NS_SCREW_FACEBOOK;
USING_NS_CC;
using namespace std;

namespace jni {

class DialogAndroid : public DialogImpl{
public:
	DialogAndroid();
	virtual ~DialogAndroid();

	virtual void show(Dialog *dialog);
	static void onDialogComplete(long requestCode, int error, const string &errorMessage, const string &requestId, const list<string> &toes);

private:
	static map<int, DialogCallback> _callbacks;
	static long _requestCode;
};

} /* namespace jni { */
#endif /* DIALOGANDROID_H_ */
