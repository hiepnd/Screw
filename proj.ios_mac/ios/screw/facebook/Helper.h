//
//  Helper.h
//  Screw
//
//  Created by Ngo Duc Hiep on 4/3/14.
//
//

#ifndef __Screw__Helper__
#define __Screw__Helper__

#include <iostream>
#include <list>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

namespace screw { namespace ios {

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
};
    
}}  /* namespace screw { namespace ios { */

#endif /* defined(__Screw__Helper__) */
