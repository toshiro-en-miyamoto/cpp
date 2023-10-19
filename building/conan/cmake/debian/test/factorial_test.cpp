#include <catch2/catch_test_macros.hpp>
#include "factorial.hpp"

TEST_CASE("factorials", "[factorial]")
{
    CHECK(  1 == factorial(0));
    CHECK(  1 == factorial(1));
    CHECK(  2 == factorial(2));
    CHECK(  6 == factorial(3));
    CHECK( 24 == factorial(4));
    CHECK(120 == factorial(5));
}
