//
//  SessionApple.cpp
//  Screw
//
//  Created by Ngo Duc Hiep on 4/3/14.
//
//

#include "SessionApple.h"
#include "Helper.h"
#import <FacebookSDK/FacebookSDK.h>

USING_NS_SCREW_FACEBOOK;

bool SessionApple::_started = false;
static FBSessionStateHandler _statusCallback = nil;

static Session::State nsState2NativeState(FBSessionState status);

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

void SessionApple::open(bool allowUi, const list<string> &permission) {
    if (FB_ISSESSIONSTATETERMINAL([FBSession activeSession].state)) {
        FBSession *session = [[[FBSession alloc] init] autorelease];
        [FBSession setActiveSession:session];
    }
    
    [FBSession openActiveSessionWithReadPermissions:screw::ios::Helper::cList2nsArray(permission)
                                       allowLoginUI:allowUi
                                  completionHandler:_statusCallback];
}

void SessionApple::close() {
    [[FBSession activeSession] closeAndClearTokenInformation];
}

void SessionApple::requestReadPermissions(const list<string> &permission) {
    [[FBSession activeSession] requestNewReadPermissions:screw::ios::Helper::cList2nsArray(permission)
                                       completionHandler:nil];
}

void SessionApple::requestPublishPermissions(const list<string> &permission) {
    [[FBSession activeSession] requestNewPublishPermissions:screw::ios::Helper::cList2nsArray(permission)
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