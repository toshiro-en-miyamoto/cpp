#pragma once

#include <cstddef>
#include <string>

namespace my {

bool is_palindrome(const char* first, const char* last);
char* cat(const char* s1, const char* s2, const char* sep);
std::string cat(const std::string& s1, const std::string& s2, const std::string& sep);
std::string cat_dot(const std::string& s1, const std::string& s2);
int strcmp(const char* s1, const char* s2);
char* findx(const char* s, const char* x);
char* strdup(const char* s);
bool matchx(const char* s, const char* x);
std::size_t strlen(const char* s);

}
