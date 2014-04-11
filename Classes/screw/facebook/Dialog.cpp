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

#include "Dialog.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "DialogApple.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/screw/facebook/DialogAndroid.h"
#endif


NS_SCREW_FACEBOOK_BEGIN

Vector<Dialog *> Dialog::_dialogs;

Dialog::Dialog(): _dialog(""), _params(ValueMap()), _callback(nullptr) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _impl = new screw::ios::DialogApple();
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _impl = new screw::jni::DialogAndroid();
#endif
}

Dialog::~Dialog() {
    delete _impl;
}

Dialog *Dialog::create(const string &dialog, const ValueMap &params, const DialogCallback &callback) {
    Dialog *d = new Dialog(dialog, params, callback);
    d->autorelease();
    return d;
}

Dialog::Dialog(const string &dialog, const ValueMap &params, const DialogCallback &callback):
Dialog()
{
    this->setDialog(dialog);
    this->setParams(params);
    this->setCallback(callback);
}

const DialogCallback &Dialog::getCallback() const {
	return _callback;
}

void Dialog::setCallback(const DialogCallback &callback) {
	_callback = callback;
}

const string &Dialog::getDialog() const {
	return _dialog;
}

void Dialog::setDialog(const string &dialog) {
	_dialog = dialog;
}

ValueMap &Dialog::getParams() {
	return _params;
}

void Dialog::setParams(const ValueMap &params) {
	_params = params;
}

void Dialog::show() {
    FB_LOG("Dialog::show - showing %ld dialog(s)", _dialogs.size());
    CCASSERT(!_dialogs.contains(this), "Bitch ! - try to show a dialog twice ?? ");
    _dialogs.pushBack(this);
    DialogCallback callback = this->getCallback();
    this->setCallback([=](int error, const string &requestId, const list<string> &recveivers){
        if (callback) {
            callback(error, requestId, recveivers);
        }
        _dialogs.eraseObject(this);
    });
    _impl->show(this);
}

NS_SCREW_FACEBOOK_END
