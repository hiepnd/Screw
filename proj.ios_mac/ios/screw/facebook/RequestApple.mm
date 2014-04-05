//
//  RequestApple.cpp
//  Screw
//
//  Created by Ngo Duc Hiep on 4/3/14.
//
//

#include "RequestApple.h"
#include "Helper.h"
#import <FacebookSDK/FacebookSDK.h>

NSString *__getHTTPMethod(Request::Method m);

void RequestApple::execute(Request *request) {
#if FB_DEBUG >= 1
    NSLog(@"RequestApple::execute\n{\n\tgraphPath = %s\n\tmethod = %d\n\tparams = %s\n}",
          request->getGraphPath().c_str(),
          request->getMethod(), Value(request->getParams()).getDescription().c_str());
#endif
    FBRequest *fbr = [FBRequest requestWithGraphPath:[NSString stringWithUTF8String:request->getGraphPath().c_str()]
                                          parameters:screw::ios::Helper::valueMap2nsDictionary(request->getParams())
                                          HTTPMethod:__getHTTPMethod(request->getMethod())];
    [fbr startWithCompletionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
#if FB_DEBUG >= 1
        NSLog(@"RequestApple: request \"%s\" completed, error = %@", request->getGraphPath().c_str(), error);
#   if FB_DEBUG >= 2
        NSLog(@"Result = %@", result);
#   endif
#endif
        if (request->getCallback()) {
            int errorCode = error == nil ? 0 : 1;
            GraphObject *go = nil;
            if (!errorCode) {
                ValueMap data;
                screw::ios::Helper::nsDictionary2ValueMap((NSDictionary *)result, data);
                go = GraphObject::create(Value(data));
            }
            request->getCallback()(errorCode, go);
        }
    }];
}

NSString *__getHTTPMethod(Request::Method m) {
    switch (m) {
        case Request::Method::GET:
            return @"GET";
            break;
            
        case Request::Method::POST:
            return @"POST";
            break;
            
        case Request::Method::DELETE:
            return @"DELETE";
            break;
            
        default:
            return nil;
            break;
    }
}