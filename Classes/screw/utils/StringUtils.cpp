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
    converter.precision(7);
    converter.setf(std::ios::fixed);
    converter << number;
    return converter.str();
}

string StringUtils::toString(double number){
    ostringstream converter;
    converter.precision(16);
    converter.setf(std::ios::fixed);
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