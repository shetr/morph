
#ifndef MORPH_CORE_STRING_UTILS_HPP
#define MORPH_CORE_STRING_UTILS_HPP

#include "Types.hpp"

#include <algorithm> 
#include <cctype>
#include <locale>

namespace Morph {

// trim from start (in place)
void ltrim(string &s);
// trim from end (in place)
void rtrim(string &s);
// trim from both ends (in place)
void trim(string &s);
// trim from start (copying)
string ltrim_copy(string s);
// trim from end (copying)
string rtrim_copy(string s);
// trim from both ends (copying)
string trim_copy(string s);

bool string_starts_with(const string& src, const string& start);

}

#endif // MORPH_CORE_STRING_UTILS_HPP