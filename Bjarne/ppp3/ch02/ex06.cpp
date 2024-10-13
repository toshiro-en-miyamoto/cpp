// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex06.cpp
/*
  Provided with three integer values, and then outputs the values
  in numerical sequence separated by commas. So, if the user enters
  the values 10 4 6, the output should be 4, 6, 10. If two values
  are the same, they should just be ordered together. So, the
  input 4 5 4 should give 4, 4, 5.

  Sorting approach: Selection Sort
  (https://www.geeksforgeeks.org/selection-sort-algorithm-2/)
  It sorts an array by repeatedly selecting the smallest (or
  largest) element from the unsorted portion and swapping it with
  the first unsorted element. This process continues until the
  entire array is sorted.

  Test cases for three integer values:
  For simple analysis, let possible values be 1, 2, and 3.
  Then we have following arrays as the initial state:
*/

  //        init    expected
  //  (01)  1 1 1   1 1 1
  //  (02)  1 1 2   1 1 2
  //  (03)  1 1 3   1 1 3
  //  (04)  1 2 1   1 1 2
  //  (05)  1 2 2   1 2 2
  //  (06)  1 2 3   1 2 3
  //  (07)  1 3 1   1 1 3
  //  (08)  1 3 2   1 2 3
  //  (09)  1 3 3   1 3 3
  //  (10)  2 1 1   1 1 2
  //  (11)  2 1 2   1 2 2
  //  (12)  2 1 3   1 2 3
  //  (13)  2 2 1   1 2 2
  //  (14)  2 2 2   2 2 2
  //  (15)  2 2 3   2 2 3
  //  (16)  2 3 1   1 2 3
  //  (17)  2 3 2   2 2 3
  //  (18)  2 3 3   2 3 3
  //  (19)  3 1 1   1 1 3
  //  (20)  3 1 2   1 2 3
  //  (21)  3 1 3   1 3 3
  //  (22)  3 2 1   1 2 3
  //  (23)  3 2 2   2 2 3
  //  (24)  3 2 3   2 3 3
  //  (25)  3 3 1   1 3 3
  //  (26)  3 3 2   2 3 3
  //  (27)  3 3 3   3 3 3

#include <array>

struct Test_case {
  std::array<int, 3> input;
  std::array<int, 3> expected;
};

constexpr Test_case tc[] {
  //  (01)  1 1 1   1 1 1
          {{1,1,1},{1,1,1}}
  //  (02)  1 1 2   1 1 2
        , {{1,1,2},{1,1,2}}
  //  (03)  1 1 3   1 1 3
        , {{1,1,3},{1,1,3}}
  //  (04)  1 2 1   1 1 2
        , {{1,2,1},{1,1,2}}
  //  (05)  1 2 2   1 2 2
        , {{1,2,2},{1,2,2}}
  //  (06)  1 2 3   1 2 3
        , {{1,2,3},{1,2,3}}
  //  (07)  1 3 1   1 1 3
        , {{1,3,1},{1,1,3}}
  //  (08)  1 3 2   1 2 3
        , {{1,3,2},{1,2,3}}
  //  (09)  1 3 3   1 3 3
        , {{1,3,3},{1,3,3}}
  //  (10)  2 1 1   1 1 2
        , {{2,1,1},{1,1,2}}
  //  (11)  2 1 2   1 2 2
        , {{2,1,2},{1,2,2}}
  //  (12)  2 1 3   1 2 3
        , {{2,1,3},{1,2,3}}
  //  (13)  2 2 1   1 2 2
        , {{2,2,1}, {1,2,2}}
  //  (14)  2 2 2   2 2 2
        , {{2,2,2},{2,2,2}}
  //  (15)  2 2 3   2 2 3
        , {{2,2,3},{2,2,3}}
  //  (16)  2 3 1   1 2 3
        , {{2,3,1},{1,2,3}}
  //  (17)  2 3 2   2 2 3
        , {{2,3,2},{2,2,3}}
  //  (18)  2 3 3   2 3 3
        , {{2,3,3},{2,3,3}}
  //  (19)  3 1 1   1 1 3
        , {{3,1,1},{1,1,3}}
  //  (20)  3 1 2   1 2 3
        , {{3,1,2},{1,2,3}}
  //  (21)  3 1 3   1 3 3
        , {{3,1,3},{1,3,3}}
  //  (22)  3 2 1   1 2 3
        , {{3,2,1},{1,2,3}}
  //  (23)  3 2 2   2 2 3
        , {{3,2,2},{2,2,3}}
  //  (24)  3 2 3   2 3 3
        , {{3,2,3},{2,3,3}}
  //  (25)  3 3 1   1 3 3
        , {{3,3,1},{1,3,3}}
  //  (26)  3 3 2   2 3 3
        , {{3,3,2},{2,3,3}}
  //  (27)  3 3 3   3 3 3
        , {{3,3,3},{3,3,3}}
};

#include <algorithm>

[[nodiscard]] constexpr inline
std::array<int, 3> selection_sort(std::array<int, 3> a)
{
  for (auto it_range{a.begin()}; it_range != a.end(); it_range++) {
    auto it_finder{it_range};
    auto it_min{std::min_element(it_finder, a.end())};
    std::swap(*it_finder, *it_min);
  }
  return a;
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"

TEST_CASE("27 test cases")
{
  constexpr std::size_t elements = sizeof tc / sizeof (Test_case);

  for (std::size_t i = 0; i < elements; i++) {
    CHECK(selection_sort(tc[i].input) == tc[i].expected);
  }
}
