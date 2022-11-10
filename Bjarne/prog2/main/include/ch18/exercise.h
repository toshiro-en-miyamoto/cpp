#pragma once

#include <cstddef>

namespace my {

int strcmp(const char* s1, const char* s2);
char* findx(const char* s, const char* x);
char* strdup(const char* s);
bool matchx(const char* s, const char* x);
std::size_t strlen(const char* s);

}
