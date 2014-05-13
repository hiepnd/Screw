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

#include "PhotoLoader.h"
#include "screw/screw.h"

using namespace screw;

NS_SCREW_FACEBOOK_BEGIN

PhotoLoader *PhotoLoader::_instance = nullptr;
const string PhotoLoaderLoadedNotification = "PhotoLoaderLoadedNotification";

PhotoLoader *PhotoLoader::getInstance() {
    if (!_instance) {
        _instance = new PhotoLoader();
    }
    
    return _instance;
}

PhotoLoader::PhotoLoader() {
    if (!utils::FileUtils::isFileExist(utils::FileUtils::getDocumentPath("facebook-images"))) {
        utils::FileUtils::createDirectory("facebook-images");
    }
}

void PhotoLoader::download(const string &uid, int size) {
    if (_dowloadings.find(uid) != _dowloadings.end()) {
        return;
    }
    _dowloadings.insert(uid);
    HttpRequest *r = new HttpRequest();
    r->setRequestType(HttpRequest::Type::GET);
    string url = string("http://graph.facebook.com/") + uid + "/picture?width=" +
                            utils::StringUtils::toString(size) + "&height=" + utils::StringUtils::toString(size);
    r->setUrl(url.c_str());
    r->setResponseCallback(this, (SEL_HttpResponse)&PhotoLoader::httpCallback);
    HttpClient::getInstance()->send(r);
    r->setTag(uid.c_str());
    r->release();
}

void PhotoLoader::httpCallback(HttpClient* client, HttpResponse* response) {
    _dowloadings.erase(response->getHttpRequest()->getTag());
    CCLOG("%s, code = %ld, error = %s", __func__, response->getResponseCode(),
          response->getErrorBuffer() ? response->getErrorBuffer() : "(null)");
    if (response->getResponseCode() == 200) {
        string imageData(response->getResponseData()->begin(), response->getResponseData()->end());
        utils::FileUtils::createFile((unsigned char *)imageData.c_str(), response->getResponseData()->size(),
                                     utils::FileUtils::getDocumentPath(string("facebook-images/")
                                                                       + response->getHttpRequest()->getTag()
                                                                       + ".png"));
        PhotoLoaderEvent ev(PhotoLoaderLoadedNotification, response->getHttpRequest()->getTag());
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&ev);
    }
}

Texture2D *PhotoLoader::loadTexture(const string &uid) {
    string path = utils::FileUtils::getDocumentPath("facebook-images/" + uid + ".png");
    if (utils::FileUtils::isFileExist(path)) {
        Image *image = new Image();
        if (image->initWithImageFile(path)) {
            Texture2D *texture = new Texture2D();
            if (texture->initWithImage(image)) {
                texture->autorelease();
                image->release();
                return texture;
            }
            texture->release();
        }
        image->release();
    }
    
    return Director::getInstance()->getTextureCache()->addImage("fb-default.png");
}

bool PhotoLoader::isPhotoExist(const string &uid) {
    return utils::FileUtils::isFileExist(utils::FileUtils::getDocumentPath("facebook-images/" + uid + ".png"));
}

NS_SCREW_FACEBOOK_END