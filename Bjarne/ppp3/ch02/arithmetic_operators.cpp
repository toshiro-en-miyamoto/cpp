// g++ -std=c++20 arithmetic_operators.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include <utility>

[[nodiscard]]
std::tuple<double, double, double, double, double, double>
operators_for_double(double n)
{
  return {
    n+1,
    3*n,
    n+n,
    n*n,
    n/2,
    std::sqrt(n)
  };
}

TEST_CASE("operators for double")
{
  auto [inc_by_1, time_3, twice, squared, half, root]
  = operators_for_double(1.21);
  CHECK(inc_by_1 == 2.21);
  CHECK(time_3 == 3.63);
  CHECK(twice == 2.42);
  CHECK(squared == 1.4641);
  CHECK(half == 0.605);
  CHECK(root == 1.1);
}

[[nodiscard]]
std::tuple<int, int, int, int, int, double, int>
operators_for_int(int n)
{
  return {
    n+1,
    3*n,
    n+n,
    n*n,
    n/2,
    std::sqrt(n),
    n%2
  };
}

TEST_CASE("operators for int")
{
  auto [inc_by_1, time_3, twice, squared, half, root, modulo2]
  = operators_for_int(9);
  CHECK(inc_by_1 == 10);
  CHECK(time_3 == 27);
  CHECK(twice == 18);
  CHECK(squared == 81);
  CHECK(half == 4);
  CHECK(root == 3.0);
  CHECK(modulo2 == 1);
}
