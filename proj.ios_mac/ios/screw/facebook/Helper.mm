//
//  Helper.cpp
//  Screw
//
//  Created by Ngo Duc Hiep on 4/3/14.
//
//

#include "Helper.h"

namespace screw { namespace ios {

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

}}  /* namespace screw { namespace ios { */