#include <gtest/gtest.h>
#include "Bjarne/cpp4/ch17/poc1/main/entity.h"
#include <functional>
#include <string_view>
#include <sstream>
#include <iostream>

TEST(ch17, move)
{
    std::ostringstream sout;
    auto cout_buf = std::cout.rdbuf();
    std::cout.rdbuf(sout.rdbuf());

    {
        Entity e1 {"foo"};
        e1 = f(e1);
        Entity e2 {"bar"};
        e1 = e2;
    }

    std::cout.rdbuf(cout_buf);
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
    EXPECT_EQ(sout.str(), expected);
}
