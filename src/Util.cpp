#include "Util.hpp"
#include <iostream>
#include <numeric>
#include <algorithm>


// https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed
#ifdef linux
#include <unistd.h>
#include <termios.h>
#endif
    
char Util::readCharacter(char &input) {
    #ifdef linux
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return buf;
    #else
        std::cin >> input;
        return input;
    #endif
}

// Simple cmd line parsing function to determine if a string is representing a number
bool Util::isNumber(const std::string& str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) {
            return false;
        }
    }
    return true;
}

// Simple function to convert a string to lowercase
void Util::lowercase(std::string &str) {
    std::transform(str.begin(), str.end(), str.begin(), 
        [](unsigned char c){ return std::tolower(c); });
}

/**
 * Binary search function for determining the number of digits in a number
 * (more efficient than the log method) 
 * from: https://stackoverflow.com/questions/1489830/efficient-way-to-determine-number-of-digits-in-an-integer
 */
int Util::numDigits(int32_t x)
{
    if (x >= 10000) {
        if (x >= 10000000) {
            if (x >= 100000000) {
                if (x >= 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if (x >= 100000) {
            if (x >= 1000000)
                return 7;
            return 6;
        }
        return 5;
    }
    if (x >= 100) {
        if (x >= 1000)
            return 4;
        return 3;
    }
    if (x >= 10)
        return 2;
    return 1;
}

/**
 * Function to print 2D vectors (for debugging purposes)
 */
void Util::print_vec(std::vector<std::vector<int>> v) {
    for (int i = 0; i < (int) v.size(); ++i) {
        for (int j = 0; j < (int) v[i].size(); ++j) {
            std::cout << v[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
