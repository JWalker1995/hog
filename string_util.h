#ifndef TRIM_H
#define TRIM_H

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

namespace string_util {

// Kind of copied from here:
// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

// trim from start
static void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end
static void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends
static void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

}

#endif // TRIM_H
