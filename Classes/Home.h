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

#ifndef __Screw__Home__
#define __Screw__Home__

#include <iostream>
#include "screw/screw.h"
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "settings/Settings.h"

USING_NS_CC;
USING_NS_SCREW;
USING_NS_SCREW_FACEBOOK;
using namespace cocos2d::network;

class Home : public Scene {
public:
    static Home *create();
    bool init();
    
    void onEnter() override;
    void onExit() override;
    
private:
    void onSessionStateChanged(EventCustom *event);
    void onUserDetailFetched(EventCustom *event);
    void onFriendsFetched(EventCustom *event);
    void onPhotoDownloaded(EventCustom *event);
    
    void updateUI();
    void updateGreeting();
    void updateAvatar();
    void updateFishCount();
    
    void login();
    void logout();
    void fetchUserDetail();
    void fetchFriends();
    void fetchInvitableFriends();
    void shareLink();
    void shareStatus();
    void shareOpenGraph();
    void requestPublish();
    void deauthorize();
    
    Label *_greetingLbl;
    Label *_loginLbl;
    Label *_fishCount;
    Sprite *_avatar;
};

#endif /* defined(__Screw__Home__) */
