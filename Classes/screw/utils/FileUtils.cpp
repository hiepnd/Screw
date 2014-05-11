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

#include "FileUtils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>

NS_SCREW_UTILS_BEGIN

string FileUtils::getResourcePath(const string &path) {
    return cocos2d::FileUtils::getInstance()->fullPathForFilename(path);
}

string FileUtils::getDocumentPath(const string &path) {
    return cocos2d::FileUtils::getInstance()->getWritablePath() + "/" + path;
}

bool FileUtils::createDirectory(const string &dirname){
    string path = cocos2d::FileUtils::getInstance()->getWritablePath() + string("/") + dirname;
    int ret = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    return ret == 0;
}

bool FileUtils::isFileExist(const string &fileOrDirectory){
    return cocos2d::FileUtils::getInstance()->isFileExist(fileOrDirectory);
}

bool FileUtils::createFile(const unsigned char *buff, long size, const string &filepath) {
    std::ofstream ofs(filepath, std::ofstream::binary);
    if (ofs.fail()) {
        CCLOG("FileUtils::createFile - failed to create %s", filepath.c_str());
        return false;
    }
    
    ofs.write((const char *)buff, size);
    ofs.close();
    
    if (ofs.fail()) {
        CCLOG("FileUtils::createFile - something wrong when write %s", filepath.c_str());
    }
    
    return true;
}

bool FileUtils::createFile(const char *buff, const string &filepath) {
    std::ofstream ofs(filepath, std::ofstream::binary);
    if (ofs.fail()) {
        CCLOG("FileUtils::createFile - failed to create %s", filepath.c_str());
        return false;
    }
    
    ofs.write(buff, strlen(buff));
    ofs.close();
    
    return true;
}
NS_SCREW_UTILS_END