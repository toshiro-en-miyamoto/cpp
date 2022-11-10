#include <ch18/exercise.h>
#include <stdexcept>

namespace my {

// exercise 3
/*
 * Compares C-style strings. Let it return a negative number
 * if s1 is lexicographically before s2, zero if s1 equals s2,
 * and a positive number if s1 is lexicographically after s2.
 * 
 * The lexicographical order on the set of all these finite
 * words orders the words as follows:
 *
 * 1. Given two different words of the same length, say
 *      a = a1a2...ak and b = b1b2...bk, the order of the two
 *      words depends on the alphabetic order of the symbols in
 *      the first place i where the two words differ (counting
 *      from the beginning of the words):
 * 
 *              a < b if and only if ai < bi
 * 
 *      in the underlying order of the alphabet A.
 *
 * 2. If two words have different lengths, the usual
 *      lexicographical order pads the shorter one with "blanks"
 *      (a special symbol that is treated as smaller than every
 *      element of A) at the end until the words are the same
 *      length, and then the words are compared as in the
 *      previous case.
 */

int strcmp(const char* s1, const char* s2)
{
    if (s1 == nullptr) throw std::invalid_argument("s1");
    if (s2 == nullptr) throw std::invalid_argument("s2");

    const auto len1 = strlen(s1);
    const auto len2 = strlen(s2);
    if (len1 == 0 && len2 == 0) return 0;

    const auto len = len1 < len2 ? len1 : len2;
    auto s_begin = s1;
    while(*s1 == *s2 && s1 - s_begin < len) {
        // do {*s1 == *s2} before {s1 - s_begin < len}
        // because the case (len1 != len2) is allowed such as
        //      s1="abc" and s2="abcd"
        // we need to check if (s1[len] == s2[len])
        //      s1[len] := 0x00 and s2[len] := 'd'
        // otherwise strcmp() would tell that s1 equals s2
        ++s1;
        ++s2;
    }
    // a negative if *s1 < *s2
    // zero       if *s1 == *s2
    // a positive if *s1 > *s2
    return *s1 - *s2;
}


// exercise 2
char* findx(const char* s, const char* x)
{
    if (s == nullptr) return nullptr;
    if (x == nullptr) return nullptr;

    const auto s_len = strlen(s);
    if (s_len == 0) return nullptr;

    const auto x_len = strlen(x);
    if (x_len == 0) return nullptr;

    auto ps = s;
    while (ps - s <= x_len) {
        auto px = x;
        if (matchx(ps, px)) {
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

    const auto len = strlen(s);
    if (len == 0) return nullptr;

    auto p = new char[len];
    const auto dup = p;
    while (*s != 0) {
        *p = *s;
        ++p;
        ++s;
    }
    *p = *s;    // copy the trailing zero

    return dup;
}

bool matchx(const char* s, const char* x)
{
    // assuming both s and x are C-style string
    // if (s == nullptr) return 0;
    // if (x == nullptr) return 0;

    const auto s_len = strlen(s);
    const auto x_len = strlen(x);
    if (s_len < x_len) return false;

    auto x_begin = x;
    while (*s == *x && x - x_begin < x_len) {
        ++s;
        ++x;
    }

    return x - x_begin == x_len;
}

std::size_t strlen(const char* s)
{
    // assuming both s and x are C-style string
    // if (s == nullptr) return 0;
    const auto s_begin = s;
    while (*s != 0) ++s;
    return s - s_begin;
}

}
