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

#ifndef __Screw__RequestScene__
#define __Screw__RequestScene__

#include <iostream>
#include "screw/screw.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_SCREW;
USING_NS_SCREW_FACEBOOK;
USING_NS_CC_EXT;

enum {
    RequestTypeInvite = 1,
    RequestTypeSendFish,
    RequestTypeAskforFish,
};

class RequestCell : public TableViewCell {
public:
    RequestCell();
    void update(GraphRequest *request);
    void updateAvatar();
    GraphRequest *getRequest();
private:
    Label *_message;
    Label *_action;
    Sprite *_avatar;
    GraphRequest *_request;
};

class RequestTable : public Node, public TableViewDataSource, public TableViewDelegate {
public:
    RequestTable();
    
    virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void scrollViewDidScroll(ScrollView* view) {}
    virtual void scrollViewDidZoom(ScrollView* view) {}
    
    void sendAFish(const string &uid, const string &rid);
    void eateAFish();
    void reload();
    void updateFishCount();
    
    void onPhotoDownloaded(EventCustom *event);
    void onExit() override;
    
private:
    TableView *_table;
    Vector<GraphRequest *> _requests;
    Label *_fishCount;
    Label *_empty;
};

class RequestScene : public Scene {
public:
    static RequestScene *create() {
        RequestScene *scene = new RequestScene();
        scene->init();
        scene->autorelease();
        
        return scene;
    }
    
    bool init();
    void onEnter() override;
    void onExit() override;
    
private:
    void fetchRequests();
    void sendAFish();
    void askforAFish();
    
    void onRequestsFetched();
    
    RequestTable *_table;
};

#endif /* defined(__Screw__RequestScene__) */
