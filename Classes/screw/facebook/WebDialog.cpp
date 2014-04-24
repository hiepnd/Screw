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

#include "WebDialog.h"
#include "../utils/StringUtils.h"
#include "../utils/JsonUtils.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "WebDialogApple.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/screw/facebook/WebDialogAndroid.h"
#endif


NS_SCREW_FACEBOOK_BEGIN

Vector<WebDialog *> WebDialog::_dialogs;

WebDialog::WebDialog(): _dialog(""), _params(ValueMap()), _callback(nullptr) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _impl = new screw::ios::WebDialogApple();
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _impl = new screw::jni::WebDialogAndroid();
#endif
}

WebDialog::~WebDialog() {
    delete _impl;
}

WebDialog *WebDialog::create(const string &dialog, const ValueMap &params, const WebDialogCallback &callback) {
    WebDialog *d = new WebDialog(dialog, params, callback);
    d->autorelease();
    return d;
}

WebDialog::WebDialog(const string &dialog, const ValueMap &params, const WebDialogCallback &callback):
WebDialog()
{
    this->setDialog(dialog);
    this->setParams(params);
    this->setCallback(callback);
}

const WebDialogCallback &WebDialog::getCallback() const {
	return _callback;
}

void WebDialog::setCallback(const WebDialogCallback &callback) {
	_callback = callback;
}

const string &WebDialog::getDialog() const {
	return _dialog;
}

void WebDialog::setDialog(const string &dialog) {
	_dialog = dialog;
}

ValueMap &WebDialog::getParams() {
	return _params;
}

void WebDialog::setParams(const ValueMap &params) {
	_params = params;
}

void WebDialog::show() {
    FB_LOG("Dialog::show - showing %ld dialog(s)", _dialogs.size());
    CCASSERT(!_dialogs.contains(this), "Bitch ! - try to show a dialog twice ?? ");
    _dialogs.pushBack(this);
    WebDialogCallback callback = this->getCallback();
    this->setCallback([=](int error, ValueMap &values){
        if (callback) {
            callback(error, values);
        }
        _dialogs.eraseObject(this);
    });
    _impl->show(this);
}

#pragma mark Request Dialog Builder
RequestDialogBuilder *RequestDialogBuilder::setMessage(const string &message) {
    _params["message"] = message;
    return this;
}

RequestDialogBuilder *RequestDialogBuilder::setTitle(const string &title) {
    _params["title"] = title;
    return this;
}

RequestDialogBuilder *RequestDialogBuilder::setTo(const string &uid) {
    _params["to"] = uid;
    return this;
}

RequestDialogBuilder *RequestDialogBuilder::setTo(const list<string> &uids) {
    _params["to"] = utils::StringUtils::join(uids, ",");
    return this;
}

RequestDialogBuilder *RequestDialogBuilder::setType(int type) {
    _data["_t"] = type;
    return this;
}

RequestDialogBuilder *RequestDialogBuilder::setData(const string &key, const string &value) {
    _data[key] = value;
    return this;
}

RequestDialogBuilder *RequestDialogBuilder::setCallback(const RequestDialogCallback &callback) {
    _callback = callback;
    return this;
}

WebDialog *RequestDialogBuilder::build() {
    _params["data"] = JsonUtils::toJsonString(_data);
    WebDialogCallback callback = nullptr;
    if (_callback) {
        RequestDialogCallback &this_callback = _callback;
        /* Capture this_callback by copy
         * Cannot capture 'this' here, because it is (normally) freed after build() is called
         */
        callback = [this_callback](int error, ValueMap &values){
            string rid = values["request"].asString();
            list<string> recipients;
            int i = 0;
            while (values.find(string("to[") + utils::StringUtils::toString(i) + string("]")) != values.end()) {
                recipients.push_back(values[string("to[") + utils::StringUtils::toString(i) + string("]")].asString());
                i++;
            }
            this_callback(error, rid, recipients);
        };
    }

    return WebDialog::create("apprequests", _params, callback);
}

#pragma mark Feed Dialog Builder
FeedDialogBuilder *FeedDialogBuilder::setName(const string &name) {
    _params["name"] = name;
    return this;
}

FeedDialogBuilder *FeedDialogBuilder::setLink(const string &link) {
    _params["link"] = link;
    return this;
}

FeedDialogBuilder *FeedDialogBuilder::setCaption(const string &caption) {
    _params["caption"] = caption;
    return this;
}

FeedDialogBuilder *FeedDialogBuilder::setDescription(const string &description) {
    _params["description"] = description;
    return this;
}

FeedDialogBuilder *FeedDialogBuilder::setTo(const string &uid) {
    _params["to"] = uid;
    return this;
}

FeedDialogBuilder *FeedDialogBuilder::setCallback(const FeedDialogCallback &callback) {
    _callback = callback;
    return this;
}


WebDialog *FeedDialogBuilder::build() {
    WebDialogCallback callback = nullptr;
    if (_callback) {
        FeedDialogCallback &this_callback = _callback;
        callback = [this_callback](int error, ValueMap &values){
            string rid = values["post_id"].asString();;
            this_callback(error, rid);
        };
    }

    return WebDialog::create("feed", _params, callback);
}


NS_SCREW_FACEBOOK_END
