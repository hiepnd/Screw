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

#ifndef __Screw__OpenGraphObject__
#define __Screw__OpenGraphObject__

#include <iostream>
#include "../macros.h"
#include "GraphObject.h"
#include "cocos2d.h"

USING_NS_CC;
USING_NS_SCREW_UTILS;
using namespace std;

NS_SCREW_FACEBOOK_BEGIN

class OpenGraphObject : public GraphObject {
public:
    GO_CREATE(OpenGraphObject);
    GO_CREATE_EMPTY(OpenGraphObject);
    
    GO_PROPERTY_STRING(Id, "id");
    GO_PROPERTY_STRING(Type, "type");
    GO_PROPERTY_STRING(Title, "title");
    GO_PROPERTY_STRING(Image, "image");
    GO_PROPERTY_STRING(Url, "url");
    GO_PROPERTY_STRING(Description, "description");
    
    GO_PROPERTY_OBJECT(GraphObject, Data, "data");
};

class OpenGraphAction : public GraphObject {
public:
    GO_CREATE(OpenGraphAction);
    GO_CREATE_EMPTY(OpenGraphAction);
    
    GO_PROPERTY_STRING(Id, "id");
    GO_PROPERTY_STRING(StartTime, "start_time");
    GO_PROPERTY_STRING(EndTime, "end_time");
    GO_PROPERTY_STRING(PublishTime, "publish_time");
    GO_PROPERTY_STRING(CreatedTime, "created_time");
    GO_PROPERTY_STRING(ExpiresTime, "expires_time");
    GO_PROPERTY_STRING(Ref, "ref");
    GO_PROPERTY_STRING(Message, "message");
    
    GO_PROPERTY_OBJECT(GraphUser, From, "from");
    GO_PROPERTY_OBJECT(GraphApplication, Application, "application");
    
    void setTags(const vector<string> &tags);
    vector<string> getTags();
};

class ShareDialogParams : public GraphObject {
public:
    GO_CREATE_EMPTY(ShareDialogParams);
    
    GO_PROPERTY_STRING(Link, "link");
    GO_PROPERTY_STRING(Name, "name");
    GO_PROPERTY_STRING(Caption, "caption");
    GO_PROPERTY_STRING(Description, "description");
    GO_PROPERTY_STRING(Picture, "picture");
    GO_PROPERTY_STRING(Ref, "ref");
    GO_PROPERTY_BOOL(DataFailuresFatal, "dataFailuresFatal");
    
    void setFriends(const list<string> &friends);
    list<string> getFriends();
};

class OpenGraphActionShareDialogParams : public GraphObject {
public:
    GO_CREATE_EMPTY(OpenGraphActionShareDialogParams);
    
    GO_PROPERTY_OBJECT(OpenGraphAction, Action, "action");
    GO_PROPERTY_STRING(PreviewPropertyName, "previewPropertyName");
    GO_PROPERTY_STRING(ActionType, "actionType");
};

NS_SCREW_FACEBOOK_END

#endif /* defined(__Screw__OpenGraphObject__) */
