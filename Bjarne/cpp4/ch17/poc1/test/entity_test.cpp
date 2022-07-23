#include <gtest/gtest.h>
#include "Bjarne/cpp4/ch17/poc1/main/entity.h"
#include <functional>
#include <string_view>
#include <sstream>
#include <iostream>

void simple(
    std::function<void(void)> code,
    std::string_view expected
) {
    // https://en.cppreference.com/w/cpp/io/basic_ios/rdbuf
    std::ostringstream sout;
    auto cout_buf = std::cout.rdbuf();
    std::cout.rdbuf(sout.rdbuf());

    code();

    std::cout.rdbuf(cout_buf);
    EXPECT_EQ(sout.str(), expected);
}


TEST(ch17, move)
{
    std::function<void(void)> code = []() {
        Entity e1 {"foo"};
        e1 = f(e1);
        Entity e2 {"bar"};
        e1 = e2;
    };
    std::string_view expected {
        "ctor: foo\n"
        "copy ctor: foo\n"
        "move ctor: foo\n"
        "move asign: foo\n"
        "dtor: \n"
        "dtor: \n"
        "ctor: bar\n"
        "copy asign: bar\n"
        "dtor: bar\n"
        "dtor: bar\n"
    };
    simple(code, expected);
}
