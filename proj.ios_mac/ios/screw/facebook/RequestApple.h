//
//  RequestApple.h
//  Screw
//
//  Created by Ngo Duc Hiep on 4/3/14.
//
//

#ifndef __Screw__RequestApple__
#define __Screw__RequestApple__

#include <iostream>
#include "screw.h"
#include "cocos2d.h"

USING_NS_SCREW_FACEBOOK;
USING_NS_CC;

class RequestApple : public RequestImpl {
    
public:
    virtual void execute(Request *request);
    
private:

};

#endif /* defined(__Screw__RequestApple__) */
