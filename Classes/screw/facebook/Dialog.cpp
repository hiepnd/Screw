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

bool Dialog::canPresent(ShareDialogParams *params) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return screw::ios::DialogApple::canPresent(params);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return screw::jni::DialogAndroid::canPresent(params);
#endif
}

void Dialog::present(ShareDialogParams *params, const DialogCallback &callback) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    screw::ios::DialogApple::present(params, callback);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    screw::jni::DialogAndroid::present(params, callback);
#endif
    
}

bool Dialog::canPresent(OpenGraphActionShareDialogParams *params) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return screw::ios::DialogApple::canPresent(params);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return screw::jni::DialogAndroid::canPresent(params);
#endif
    
}

void Dialog::present(OpenGraphActionShareDialogParams *params, const DialogCallback &callback) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    screw::ios::DialogApple::present(params, callback);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    screw::jni::DialogAndroid::present(params, callback);
#endif
    
}

void Dialog::presentShareLink(const string &link, const string &name, const string &caption,
                      const string &description, const string &picture, const DialogCallback &callback) {
    ShareDialogParams *params = ShareDialogParams::create();
    params->setLink(link);
    params->setName(name);
    params->setCaption(caption);
    params->setDescription(description);
    params->setPicture(picture);
    Dialog::present(params, callback);
}

NS_SCREW_FACEBOOK_END
