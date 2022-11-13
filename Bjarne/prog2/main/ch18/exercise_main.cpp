#include <ch18/exercise.h>
#include <string_view>
#include <cassert>

void exercise8()
{
    const char s1[] = "home";
    const auto r1 = ch18::is_palindrome(s1, s1 + ch18::strlen(s1) - 1);
    const char s2[] = "malayalam";
    const auto r2 = ch18::is_palindrome(s2, s2 + ch18::strlen(s2) - 1);
}

void exercise3()
{
    const char a1[] = "abc";
    const char b1[] = "abcd";
    const auto r1 = ch18::strcmp(a1, b1);
    const char a2[] = "abb";
    const char b2[] = "abc";
    const auto r2 = ch18::strcmp(a2, b2);
    const char a3[] = "abc";
    const char b3[] = "abc";
    const auto r3 = ch18::strcmp(a3, b3);
    const char a4[] = "abd";
    const char b4[] = "abc";
    const auto r4 = ch18::strcmp(a4, b4);
    const char a5[] = "abcd";
    const char b5[] = "abc";
    const auto r5 = ch18::strcmp(a5, b5);
}

void exercise2()
{
    const char p[] = "exercise";
    const char x1[] = "xe";
    auto found = ch18::findx(p, x1);
    assert(found == p+1);
}

void exercise1()
{
    constexpr char pi[] = "exercise";
    const std::string_view svi{pi};

    char* po = ch18::strdup(pi);
    std::string_view svo{po};
    assert(svi == svo);
    delete[] po;
}

void matchx()
{
    const char a1[] = "abc";
    const char b1[] = "abcd";
    const auto r1 = ch18::matchx(a1, b1);
    const char a2[] = "ab_";
    const char b2[] = "abc";
    const auto r2 = ch18::matchx(a2, b2);
    const char a3[] = "abc";
    const char b3[] = "abc";
    const auto r3 = ch18::matchx(a3, b3);
    const char a4[] = "abcd";
    const char b4[] = "abc";
    const auto r4 = ch18::matchx(a4, b4);
}

int main()
{
    exercise8();
    exercise3();
    exercise2();
    exercise1();
    matchx();
}
