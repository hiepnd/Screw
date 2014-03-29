/*
 * StringUtils.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "StringUtils.h"
#include <sstream>

NS_SCREW_UTILS_BEGIN

vector<string> StringUtils::split(const string &str, const string &sep) {
    vector<string> tokens;
    
    int start = 0, end = 0;
    while ((end = str.find(sep, start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + sep.length();
    }
    tokens.push_back(str.substr(start));
    
    return tokens;
}

string StringUtils::join(const vector<string> &v, const string &sep) {
    return StringUtils::join(v.begin(), v.end(), sep);
}

string StringUtils::join(const list<string> &v, const string &sep) {
    return StringUtils::join(v.begin(), v.end(), sep);
}

string StringUtils::toString(int number){
    ostringstream converter;
    converter << number;
    return converter.str();
}

string StringUtils::toString(float number){
    ostringstream converter;
    converter << number;
    return converter.str();
}

string StringUtils::toString(double number){
    ostringstream converter;
    converter << number;
    return converter.str();
}

string StringUtils::toString(long number){
    ostringstream converter;
    converter << number;
    return converter.str();
}

string StringUtils::toString(bool number){
    return number ? string("1") : string("0");
}

NS_SCREW_UTILS_END