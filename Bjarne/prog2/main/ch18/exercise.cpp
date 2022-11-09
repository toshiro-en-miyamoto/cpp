#include <ch18/exercise.h>

namespace my {

// exercise 2
char* findx(const char* s, const char* x)
{
    if (s == nullptr) return nullptr;
    if (x == nullptr) return nullptr;

    auto s_len = strlen(s);
    if (s_len == 0) return nullptr;

    auto x_len = strlen(x);
    if (x_len == 0) return nullptr;

    auto ps = s;
    while (ps - s <= x_len) {
        auto px = x;
        if (strcmp(ps, px) == x_len) {
            return const_cast<char*>(ps);
        } else {
            ++ps;
        }
    }

    return nullptr;
}

// exercise 1
char* strdup(const char* s)
{
    if (s == nullptr) return nullptr;

    auto len = strlen(s);
    if (len == 0) return nullptr;

    auto p = new char[len];
    auto dup = p;
    while (*s != 0) {
        *p = *s;
        ++p;
        ++s;
    }
    *p = *s;    // copy the trailing zero

    return dup;
}

std::size_t strcmp(const char* a, const char* b)
{
    if (a == nullptr) return 0;
    if (b == nullptr) return 0;

    auto a_len = strlen(a);
    auto b_len = strlen(b);
    auto len = a_len < b_len ? a_len : b_len;

    decltype(len) i = 0;
    while (i < len) {
        if (a[i] == b[i]) {
            ++i;
        } else {
            break;
        }
    }
    return i == len ? len : 0;
}

std::size_t strlen(const char* s)
{
    if (s == nullptr) return 0;
    auto s_begin = s;
    while (*s != 0) ++s;
    return s - s_begin;
}

}
