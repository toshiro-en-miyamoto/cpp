#include <catch2/catch_test_macros.hpp>
#include "calc.h"

TEST_CASE("SumAddsTwoInts", "[calc]")
{
    Calc sut;
    CHECK(5 == sut.Sum(2, 3));
}
TEST_CASE("MultiplyMultipliesTwoInts", "[calc]")
{
    Calc sut;
    CHECK(12 == sut.Multiply(3, 4));
}
