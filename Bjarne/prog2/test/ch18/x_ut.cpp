#include <ch18/x.h>
#include <boost/ut.hpp>
#include <sstream>

namespace my {

X copy(X a) { return a; }
X copy2(X a) { X aa = a; return aa; }
X& ref_to(X& a) { return a; }
X* make(int i) { X a(i); return new X(a); }
struct XX { X a; X b; };

}

int main()
{
    using namespace boost::ut;

    "five essential operations"_test = [] {
        std::ostringstream test_out;
        auto cout_buff = std::cout.rdbuf();
        std::cout.rdbuf(test_out.rdbuf());

        {
            std::cout << "local variable\n";
            my::X loc {4};

            std::cout << "copy construction\n";
            my::X loc2 {loc};

            std::cout << "copy assignment\n";
            loc = my::X {5};

            std::cout << "copy by value and return\n";
            loc2 = copy(loc);
        }

        std::cout.rdbuf(cout_buff);
        expect(
            test_out.str() ==
            "local variable\n"
                "X(int):4(4)\n"

            "copy construction\n"
                "X(X&):4(4)\n"

            "copy assignment\n"
                "X(int):5(5)\n"         // constructing rvalue X{5}
                "X::operator=():4(5)\n"
                "~X():5(0)\n"           // the rvalue is a temporary

            "copy by value and return\n"
                "X(X&):5(5)\n"          // copying the arg value
                "X(X&):5(5)\n"          // copying the ret value
                "X::operator=():4(5)\n"
                "~X():5(0)\n"           // the copy of ret is a temporary
                "~X():5(0)\n"           // the copy of arg is a temporary

            "~X():5(0)\n"               // loc2
            "~X():5(0)\n"               // loc
        ) << test_out.str();
    };
}