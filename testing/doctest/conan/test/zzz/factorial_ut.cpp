#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("testing the factorial function") {
  CHECK(factorial2(0) == 1);
  CHECK(factorial2(1) == 1);
  CHECK(factorial2(2) == 2);
  CHECK(factorial2(3) == 6);
  CHECK(factorial2(10) == 3628800);
}
