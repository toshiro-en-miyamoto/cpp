#include <gtest/gtest.h>
#include "Bjarne/prog2/ch03/main/greet.h"
#include <string>
#include <sstream>

TEST(ch03, greet)
{
    std::istringstream input { "Bjarne 24" };
    std::string expected { "Hello, Bjarne (24)" };

    std::ostringstream actual;
    greet(input, actual);

    EXPECT_EQ(expected, actual.str());
}
