//
//  Dialog.cpp
//  Screw
//
//  Created by Ngo Duc Hiep on 3/28/14.
//
//

#include "Dialog.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/screw/facebook/DialogAndroid.h"
#endif


NS_SCREW_FACEBOOK_BEGIN

Vector<Dialog *> Dialog::_dialogs;

Dialog::Dialog(): _dialog(""), _params(ValueMap()), _callback(nullptr) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _impl = new jni::DialogAndroid();
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

Dialog::Dialog(const string &dialog, const ValueMap &params, const DialogCallback &callback) {
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
