//
//  Utils.cpp
//  Screw
//
//  Created by Ngo Duc Hiep on 4/5/14.
//
//

#include "Utils.h"
#include <time.h>

NS_SCREW_UTILS_BEGIN

long Utils::getTimestamp() {
    return time(NULL);
}

NS_SCREW_UTILS_END
