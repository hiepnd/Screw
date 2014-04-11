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

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_
#include "../macros.h"
#include <vector>
#include <string>
#include <iterator>
#include <list>
#include <numeric>

using namespace std;

NS_SCREW_UTILS_BEGIN

class StringUtils {
public:
	static vector<string> split(const string &s, const string &sep);
    static string join(const vector<string> &v, const string &sep);
    static string join(const list<string> &v, const string &sep);
    
    template<class InputIterator>
    static string join(InputIterator first, InputIterator last, const string &sep) {
        if (first == last) {
            return "";
        }
        
        string init = *first;
        first++;
        return std::accumulate(first, last, init, [&sep](const string &acc,  const string &elm){
            return acc + sep + elm;
        });
    }
    
    static string toString(int number);
    static string toString(float number);
    static string toString(double number);
    static string toString(long number);
    static string toString(bool number);
};

NS_SCREW_UTILS_END

#endif /* STRINGUTILS_H_ */
