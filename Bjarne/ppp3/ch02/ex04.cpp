// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex04.cpp
// std::unreachable() requires c++23
/*
  Get two integer values from the input stream and store the values
  in int variables named val1 and val2.
  Determine the smaller, larger, sum, difference, product, and ratio
  of these values and report them via the output stream.

  Factors to consider test cases:
    (1) int values can be positive, zero or negative, that is
    (2) lhs can be smaller or larger than or equal to rhs
    (3) lhs/rhs
*/
  // test cases:
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (01)  pos pos  >   l/r    ==0    6   3
  // (02)  pos pos  >   l/r    !=0    5   2
  // (03)  pos pos  =    1     ==0    4   4
  // (04)  pos pos  <   l/r    !=0    2   3
  // (--)  pos pos  <   l/r    ==0     n/a
  // (05)  pos nul  >  div-0   n/a    8   0
  // (06)  pos neg  >   l/r    ==0    6  -3
  // (07)  pos neg  >   l/r    !=0    5  -2
  // (08)  nul pos  <    0     ==0    0   7
  // (09)  nul nul  =  div-0   n/a    0   0
  // (10)  nul neg  >    0     ==0    0  -4
  // (11)  neg pos  <   l/r    ==0   -9   3
  // (12)  neg pos  <   l/r    !=0   -8   5
  // (13)  neg nul  <  div-0   n/a   -7   0
  // (--)  neg neg  >   l/r    ==0     n/a
  // (14)  neg neg  >   l/r    !=0   -2  -4
  // (15)  neg neg  =    1     ==0   -3  -3
  // (16)  neg neg  <   l/r    ==0   -4  -2
  // (17)  neg neg  <   l/r    !=0   -5  -4

#include <string_view>

struct Test_case {
  const std::string_view input;
  const std::string_view expected;
};

/*
*/
constexpr static Test_case tc[] {
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (01)  pos pos  >   l/r    ==0    6   3
      { "6 3" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { " 6,  3, l>r,    9,    3,   18,   2.00" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (02)  pos pos  >   l/r    !=0    5   2
      { "5 2" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { " 5,  2, l>r,    7,    3,   10,   2.50" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (03)  pos pos  =    1     ==0    4   4
      { "4 4" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { " 4,  4, l=r,    8,    0,   16,   1.00" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (04)  pos pos  <   l/r    !=0    2   3
      { "2 3" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { " 2,  3, l<r,    5,   -1,    6,   0.67" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (05)  pos nul  >  div-0   n/a    8   0
      { "8 0" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { " 8,  0, l>r,    8,    8,    0,  div-0" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (06)  pos neg  >   l/r    ==0    6  -3
      { "6 -3" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { " 6, -3, l>r,    3,    9,  -18,  -2.00" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (07)  pos neg  >   l/r    !=0    5  -2
      { "5 -2" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { " 5, -2, l>r,    3,    7,  -10,  -2.50" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (08)  nul pos  <    0     ==0    0   7
      { "0 7" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { " 0,  7, l<r,    7,   -7,    0,   0.00" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (09)  nul nul  =  div-0   n/a    0   0
      { "0 0" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { " 0,  0, l=r,    0,    0,    0,  div-0" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (10)  nul neg  >    0     ==0    0  -4
      { "0 -4" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { " 0, -4, l>r,   -4,    4,    0,  -0.00" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (11)  neg pos  <   l/r    ==0   -9   3
      { "-9 3" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { "-9,  3, l<r,   -6,  -12,  -27,  -3.00" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (12)  neg pos  <   l/r    !=0   -8   5
      { "-8 5" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { "-8,  5, l<r,   -3,  -13,  -40,  -1.60" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (13)  neg nul  <  div-0   n/a   -7   0
      { "-7 0" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { "-7,  0, l<r,   -7,   -7,    0,  div-0" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (14)  neg neg  >   l/r    !=0   -2  -4
      { "-2 -4" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { "-2, -4, l>r,   -6,    2,    8,   0.50" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (15)  neg neg  =    1     ==0   -3  -3
      { "-3 -3" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { "-3, -3, l=r,   -6,    0,    9,   1.00" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (16)  neg neg  <   l/r    ==0   -4  -2
      { "-4 -2" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { "-4, -2, l<r,   -6,   -2,    8,   2.00" }
  },
  {
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (17)  neg neg  <   l/r    !=0   -5  -4
      { "-5 -4" },
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      { "-5, -4, l<r,   -9,   -1,   20,   1.25" }
  }
};

// report templates analysis
  //       lhs rhs <=> ratio   l%r   v1  v2
  // (01)  pos pos  >   l/r    ==0    6   3
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 6,  3, l>r,    9,    3,   18,   2.00" } lhs_gt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (02)  pos pos  >   l/r    !=0    5   2
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 5,  2, l>r,    7,    3,   10,   2.50" } lhs_gt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (03)  pos pos  =    1     ==0    4   4
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 4,  4, l=r,    8,    0,   16,   1.00" } lhs_eq_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (04)  pos pos  <   l/r    !=0    2   3
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 2,  3, l<r,    5,   -1,    6,   0.67" } lhs_lt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (05)  pos nul  >  div-0   n/a    8   0
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 8,  0, l>r,    8,    8,    0,  div-0" } lhs_gt_rhs_div0

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (06)  pos neg  >   l/r    ==0    6  -3
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 6, -3, l>r,    3,    9,  -18,  -2.00" } lhs_gt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (07)  pos neg  >   l/r    !=0    5  -2
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 5, -2, l>r,    3,    7,  -10,  -2.50" } lhs_gt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (08)  nul pos  <    0     ==0    0   7
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 0,  7, l<r,    7,   -7,    0,   0.00" } lhs_lt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (09)  nul nul  =  div-0   n/a    0   0
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 0,  0, l=r,    0,    0,    0,  div-0" } lhs_eq_rhs_div0

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (10)  nul neg  >    0     ==0    0  -4
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 0, -4, l>r,   -4,    4,    0,   0.00" } lhs_gt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (11)  neg pos  <   l/r    ==0   -9   3
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-9,  3, l<r,   -6,  -12,  -27,  -3.00" } lhs_lt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (12)  neg pos  <   l/r    !=0   -8   5
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-8,  5, l<r,   -3,  -13,  -40,  -1.60" } lhs_lt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (13)  neg nul  <  div-0   n/a   -7   0
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-7,  0, l<r,   -7,   -7,    0,  div-0" } lhs_lth_rhs_div0

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (14)  neg neg  >   l/r    !=0   -2  -4
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-2, -4, l>r,   -6,    2,    8,   0.50" } lhs_gt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (15)  neg neg  =    1     ==0   -3  -3
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-3, -3, l=r,   -6,    0,    9,   1.00" } lhs_eq_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (16)  neg neg  <   l/r    ==0   -4  -2
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-4, -2, l<r,   -6,   -2,    8,   2.00" } lhs_lt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (17)  neg neg  <   l/r    !=0   -5  -4
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-5, -4, l<r,   -9,   -1,   20,   1.25" } lhs_lt_rhs

// implementing format based on analysis of patterns
// report template pattern: lhs_gt_rhs

constexpr static std::string_view fmt_lhs_gt_rhs {
  "{:> 2d}, {:> 2d}, l>r, {:> 4d}, {:> 4d}, {:> 4d}, {:> 6.2f}"
};

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (01)  pos pos  >   l/r    ==0    6   3
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 6,  3, l>r,    9,    3,   18,   2.00" } lhs_gt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (02)  pos pos  >   l/r    !=0    5   2
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 5,  2, l>r,    7,    3,   10,   2.50" } lhs_gt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (06)  pos neg  >   l/r    ==0    6  -3
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 6, -3, l>r,    3,    9,  -18,  -2.00" } lhs_gt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (07)  pos neg  >   l/r    !=0    5  -2
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 5, -2, l>r,    3,    7,  -10,  -2.50" } lhs_gt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (10)  nul neg  >    0     ==0    0  -4
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 0, -4, l>r,   -4,    4,    0,   0.00" } lhs_gt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (14)  neg neg  >   l/r    !=0   -2  -4
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-2, -4, l>r,   -6,    2,    8,   0.50" } lhs_gt_rhs

// report template pattern: lhs_gt_rhs_div0

constexpr static std::string_view fmt_lhs_gt_rhs_div0 {
  "{:> 2d}, {:> 2d}, l>r, {:> 4d}, {:> 4d}, {:> 4d},  div-0"
};

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (05)  pos nul  >  div-0   n/a    8   0
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 8,  0, l>r,    8,    8,    0,  div-0" } lhs_gt_rhs_div0

// report template pattern: lhs_eq_rhs

constexpr static std::string_view fmt_lhs_eq_rhs {
  "{:> 2d}, {:> 2d}, l=r, {:> 4d}, {:> 4d}, {:> 4d}, {:> 6.2f}"
};

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (03)  pos pos  =    1     ==0    4   4
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 4,  4, l=r,    8,    0,   16,   1.00" } lhs_eq_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (15)  neg neg  =    1     ==0   -3  -3
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-3, -3, l=r,   -6,    0,    9,   1.00" } lhs_eq_rhs

// report template pattern: lhs_eq_rhs_div0

constexpr static std::string_view fmt_lhs_eq_rhs_div0 {
  "{:> 2d}, {:> 2d}, l=r, {:> 4d}, {:> 4d}, {:> 4d},  div-0"
};

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (09)  nul nul  =  div-0   n/a    0   0
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 0,  0, l=r,    0,    0,    0,  div-0" } lhs_eq_rhs_div0

// report template pattern: lhs_lt_rhs

constexpr static std::string_view fmt_lhs_lt_rhs {
  "{:> 2d}, {:> 2d}, l<r, {:> 4d}, {:> 4d}, {:> 4d}, {:> 6.2f}"
};

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (04)  pos pos  <   l/r    !=0    2   3
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 2,  3, l<r,    5,   -1,    6,   0.67" } lhs_lt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (08)  nul pos  <    0     ==0    0   7
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { " 0,  7, l<r,    7,   -7,    0,   0.00" } lhs_lt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (11)  neg pos  <   l/r    ==0   -9   3
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-9,  3, l<r,   -6,  -12,  -27,  -3.00" } lhs_lt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (12)  neg pos  <   l/r    !=0   -8   5
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-8,  5, l<r,   -3,  -13,  -40,  -1.60" } lhs_lt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (16)  neg neg  <   l/r    ==0   -4  -2
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-4, -2, l<r,   -6,   -2,    8,   2.00" } lhs_lt_rhs

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (17)  neg neg  <   l/r    !=0   -5  -4
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-5, -4, l<r,   -9,   -1,   20,   1.25" } lhs_lt_rhs

// report template pattern: lhs_lt_rhs_div0

constexpr static std::string_view fmt_lhs_lt_rhs_div0 {
  "{:> 2d}, {:> 2d}, l<r, {:> 4d}, {:> 4d}, {:> 4d},  div-0"
};

  //       lhs rhs <=> ratio   l%r   v1  v2
  // (13)  neg nul  <  div-0   n/a   -7   0
  //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
  //  { "-7,  0, l<r,   -7,   -7,    0,  div-0" } lhs_lth_rhs_div0

#include <sstream>
#include <format>
#include <utility>

void report_stream(std::istream& is, std::ostream& os)
{
  int i1{0}, i2{0};
  is >> i1 >> i2;
  const double d1{static_cast<double>(i1)};
  const double d2{static_cast<double>(i2)};

  if (i1 > i2) {
    if (i2 != 0) {
      // report template pattern: lhs_gt_rhs
      // (01), (02), (06), (07), (10), (14)
      //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      os << std::format(fmt_lhs_gt_rhs,
        i1, i2, i1+i2, i1-i2, i1*i2, d1/d2
      );
    } else {
      // report template pattern: lhs_gt_rhs_div0
      // (05)
      os << std::format(fmt_lhs_gt_rhs_div0,
        i1, i2, i1+i2, i1-i2, i1*i2
      );
    }
  } else if (i1 == i2) {
    if (i2 != 0) {
      // report template pattern: lhs_eq_rhs
      // (03), (15)
      //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      os << std::format(fmt_lhs_eq_rhs,
        i1, i2, i1+i2, i1-i2, i1*i2, d1/d2
      );
    } else {
      // report template pattern: lhs_eq_rhs_div0
      // (09)
      //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      os << std::format(fmt_lhs_eq_rhs_div0,
        i1, i2, i1+i2, i1-i2, i1*i2
      );
    }
  } else {
    if (i2 != 0) {
      // report template pattern: lhs_lt_rhs
      // (04), (08), (11), (12), (16), (17)
      //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      os << std::format(fmt_lhs_lt_rhs,
        i1, i2, i1+i2, i1-i2, i1*i2, d1/d2
      );
    } else {
      // report template pattern: lhs_lt_rhs_div0
      // (13)
      //  { "v1, v2, <=>,  sum, diff, prod,  ratio" }
      os << std::format(fmt_lhs_lt_rhs_div0,
        i1, i2, i1+i2, i1-i2, i1*i2
      );
      // std::unreachable();
    }
  }
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"

[[nodiscard]]
std::string report_string(const std::string_view input)
{
  std::istringstream is{std::string{input}};
  std::ostringstream os;

  report_stream(is, os);

  return os.str();
}

TEST_CASE("17 test cases")
{
  // report template pattern: lhs_gt_rhs
  // (01), (02), (06), (07), (10), (14)
  CHECK(tc[0].expected == report_string(tc[0].input));
  CHECK(tc[1].expected == report_string(tc[1].input));
  CHECK(tc[5].expected == report_string(tc[5].input));
  CHECK(tc[6].expected == report_string(tc[6].input));
  CHECK(tc[9].expected == report_string(tc[9].input));
  CHECK(tc[13].expected == report_string(tc[13].input));

  // report template pattern: lhs_gt_rhs_div0
  // (05)
  CHECK(tc[4].expected == report_string(tc[4].input));

  // report template pattern: lhs_eq_rhs
  // (03), (15)
  CHECK(tc[2].expected == report_string(tc[2].input));
  CHECK(tc[14].expected == report_string(tc[14].input));

  // report template pattern: lhs_eq_rhs_div0
  // (09)
  CHECK(tc[8].expected == report_string(tc[8].input));

  // report template pattern: lhs_lt_rhs
  // (04), (08), (11), (12), (16), (17)
  CHECK(tc[3].expected == report_string(tc[3].input));
  CHECK(tc[7].expected == report_string(tc[7].input));
  CHECK(tc[10].expected == report_string(tc[10].input));
  CHECK(tc[11].expected == report_string(tc[11].input));
  CHECK(tc[15].expected == report_string(tc[15].input));
  CHECK(tc[16].expected == report_string(tc[16].input));

  // report template pattern: lhs_lt_rhs_div0
  // (13)
  CHECK(tc[12].expected == report_string(tc[12].input));
}

