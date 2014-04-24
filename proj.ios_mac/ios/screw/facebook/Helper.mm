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

#include "Helper.h"

NS_SCREW_IOS_BEGIN

string Helper::nsString2cString(NSString *str) {
    return [str cStringUsingEncoding:NSUTF8StringEncoding];
}

list<string> Helper::nsArray2cList(NSArray *array) {
    list<string> ls;
    for (NSString *s : array) {
        ls.push_back([s cStringUsingEncoding:NSUTF8StringEncoding]);
    }
    return ls;
}

NSArray *Helper::cList2nsArray(const list<string> &ll) {
    NSMutableArray *ns = [NSMutableArray arrayWithCapacity:ll.size()];
    for (auto s : ll) {
        [ns addObject:[NSString stringWithUTF8String:s.c_str()]];
    }
    return ns;
}
    
ValueMap Helper::nsDictionary2ValueMap(NSDictionary *dic) {
    ValueMap vm;
    nsDictionary2ValueMap(dic, vm);
    return vm;
}
    
void Helper::nsDictionary2ValueMap(NSDictionary *dic, ValueMap &vm) {
    NSArray *keys = [dic allKeys];
    for (NSString *k : keys) {
        id obj = dic[k];
        
        if ([obj isKindOfClass:[NSDictionary class]]) {
            //Dictionary
            ValueMap vmm;
            nsDictionary2ValueMap((NSDictionary *) obj, vmm);
            vm[[k cStringUsingEncoding:NSUTF8StringEncoding]] = vmm;
        } else if ([obj isKindOfClass:[NSArray class]]) {
            //Array
            ValueVector vv;
            nsArray2ValueVector((NSArray *) obj, vv);
            vm[[k cStringUsingEncoding:NSUTF8StringEncoding]] = vv;
        } else if ([obj isKindOfClass:[NSString class]]) {
            //String
            vm[[k cStringUsingEncoding:NSUTF8StringEncoding]] = [(NSString *) obj cStringUsingEncoding:NSUTF8StringEncoding];
        } else if ([obj isKindOfClass:[NSNumber class]]) {
            //Number
            vm[[k cStringUsingEncoding:NSUTF8StringEncoding]] = [obj stringValue];
        } else {
            NSLog(@"%s - Non supported type %@", __FUNCTION__, [obj class]);
        }
    }
}

ValueVector Helper::nsArray2ValueVector(NSArray *array) {
    ValueVector vv;
    nsArray2ValueVector(array, vv);
    return vv;
}

void Helper::nsArray2ValueVector(NSArray *array, ValueVector &vv) {
    for (id obj : array) {
        if ([obj isKindOfClass:[NSDictionary class]]) {
            //Dictionary
            ValueMap vm;
            nsDictionary2ValueMap((NSDictionary *) obj, vm);
            vv.push_back(Value(vm));
        } else if ([obj isKindOfClass:[NSArray class]]) {
            //Array
            ValueVector vvv;
            nsArray2ValueVector((NSArray *) obj, vvv);
            vv.push_back(Value(vvv));
        } else if ([obj isKindOfClass:[NSString class]]) {
            //String
            vv.push_back(Value([(NSString *)obj cStringUsingEncoding:NSUTF8StringEncoding]));
        } else if ([obj isKindOfClass:[NSNumber class]]) {
            //Number
            vv.push_back(Value([obj stringValue]));
        } else {
            NSLog(@"%s - Non supported type %@", __FUNCTION__, [obj class]);
        }
    }
}
    
NSDictionary *Helper::valueMap2nsDictionary(ValueMap &vm) {
    NSMutableDictionary *dic = [NSMutableDictionary dictionary];
    for (auto i : vm) {
        NSString *key = [NSString stringWithUTF8String:i.first.c_str()];
        switch (i.second.getType()) {
            case Value::Type::MAP:
                dic[key] = valueMap2nsDictionary(i.second.asValueMap());
                break;
                
            case Value::Type::VECTOR:
                dic[key] = valueVector2nsArray(i.second.asValueVector());
                break;
                
            default:
                dic[key] = [NSString stringWithUTF8String:i.second.asString().c_str()];
                break;
        }
    }
    return dic;
}
 
NSArray *Helper::valueVector2nsArray(ValueVector &vv) {
    NSMutableArray *array = [NSMutableArray arrayWithCapacity:vv.size()];
    for (auto i : vv) {
        switch (i.getType()) {
            case Value::Type::MAP:
                [array addObject:valueMap2nsDictionary(i.asValueMap())];
                break;
                
            case Value::Type::VECTOR:
                [array addObject:valueVector2nsArray(i.asValueVector())];
                break;
                
            default:
                [array addObject:[NSString stringWithUTF8String:i.asString().c_str()]];
                break;
        }
    }
    
    return array;
}

FBShareDialogParams *Helper::fromParams(ShareDialogParams *cparams) {
    FBShareDialogParams *params = [[[FBShareDialogParams alloc] init] autorelease];
    
    //Link
    if (cparams->hasLink()) {
        params.link = [NSURL URLWithString:[NSString stringWithUTF8String:cparams->getLink().c_str()]];
    }
    
    //Name
    if (cparams->hasName()) {
        params.name = [NSString stringWithUTF8String:cparams->getName().c_str()];
    }
    
    //Caption
    if (cparams->hasCaption()) {
        params.caption = [NSString stringWithUTF8String:cparams->getCaption().c_str()];
    }
    
    //Description
    if (cparams->hasDescription()) {
        params.description = [NSString stringWithUTF8String:cparams->getDescription().c_str()];
    }
    
    //Ref
    if (cparams->hasRef()) {
        params.ref = [NSString stringWithUTF8String:cparams->getRef().c_str()];
    }
    
    //Picture
    if (cparams->hasPicture()) {
        params.picture = [NSURL URLWithString:[NSString stringWithUTF8String:cparams->getPicture().c_str()]];
    }
    
    //Friends
    params.friends = cList2nsArray(cparams->getFriends());
    
    //DataFailuresFatal
    if (cparams->hasDataFailuresFatal()) {
        params.dataFailuresFatal = cparams->getDataFailuresFatal();
    }
    
    return params;
}

FBOpenGraphActionShareDialogParams *Helper::fromOGParams(OpenGraphActionShareDialogParams *cparams) {
    FBOpenGraphActionShareDialogParams *params = [[[FBOpenGraphActionShareDialogParams alloc] init] autorelease];
    
    //Action
    OpenGraphAction *action = cparams->getAction();
    if (action) {
        params.action = (id<FBOpenGraphAction>)[FBGraphObject graphObjectWrappingDictionary:Helper::valueMap2nsDictionary(action->getValue().asValueMap())];
    }
    
    //PreviewPropertyName
    params.previewPropertyName = [NSString stringWithUTF8String:cparams->getPreviewPropertyName().c_str()];
    
    //ActionType
    params.actionType = [NSString stringWithUTF8String:cparams->getActionType().c_str()];
    return params;
}

NS_SCREW_IOS_END  /* namespace screw { namespace ios { */