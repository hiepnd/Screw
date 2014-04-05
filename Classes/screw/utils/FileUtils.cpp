/*
 * FileUtils.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "FileUtils.h"

NS_SCREW_UTILS_BEGIN

string FileUtils::getResourcePath(const string &path) {
    return cocos2d::FileUtils::getInstance()->fullPathForFilename(path);
}

string FileUtils::getDocumentPath(const string &path) {
    return cocos2d::FileUtils::getInstance()->getWritablePath() + "/" + path;
}

NS_SCREW_UTILS_END