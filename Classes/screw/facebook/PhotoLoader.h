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

#ifndef _SCREW_PHOTOLOADER_H_
#define _SCREW_PHOTOLOADER_H_

#include <iostream>
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "../macros.h"
#include <set>


USING_NS_CC;
using namespace cocos2d::network;
using namespace std;

NS_SCREW_FACEBOOK_BEGIN

extern const string PhotoLoaderLoadedNotification;

class PhotoLoaderEvent : public EventCustom {
public:
    PhotoLoaderEvent(const string &evenName, const string &uid):EventCustom(evenName), _uid(uid) {}
    
    const string &getUid() {
        return _uid;
    }
private:
    string _uid;
};


class PhotoLoader : public Ref {
public:
    static PhotoLoader *getInstance();
    
    bool isPhotoExist(const string &uid);
    void download(const string &uid, int size = 128);
    
    Texture2D *loadTexture(const string &uid);
    void httpCallback(HttpClient* client, HttpResponse* response);
    
private:
    PhotoLoader();
        
    static PhotoLoader *_instance;
    set<string> _dowloadings;
};

NS_SCREW_FACEBOOK_END

#endif /* _SCREW_PHOTOLOADER_H_ */
