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

#include "RequestApple.h"
#include "Helper.h"
#import <FacebookSDK/FacebookSDK.h>

NSString *__getHTTPMethod(Request::Method m);

NS_SCREW_IOS_BEGIN

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
        NSLog(@"RequestApple: %s \"%s\" completed, error = %@", [__getHTTPMethod(request->getMethod())
                                                                    cStringUsingEncoding:NSUTF8StringEncoding],
                                                                request->getGraphPath().c_str(), error);
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

NS_SCREW_IOS_END

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