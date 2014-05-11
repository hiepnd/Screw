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

#ifndef _SCREW_HELPERAPPLE_H_
#define _SCREW_HELPERAPPLE_H_

#include <iostream>
#include <list>
#import <FacebookSDK/FacebookSDK.h>
#include "cocos2d.h"
#include "screw.h"

using namespace std;
using namespace cocos2d;
USING_NS_SCREW_FACEBOOK;

NS_SCREW_IOS_BEGIN

class NSObjectVisitor {
public:
    virtual void visitObject(id object);
    virtual void visitDictionary(NSDictionary *dictionary);
    virtual void visitDictionaryStart() = 0;
    virtual void visitDictionaryEnd() = 0;
    virtual void visitArray(NSArray *array);
    virtual void visitArrayStart() = 0;
    virtual void visitArrayEnd() = 0;
    virtual void visitNumber(NSNumber *number) = 0;
    virtual void visitString(NSString *string) = 0;
};

class NSObjectValueConverter : public NSObjectVisitor {
public:
    virtual void visitDictionaryStart();
    virtual void visitDictionaryEnd();
    virtual void visitArrayStart();
    virtual void visitArrayEnd();
    virtual void visitNumber(NSNumber *number);
    virtual void visitString(NSString *string);
    
    Value &getResult();
private:
    NSMutableArray *_stack;
};

class Helper {
    
public:
    static string nsString2cString(NSString *str);
    static list<string> nsArray2cList(NSArray *array);
    
    static NSArray *cList2nsArray(const list<string> &ll);
    
    static ValueMap nsDictionary2ValueMap(NSDictionary *dic);
    static void nsDictionary2ValueMap(NSDictionary *dic, ValueMap &vm);
    
    static ValueVector nsArray2ValueVector(NSArray *array);
    static void nsArray2ValueVector(NSArray *array, ValueVector &vm);
    
    static NSDictionary *valueMap2nsDictionary(ValueMap &vm);
    static NSArray *valueVector2nsArray(ValueVector &vv);
    
    static FBShareDialogParams *fromParams(ShareDialogParams *cparams);
    static FBOpenGraphActionShareDialogParams *fromOGParams(OpenGraphActionShareDialogParams *cparams);
};
    
NS_SCREW_IOS_END

#endif /* _SCREW_HELPERAPPLE_H_ */
