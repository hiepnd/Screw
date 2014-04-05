/*
 * FileUtils.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#ifndef FILEUTILS_H_
#define FILEUTILS_H_
#include "../macros.h"
#include "cocos2d.h"
#include <string>

NS_SCREW_UTILS_BEGIN
using namespace std;

class FileUtils {
public:
	static string getResourcePath(const string &path);
    static string getDocumentPath(const string &path);
};

NS_SCREW_UTILS_END

#endif /* FILEUTILS_H_ */
