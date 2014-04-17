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

#include "SessionApple.h"
#include "Helper.h"
#import <FacebookSDK/FacebookSDK.h>

USING_NS_SCREW_FACEBOOK;

NS_SCREW_IOS_BEGIN

bool SessionApple::_started = false;
static FBSessionStateHandler _statusCallback = nil;

static Session::State nsState2NativeState(FBSessionState status);
static FBSessionLoginBehavior getLoginBehavior(LoginBehavior behavior);
static FBSessionDefaultAudience getDefaultAudience(DefaultAudience audience);

void SessionApple::start() {
    CCASSERT(!_started, "Must call me once");
    _started = true;
    
    FBSession *session = [FBSession activeSession];
    if (!session) {
        session = [[[FBSession alloc] init] autorelease];
        [FBSession setActiveSession:session];
    }
    _statusCallback = ^(FBSession *session, FBSessionState status, NSError *error) {
#if FB_DEBUG
        NSLog(@"SessionApple::StatusCallback\n{\n\tstate = %d\n\tpermissions = %@\n\terror = %@\n}", status, session.permissions, error);
#endif
        Session::getActiveSession()->updateState(nsState2NativeState(session.state),
                                                 screw::ios::Helper::nsArray2cList(session.permissions));
    };
    [_statusCallback retain];
    
    Session::initActiveSession(nsState2NativeState(session.state),
                               screw::ios::Helper::nsString2cString(session.appID),
                               screw::ios::Helper::nsArray2cList(session.permissions));
}

void SessionApple::open(bool allowUi, const list<string> &permissions, DefaultAudience defaultAudience, LoginBehavior loginBehavior) {
    FBSession *session = [[[FBSession alloc] initWithAppID:nil
                                               permissions:screw::ios::Helper::cList2nsArray(permissions)
                                           defaultAudience:getDefaultAudience(defaultAudience)
                                           urlSchemeSuffix:nil
                                        tokenCacheStrategy:nil]
                          autorelease];
    if (allowUi || session.state == FBSessionStateCreatedTokenLoaded) {
        [FBSession setActiveSession:session];
        [session openWithBehavior:getLoginBehavior(loginBehavior)
                completionHandler:_statusCallback];
    }
}

void SessionApple::close() {
    [[FBSession activeSession] closeAndClearTokenInformation];
}

void SessionApple::requestReadPermissions(const list<string> &permissions) {
    [[FBSession activeSession] requestNewReadPermissions:screw::ios::Helper::cList2nsArray(permissions)
                                       completionHandler:nil];
}

void SessionApple::requestPublishPermissions(const list<string> &permissions) {
    [[FBSession activeSession] requestNewPublishPermissions:screw::ios::Helper::cList2nsArray(permissions)
                                            defaultAudience:FBSessionDefaultAudienceFriends
                                          completionHandler:nil];
}

Session::State nsState2NativeState(FBSessionState status) {
    switch (status) {
        case FBSessionStateCreated:
            return Session::State::CREATED;
            break;
            
        case FBSessionStateCreatedTokenLoaded:
            return Session::State::CREATED_TOKEN_LOADED;
            break;
            
        case FBSessionStateCreatedOpening:
            return Session::State::OPENING;
            break;
            
        case FBSessionStateClosedLoginFailed:
            return Session::State::CLOSED_LOGIN_FAILED;
            break;
            
        case FBSessionStateClosed:
            return Session::State::CLOSED;
            break;
            
        case FBSessionStateOpen:
            return Session::State::OPENED;
            break;
            
        case FBSessionStateOpenTokenExtended:
            return Session::State::OPENED_TOKEN_UPDATED;
            break;
            
        default:
            break;
    }
    
    return Session::State::INVALID;
}

FBSessionLoginBehavior getLoginBehavior(LoginBehavior behavior) {
    switch (behavior) {
        case WITH_FALLBACK_TO_WEBVIEW:
            return FBSessionLoginBehaviorWithFallbackToWebView;
            break;
            
        case WITH_NO_FALLBACK_TO_WEBVIEW:
            return FBSessionLoginBehaviorWithNoFallbackToWebView;
            break;
            
        case FORCE_WEBVIEW:
            return FBSessionLoginBehaviorForcingWebView;
            break;
            
        case SYSTEM_IF_PRESENT:
            return FBSessionLoginBehaviorUseSystemAccountIfPresent;
            break;
            
        default:
            CCASSERT(false, "Unknown behavior");
            break;
    }
}

FBSessionDefaultAudience getDefaultAudience(DefaultAudience audience) {
    switch (audience) {
        case DefaultAudience::PUBLIC:
            return FBSessionDefaultAudienceEveryone;
            break;
            
        case DefaultAudience::ONLY_ME:
            return FBSessionDefaultAudienceOnlyMe;
            break;
            
        case DefaultAudience::FRIENDS:
            return FBSessionDefaultAudienceFriends;
            break;
            
        default:
            return FBSessionDefaultAudienceNone;
            break;
    }
}

NS_SCREW_IOS_END