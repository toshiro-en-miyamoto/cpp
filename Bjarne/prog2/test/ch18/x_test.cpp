#include <ch18/x.h>
#include <boost/ut.hpp>
#include <sstream>

int main()
{
    using namespace boost::ut;

    "local variable"_test = [] {
        std::ostringstream local;
        auto cout_buff = std::cout.rdbuf();
        std::cout.rdbuf(local.rdbuf());
        {
            my::X loc {4};
        }
        std::cout.rdbuf(cout_buff);
        expect(local.str() ==
            "X(int):4(4)\n"
            "~X():4(0)\n"
        ) << local.str();
    };
    "copy construction"_test = [] {
        my::X loc {4};

        std::ostringstream local;
        auto cout_buff = std::cout.rdbuf();
        std::cout.rdbuf(local.rdbuf());
        {
            my::X loc2 {loc};
        }
        std::cout.rdbuf(cout_buff);
        expect(local.str() ==
            "X(X&):4(4)\n"
            "~X():4(0)\n"
        ) << local.str();
    };
    "copy assignment"_test = [] {
        my::X loc {4};

        std::ostringstream local;
        auto cout_buff = std::cout.rdbuf();
        std::cout.rdbuf(local.rdbuf());
        {
            loc = my::X {5};
        }
        std::cout.rdbuf(cout_buff);
        expect(local.str() ==
            "X(int):5(5)\n"
            "X::operator=():4(5)\n"
            "~X():5(0)\n"
        ) << local.str();
    };
}