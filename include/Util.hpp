#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

namespace Util {
    char readCharacter(char &input);
    bool isNumber(const std::string& str);
    void lowercase(std::string &str);
    int numDigits(const int32_t x);
    void print_vec(std::vector<std::vector<int>> v);
};

#endif