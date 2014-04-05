//
//  SessionApple.h
//  Screw
//
//  Created by Ngo Duc Hiep on 4/3/14.
//
//

#ifndef __Screw__SessionApple__
#define __Screw__SessionApple__

#include <iostream>
#include "Session.h"

class SessionApple : public screw::facebook::SessionImpl {
    
public:
    virtual void open(bool allowUi , const list<string> &permission);
    virtual void close();
    virtual void requestReadPermissions(const list<string> &permission);
    virtual void requestPublishPermissions(const list<string> &permission);
    
    static void start();
    
private:
    static bool _started;
};

#endif /* defined(__Screw__SessionApple__) */
