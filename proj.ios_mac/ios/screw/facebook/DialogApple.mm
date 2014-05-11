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

#include "DialogApple.h"
#include "Helper.h"

NS_SCREW_IOS_BEGIN

bool DialogApple::canPresent(ShareDialogParams *params) {
    return [FBDialogs canPresentShareDialogWithParams:Helper::fromParams(params)];
}

void DialogApple::present(ShareDialogParams *params, const DialogCallback &callback) {
#if FB_DEBUG
    NSLog(@"%s - params = %@", __FUNCTION__, Helper::fromParams(params));
#endif
    DialogCallback callback_ = callback;
    [FBDialogs presentShareDialogWithParams:Helper::fromParams(params)
                                clientState:nil
                                    handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
#if FB_DEBUG
                                        NSLog(@"DialogApple - present share dialog: results = %@, error = %@", results, error);
#endif
                                        if (callback_) {
                                            GraphObject *og = nullptr;
                                            if (!error && results) {
                                                og = GraphObject::create(Value(Helper::nsDictionary2ValueMap(results)));
                                            }
                                            callback_(og, error.code);
                                        }
                                    }];
}

bool DialogApple::canPresent(OpenGraphActionShareDialogParams *params) {
    return [FBDialogs canPresentShareDialogWithOpenGraphActionParams:Helper::fromOGParams(params)];
}

void DialogApple::present(OpenGraphActionShareDialogParams *params, const DialogCallback &callback) {
#if FB_DEBUG
    NSLog(@"%s - actionType = %@, action = %@", __FUNCTION__, Helper::fromOGParams(params).actionType, Helper::fromOGParams(params).action);
#endif
    DialogCallback callback_ = callback;
    [FBDialogs presentShareDialogWithOpenGraphActionParams:Helper::fromOGParams(params)
                                               clientState:nil
                                                   handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
#if FB_DEBUG
                                                       NSLog(@"%s - result = %@, error = %@", __FUNCTION__, results, error);
#endif

                                                       if (callback_) {
                                                           GraphObject *og = nullptr;
                                                           if (!error && results) {
                                                               og = GraphObject::create(Value(Helper::nsDictionary2ValueMap(results)));
                                                           }
                                                           callback_(og, error.code);
                                                       }
                                                   }];
    
//    id<FBGraphObject> object =
//    [FBGraphObject openGraphObjectForPostWithType:@"books.book"
//                                            title:@"Roasted pumpkin seeds"
//                                            image:@"http://i.imgur.com/g3Qc1HN.png"
//                                              url:@"http://samples.ogp.me/344468272304428"
//                                      description:@"Crunchy pumpkin seeds roasted in butter and lightly salted."];
//    
//    id<FBOpenGraphAction> action = (id<FBOpenGraphAction>)[FBGraphObject graphObject];
//    
//    // Link the object to the action
//    [action setObject:object forKey:@"book"];
//    
//    FBOpenGraphActionShareDialogParams *pp = [[FBOpenGraphActionShareDialogParams alloc] init];
//    pp.action = action;
//    pp.actionType = @"books.reads";
//    NSLog(@"%s - actionType = %@, action = %@", __FUNCTION__, pp.actionType, pp.action);
//    [FBDialogs presentShareDialogWithOpenGraphActionParams:pp
//                                               clientState:nil
//                                                   handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
//                                                       NSLog(@"results = %@, error = %@", results, error);
//    }];
}


NS_SCREW_IOS_END
