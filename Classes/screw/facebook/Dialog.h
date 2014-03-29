//
//  Dialog.h
//  Screw
//
//  Created by Ngo Duc Hiep on 3/28/14.
//
//

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

class Dialog;

class DialogImpl {
public:
    virtual ~DialogImpl(){}
    virtual void show(Dialog *dialog) = 0;
};

class Dialog : public Object {
    
public:
    Dialog();
    ~Dialog();
    Dialog(const string &dialog, const ValueMap &params, const DialogCallback &callback);
	
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
    
    DialogImpl *_impl;
    
    /* Dialogs in progress */
    static Vector<Dialog *> _dialogs;
};

NS_SCREW_FACEBOOK_END

#endif /* defined(__Screw__Dialog__) */
