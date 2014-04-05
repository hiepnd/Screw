//
//  DialogApple.h
//  Screw
//
//  Created by Ngo Duc Hiep on 4/3/14.
//
//

#ifndef __Screw__DialogApple__
#define __Screw__DialogApple__

#include <iostream>
#include "screw.h"
#include "cocos2d.h"

USING_NS_SCREW_FACEBOOK;
USING_NS_CC;

class DialogApple : public DialogImpl {
    
public:
    virtual void show(Dialog *dialog);
};

#endif /* defined(__Screw__DialogApple__) */
