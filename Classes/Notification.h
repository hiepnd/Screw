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

#ifndef _SCREW_NOTIFICATION_H_
#define _SCREW_NOTIFICATION_H_

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include <list>

using namespace cocos2d;
using namespace std;
using namespace cocos2d::extension;

enum NotificationAnchor{
    TOP,
    CENTER,
    LEFT,
    RIGHT
};

struct NotificationMessage{
    NotificationMessage(const string &msg, float d, const Point &off, NotificationAnchor a):
    message(msg), duration(d), offset(off), anchor(a){
        
    }
    
    string message;
    float duration;
    Point offset;
    NotificationAnchor anchor;
};

class Notification: public Scale9Sprite {
    
public:
    Notification();
    static Notification *getInstance();
    
    void show(const string &msg, float duration = 2.0f, const Point offset = Point(), NotificationAnchor anchor = NotificationAnchor::TOP);
    void setOpacity(GLubyte opacity) override;
    
private:
    static Notification *_instance;
    
    void showNext();
    
    list<NotificationMessage>   _messages;
    LabelTTF                    *_label;
    Size                        _designSize;
};

#endif /* _SCREW_NOTIFICATION_H_ */
