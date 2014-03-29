/*
 * StringUtils.h
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

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
