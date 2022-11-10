#include <ch18/exercise.h>
#include <boost/ut.hpp>
#include <string_view>

int main()
{
    using namespace boost::ut;
    using namespace std::literals;

    "exercise 3"_test = [] {
        const char a1[] = "abc";
        const char b1[] = "abcd";
        expect(my::strcmp(a1, b1) < 0_i);
        const char a2[] = "abb";
        const char b2[] = "abc";
        expect(my::strcmp(a2, b2) < 0_i);
        const char a3[] = "abc";
        const char b3[] = "abc";
        expect(my::strcmp(a3, b3) == 0_i);
        const char a4[] = "abd";
        const char b4[] = "abc";
        expect(my::strcmp(a4, b4) > 0_i);
        const char a5[] = "abcd";
        const char b5[] = "abc";
        expect(my::strcmp(a5, b5) > 0_i);
    };

    "exercise 2"_test = [] {
        const char p[] = "exercise";
        const char x1[] = "xe";
        auto found = my::findx(p, x1);
        expect(found == p+1);
    };

    "exercise 1"_test = [] {
        const char p[] = "exercise";
        auto po = my::strdup(p);

        std::string_view svp{p};
        std::string_view svpo{po};
        expect(svp == svpo);
        delete[] po;
    };

    "matchx"_test = [] {
        const char s1[] = "abc";
        const char x1[] = "abcd";
        expect(my::matchx(s1, x1) == false);
        const char s2[] = "abb";
        const char x2[] = "abc";
        expect(my::matchx(s2, x2) == false);
        const char s3[] = "abc";
        const char x3[] = "abc";
        expect(my::matchx(s3, x3) == true);
        const char s4[] = "abd";
        const char x4[] = "abc";
        expect(my::matchx(s4, x4) == false);
        const char s5[] = "abcd";
        const char x5[] = "abc";
        expect(my::matchx(s5, x5) == true);
    };

    "strlen"_test = [] {
        const char p[] = "exercise";
        auto len = my::strlen(p);
        expect(len == 8);
    };
}
