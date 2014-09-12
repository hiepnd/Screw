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

#include "TwitterApple.h"
#import <Social/Social.h>
#import "AppController.h"

NS_SCREW_IOS_BEGIN

void shareSheet(NSString *text, UIImage *image) {
    NSMutableArray *sharingItems = [NSMutableArray new];
    
    if (text) {
        [sharingItems addObject:text];
    }
    if (image) {
        [sharingItems addObject:image];
    }
    
    RootViewController *vc = [(AppController *) [UIApplication sharedApplication].delegate viewController];
    UIActivityViewController *activityController = [[UIActivityViewController alloc] initWithActivityItems:sharingItems applicationActivities:nil];
    [vc presentViewController:activityController animated:YES completion:nil];
}

void TwitterApple::tweet(const string &message, const string &imagePath, const TwitterTweetCallback &callback) {
    UIImage *image = NULL;
    if (imagePath.length()) {
        NSString *path = [NSString stringWithUTF8String:imagePath.c_str()];
        
        if (![path isAbsolutePath]) {
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentationDirectory, NSUserDomainMask, true);
            if (paths.count) {
                path = [paths[0] stringByAppendingPathComponent:path];
            }
        }
        
        image = [UIImage imageWithContentsOfFile:path];
    }
    
    if ([SLComposeViewController isAvailableForServiceType:SLServiceTypeTwitter]) {
        SLComposeViewController *controller = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeTwitter];
        TwitterTweetCallback callback_ = callback;
        controller.completionHandler = ^(SLComposeViewControllerResult result) {
            if (callback_) {
                callback_(result == SLComposeViewControllerResultDone ? 0 : 1);
            }
        };
        [controller setInitialText:[NSString stringWithUTF8String:message.c_str()]];
        
        if (image) {
            [controller addImage:image];
        }
        
        AppController *app = (AppController *)[UIApplication sharedApplication].delegate;
        [app.viewController presentViewController:controller animated:true completion:nil];
    } else {
        // Fallback to sharesheet
        shareSheet([NSString stringWithUTF8String:message.c_str()], image);
        if (callback) {
            callback(2);
        }
    }
}

NS_SCREW_IOS_END