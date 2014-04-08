//
//  DialogApple.cpp
//  Screw
//
//  Created by Ngo Duc Hiep on 4/3/14.
//
//

#include "DialogApple.h"
#import <FacebookSDK/FacebookSDK.h>
#include "Helper.h"

void DialogApple::show(Dialog *dialog) {
#if FB_DEBUG >= 1
    NSLog(@"DialogApple::show\n{\n\taction = %s\n\tparams = %s\n}", dialog->getDialog().c_str(), Value(dialog->getParams()).getDescription().c_str());
#endif
    ValueMap &params = dialog->getParams();
    params["frictionless"] = 1;
    [FBWebDialogs presentDialogModallyWithSession:[FBSession activeSession]
                                           dialog:[NSString stringWithUTF8String:dialog->getDialog().c_str()]
                                       parameters:screw::ios::Helper::valueMap2nsDictionary(params)
                                          handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
#if FB_DEBUG >= 1
                                              NSLog(@"DialogApple::DialogCallback - result = %d, url = %@, error = %@", result, resultURL, error);
#endif
                                              //url: url = fbconnect://success?request=683722131674077&to%5B0%5D=664038782
                                              if (dialog->getCallback()) {
                                                  int errorCode = 1;
                                                  string rid;
                                                  list<string> receivers;
                                                  
                                                  if (resultURL) {
                                                      NSString *queryStr = [resultURL query];
                                                      NSArray *array = [queryStr componentsSeparatedByString:@"&"];
                                                      NSMutableDictionary *queries = [NSMutableDictionary dictionary];
                                                      for (NSString *s : array) {
                                                          NSArray *ss = [s componentsSeparatedByString:@"="];
                                                          if (ss.count == 2) {
                                                              NSString *key = [ss[0] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
                                                              queries[key] = ss[1];
                                                          }
                                                      }
                                                      
                                                      if (queries[@"request"]) {
                                                          rid = [queries[@"request"] cStringUsingEncoding:NSUTF8StringEncoding];
                                                          errorCode = 0;
                                                      }
                                                      
                                                      for (int i = 0; true; i++) {
                                                          NSString *to = queries[[NSString stringWithFormat:@"to[%d]",i]];
                                                          if (!to) {
                                                              break;
                                                          }
                                                          receivers.push_back([to cStringUsingEncoding:NSUTF8StringEncoding]);
                                                      }
                                                  }
                                                  dialog->getCallback()(errorCode, rid, receivers);
                                              }
                                          }];
}