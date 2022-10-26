#include "hello.h"
#include <boost/ut.hpp>

int main()
{
    using namespace boost::ut;

    "Hello"_test = [] {
        auto actual = my::greeting("world");
        expect("Hello world" == actual);
    };
}
