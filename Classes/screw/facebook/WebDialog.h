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

#ifndef __Screw__Dialog__
#define __Screw__Dialog__

#include <iostream>
#include "../macros.h"
#include "cocos2d.h"
#include "GraphObject.h"

USING_NS_CC;
using namespace std;

NS_SCREW_FACEBOOK_BEGIN

typedef std::function<void(int error, const string &requestId, const list<string> &recveivers)> DialogCallback;

class WebDialog;

class WebDialogImpl {
public:
    virtual ~WebDialogImpl(){}
    virtual void show(WebDialog *dialog) = 0;
};

class WebDialog : public Object {
    
public:
    WebDialog();
    ~WebDialog();
    WebDialog(const string &dialog, const ValueMap &params, const DialogCallback &callback);
    static WebDialog *create(const string &dialog, const ValueMap &params, const DialogCallback &callback);
	
    const DialogCallback &getCallback() const;
    const string &getDialog() const;
    ValueMap &getParams();
    
	void setCallback(const DialogCallback &callback);
	void setDialog(const string &dialog);
	void setParams(const ValueMap &params);

    void show();
    
private:
    string _dialog;
    ValueMap _params;
    DialogCallback _callback;
    
    WebDialogImpl *_impl;
    
    /* Dialogs in progress */
    static Vector<WebDialog *> _dialogs;
};

NS_SCREW_FACEBOOK_END

#endif /* defined(__Screw__Dialog__) */
