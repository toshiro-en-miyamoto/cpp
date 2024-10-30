// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex10.cpp
/*
  Observe what happens when the number gets too
  large to represent exactly as an int and as a double.
*/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include <limits>

TEST_CASE("range of int") {
  const auto int_max_plus_1 = std::numeric_limits<int>::max() + 1;
  CHECK(int_max_plus_1 == std::numeric_limits<int>::lowest());

  const auto int_lowest_minus_1 = std::numeric_limits<int>::lowest() - 1;
  CHECK(int_lowest_minus_1 == std::numeric_limits<int>::max());
}

TEST_CASE("range of unsigned int") {
  const auto uint_max_plus_1 = std::numeric_limits<unsigned int>::max() + 1;
  CHECK(uint_max_plus_1 == 0U);

  const auto uint_lowest_minus_1 = std::numeric_limits<unsigned int>::lowest() - 1;
  CHECK(uint_lowest_minus_1 == std::numeric_limits<unsigned int>::max());
}

TEST_CASE("range of double") {
  const auto double_max_times_2 = std::numeric_limits<double>::max() * 2.0;
  CHECK(double_max_times_2 == std::numeric_limits<double>::infinity());

  const auto double_min_over_4 = std::numeric_limits<double>::min() / 2.0;
  CHECK(double_min_over_4 < std::numeric_limits<double>::min());
}
