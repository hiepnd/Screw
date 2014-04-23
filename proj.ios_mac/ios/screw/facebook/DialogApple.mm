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

void DialogApple::present(ShareDialogParams *params, const screw::facebook::DialogCallback &callback) {
    [FBDialogs presentShareDialogWithParams:Helper::fromParams(params)
                                clientState:nil
                                    handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
                                        if (callback) {
                                            GraphObject *og = nullptr;
                                            if (!error && results) {
                                                og = GraphObject::create(Value(Helper::nsDictionary2ValueMap(results)));
                                            }
                                            callback(og, error.code);
                                        }
                                    }];
}

bool DialogApple::canPresent(OpenGraphActionShareDialogParams *params) {
    return [FBDialogs canPresentShareDialogWithOpenGraphActionParams:Helper::fromOGParams(params)];
}

void DialogApple::present(OpenGraphActionShareDialogParams *params, const DialogCallback &callback) {
    [FBDialogs presentShareDialogWithOpenGraphActionParams:Helper::fromOGParams(params)
                                               clientState:nil
                                                   handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
                                                       if (callback) {
                                                           GraphObject *og = nullptr;
                                                           if (!error && results) {
                                                               og = GraphObject::create(Value(Helper::nsDictionary2ValueMap(results)));
                                                           }
                                                           callback(og, error.code);
                                                       }
                                                   }];
}

NS_SCREW_IOS_END
