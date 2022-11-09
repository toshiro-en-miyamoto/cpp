#include <ch18/exercise.h>
#include <boost/ut.hpp>
#include <string_view>

int main()
{
    using namespace boost::ut;
    using namespace std::literals;

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

    "strcmp"_test = [] {
        const char a1[] = "abc";
        const char b1[] = "abcd";
        expect(my::strcmp(a1, b1) == 3);
        const char a2[] = "ab_";
        const char b2[] = "abc";
        expect(my::strcmp(a2, b2) == 0);
        const char a3[] = "abc";
        const char b3[] = "abc";
        expect(my::strcmp(a3, b3) == 3);
    };

    "strlen"_test = [] {
        const char p[] = "exercise";
        auto len = my::strlen(p);
        expect(len == 8);
    };
}
