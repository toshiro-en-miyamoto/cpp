// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex06.cpp
/*
  a program that performs as a very simple calculator.
  Your calculator should be able to handle the four basic math operations
  – add, subtract, multiply, and divide – on two input values.
  Your program should prompt the user to enter three arguments:
  two double values and a character to represent an operation.
  If the entry arguments are 35.6, 24.1, and '+',
  the program output should be The sum of 35.6 and 24.1 is 59.7.

  Transformations
  - test code
    - from string_view to istream
  - production code
    (P.1) from istream to the operands and operation
    (P.2) from the operands and operation to the evaluated value
*/

#include <utility>
#include <limits>
#include <tuple>

enum class Operator {
  undef, add = '+', sub = '-', mul = '*', div = '/'
};
using Expression = std::tuple<const double, const double, const Operator>;

enum class parse_error {
  invalid_operator
};

constexpr double infinity  = std::numeric_limits<double>::infinity();
constexpr double not_a_num = std::numeric_limits<double>::signaling_NaN();

// (P.2) from the operands and operation to the evaluated value
// e.g., from {35.6, 4.1, Operator::plus} to 39.7
#include <expected>
auto to_value(const Expression& expr) -> std::expected<double, parse_error> {
  const auto& [lhs, rhs, op] = expr;
  switch (op) {
    case Operator::add: return lhs + rhs;
    case Operator::sub: return lhs - rhs;
    case Operator::mul: return lhs * rhs;
    case Operator::div: return rhs != 0 ? lhs / rhs : infinity;
    default: return std::unexpected(parse_error::invalid_operator);
  }
};

#include <istream>

// (P.1) from istream to the operands and operation
// e.g., from "35.6 4.1 +" to {35.6, 4.1, Operator::plus}
auto to_expr(std::istream& is) -> Expression {
  double lhs = 0.0;
  double rhs = 0.0;
  char ch_op = 0;
  is >> lhs >> rhs >> ch_op;
  switch (ch_op) {
    case static_cast<char>(Operator::add):
      return {lhs, rhs, Operator::add};
    case static_cast<char>(Operator::sub):
      return {lhs, rhs, Operator::sub};
    case static_cast<char>(Operator::mul):
      return {lhs, rhs, Operator::mul};
    case static_cast<char>(Operator::div):
      return {lhs, rhs, Operator::div};
    default:
      return {lhs, rhs, Operator::undef};
  }
};


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"

using Test_expr = std::pair<const Expression, const double>;

constexpr Test_expr test_expressions[] {
  {{35.6, 4.1, Operator::add},   35.6 + 4.1}
, {{35.6, 4.1, Operator::sub},   35.6 - 4.1}
, {{35.6, 4.1, Operator::mul},   35.6 * 4.1}
, {{35.6, 4.1, Operator::div},   35.6 / 4.1}
, {{35.6, 0.0, Operator::div},   infinity}
, {{35.6, 4.1, Operator::undef}, not_a_num}
};

#include <vector>

TEST_CASE("to value") {
  for (const auto& tc : test_expressions) {
    if (const auto& result = to_value(tc.first); result.has_value()) {
      CHECK(result == tc.second);
    } else {
      CHECK(result.error() == parse_error::invalid_operator);
    }
  }
}

#include <string_view>

using namespace std::literals;
using Test_case = std::pair<std::string_view, const double>;

constexpr Test_case test_cases[] {
    std::make_pair("35.6 4.1 +"sv, 35.6 + 4.1)
  , std::make_pair("35.6 4.1 -"sv, 35.6 - 4.1)
  , std::make_pair("35.6 4.1 *"sv, 35.6 * 4.1)
  , std::make_pair("35.6 4.1 /"sv, 35.6 / 4.1)
  , std::make_pair("35.6 0.0 /"sv, std::numeric_limits<double>::infinity())
  , std::make_pair("35.6 4.1 @"sv, std::numeric_limits<double>::signaling_NaN())
};

#include <sstream>

TEST_CASE("to_expr_value") {
  for (const auto& tc : test_cases) {
    std::istringstream iss{ std::string{tc.first} };
    const auto& result = to_value(to_expr(iss));
    if (result.has_value()) {
      CHECK(result == tc.second);
    } else {
      CHECK(result.error() == parse_error::invalid_operator);
    }
  }
}
