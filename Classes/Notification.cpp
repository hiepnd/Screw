/****************************************************************************
 Copyright (c) hiepndhut@gmail.com
 
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

#include "Notification.h"

Notification *Notification::_instance = nullptr;

Notification *Notification::getInstance() {
    if (!_instance) {
        _instance = new Notification();
    }
    
    return _instance;
}

Notification::Notification() {
    Scale9Sprite::initWithFile("info-box.png");
    
    setColor(Color3B::GREEN);
    _running = true;
    _designSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
    _label = nullptr;
    this->setVisible(false);
}

void Notification::show(const string &msg, float duration, const Point offset, NotificationAnchor anchor) {
    NotificationMessage m(msg, duration, offset, anchor);
    _messages.push_back(m);
    
    if (_messages.size() == 1) {
        this->showNext();
    }
}

void Notification::setOpacity(GLubyte opacity) {
    Scale9Sprite::setOpacity(opacity);
    if (_label) {
        _label->setOpacity(opacity);
    }
}

void Notification::showNext() {
    this->setVisible(true);
    NotificationMessage &m = _messages.front();
    
    _label = LabelTTF::create(m.message,
                              "Arial",
                              40,
                              Size(600, 0),
                              TextHAlignment::CENTER,
                              TextVAlignment::CENTER);
    this->addChild(_label);
    Size s = _label->getContentSize();
    this->setContentSize(Size(s.width + 20, MAX(s.height, 120)));
    _label->setPosition(_contentSize.width/2, _contentSize.height/2);
    
    Point offset(0, 0);
    switch (m.anchor) {
        case TOP:
        {
            this->setAnchorPoint(Point(0.5, 1.0));
            offset.y = _designSize.height/2 - _contentSize.height/2;;
        }
            break;
        
        case CENTER:
        {
            this->setAnchorPoint(Point(0.5, 0.5));
        }
            break;
            
        case LEFT:
        {
            this->setAnchorPoint(Point(0.0, 0.5));
            offset.x = s.width/2;
        }
            break;
            
        case RIGHT:
        {
            this->setAnchorPoint(Point(1.0, 0.5));
            offset.y = -s.width/2;
        }
            break;
    }
    
    this->setPosition(_designSize.width/2 + m.offset.x + offset.x, _designSize.height/2 + offset.y + m.offset.y);
    
    auto fadeIn = FadeTo::create(0.2, 255);
    auto delay = DelayTime::create(m.duration);
    auto fadeOut = FadeOut::create(0.2);
    auto check = CallFunc::create([this](){
        this->removeChild(_label);
        _label = nullptr;
        _messages.pop_front();
        if (_messages.size()) {
            this->showNext();
        } else {
            this->setVisible(false);
        }
    });
    
    auto sequence = Sequence::create(fadeIn, delay, fadeOut, check, NULL);
    this->runAction(sequence);
}
