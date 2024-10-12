// g++ -std=c++20 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion narrowing.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include <utility>

[[nodiscard]]
std::pair<int, char> narrowing(double d)
{
  int i = static_cast<int>(d);
  char c = static_cast<char>(i);

  // with -Wconversion option,
  // warning: conversion from ‘double’ to ‘int’ may change value
  //    int i = d;
  // warning: conversion from int to char may change value
  //    char c = i;
  return {i, c};
}

TEST_CASE("negative large values")
{
  // -256 <= d
  auto [i1, c1] = narrowing(-260.8);
  CHECK(i1 == -260);  // 0b_1111_1110_1111_1100
  CHECK(c1 ==  252);  // 0b_1111_1100 = 252
  // d < -256
  auto [i2, c2] = narrowing(-967.7);
  CHECK(i2 == -967);  // 0b_1111_1100_0011_1001
  CHECK(c2 ==   57);  // 0b_0011_1001 = 57
}

TEST_CASE("negative small values")
{
  auto [i1, c1] = narrowing(-2.6);
  CHECK(i1 ==   -2);  // -b_1111_1111_1111_1110
  CHECK(c1 ==  254);  // 0b_1111_1110 = (unsigned char) 254
}

TEST_CASE("large values")
{
  // greater than 255
  auto [i1, c1] = narrowing(257.8);
  CHECK(i1 == 257);   // 0b_0000_0001_0000_0001
  CHECK(c1 ==   1);   // 0b_0000_0001 = (unsigned char) 1
  // less than 256
  auto [i2, c2] = narrowing(235.7);
  CHECK(i2 == 235);   // 0b_0000_0000_1110_1011
  CHECK(c2 == 235);   // 0b_1110_1011 = (unsigned char) 235
}

TEST_CASE("small values")
{
  auto [i1, c1] = narrowing(2.6);
  CHECK(i1 == 2);
  CHECK(c1 == 2);
}
