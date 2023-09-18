#include "StringUtils.hpp"

namespace Morph {

void ltrim(string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(string &s) {
    ltrim(s);
    rtrim(s);
}

string ltrim_copy(string s) {
    ltrim(s);
    return s;
}

string rtrim_copy(string s) {
    rtrim(s);
    return s;
}

string trim_copy(string s) {
    trim(s);
    return s;
}

bool string_starts_with(const string& src, const string& start)
{
    return src.rfind(start, 0) == 0;
}

}