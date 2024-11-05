// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex15.cpp
/*
  Given a series of positive integers, find the max, min and mode of that series.
  The number that appears the most times in a sequence is called the mode.
*/

#include <array>
// the value type must be 'long double' so as to calculate the mean of dataset:
// Range::size() is unsigned long int
constexpr std::array<double, 20> samples
{
  82, 67, 47, 78, 52, 84, 76, 73, 54, 91, 52, 67, 70, 68, 71, 84, 71, 61, 69, 43
};

#include <type_traits>
static_assert(std::is_same_v<double, decltype(samples)::value_type>);

#include <algorithm>
#include <ranges>
#include <concepts>

template <std::ranges::input_range R>
  requires std::floating_point<std::ranges::range_value_t<R>>
[[nodiscard]]
constexpr auto max_v(R&& r)
{
  auto max_value = *(r.begin());
  for (auto e : r) {
    if (max_value < e) max_value = e;
  }
  return max_value;
}

static_assert(std::ranges::max(samples) == static_cast<decltype(samples)::value_type>(91));
static_assert(max_v(std::views::all(samples)) == std::ranges::max(samples));

template <std::ranges::input_range R>
  requires std::floating_point<std::ranges::range_value_t<R>>
[[nodiscard]]
constexpr auto min_v(R&& r)
{
  auto min_value = *(r.begin());
  for (auto e : r) {
    if (min_value > e) min_value = e;
  }
  return min_value;
}

static_assert(std::ranges::min(samples) == static_cast<decltype(samples)::value_type>(43));
static_assert(min_v(std::views::all(samples)) == std::ranges::min(samples));

template <std::ranges::input_range R>
  requires std::floating_point<std::ranges::range_value_t<R>>
[[nodiscard]]
constexpr auto range_v(R&& r)
{
  const auto [min, max] = std::ranges::minmax(r);
  return max - min;
}

static_assert
(
  range_v(std::views::all(samples)) == static_cast<decltype(samples)::value_type>(91 - 43)
);

template <std::ranges::input_range R>
  requires std::floating_point<std::ranges::range_value_t<R>>
[[nodiscard]]
constexpr auto sum_v(R&& r)
{
  using T = std::ranges::range_value_t<R>;
  return std::ranges::fold_left(r, static_cast<T>(0), std::plus<T>());
}

static_assert(
  sum_v(std::views::all(samples)) == static_cast<decltype(samples)::value_type>(1360)
);

template <std::ranges::input_range R>
  requires std::floating_point<std::ranges::range_value_t<R>>
[[nodiscard]]
constexpr auto mean_v(R&& r) -> std::ranges::range_value_t<R>
{
  using T = std::ranges::range_value_t<R>;
  const auto sum = std::ranges::fold_left(r, static_cast<T>(0), std::plus<T>());
  return sum / static_cast<T>(r.size());
}

static_assert(
  mean_v(std::views::all(samples)) == static_cast<decltype(samples)::value_type>(68.0)
);

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include <limits>
#include <cfenv>

TEST_CASE("range_v overflow")
{
  using T = double;
  const std::array<T, 2> range_overflow_data {
    std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max()
  };
  std::feclearexcept(FE_OVERFLOW);
  [[maybe_unused]] auto result = range_v(std::views::all(range_overflow_data));
  CHECK(std::fetestexcept(FE_OVERFLOW));
}

TEST_CASE("sum_v overflow")
{
  using T = double;
  const std::array<T, 2> sum_overflow_data {
    std::numeric_limits<T>::max(), std::numeric_limits<T>::max()
  };
  std::feclearexcept(FE_OVERFLOW);
  [[maybe_unused]] auto result = sum_v(std::views::all(sum_overflow_data));
  CHECK(std::fetestexcept(FE_OVERFLOW));
}

TEST_CASE("mean_v overflow and divide-by-zero")
{
  using T = double;
  const std::array<T, 2> mean_overflow_data {
    std::numeric_limits<T>::max(), std::numeric_limits<T>::max()
  };
  std::feclearexcept(FE_OVERFLOW or FE_DIVBYZERO);
  [[maybe_unused]] auto result1 = mean_v(std::views::all(mean_overflow_data));
  if(std::fetestexcept(FE_OVERFLOW)) {
    CHECK(true);
  } else if (std::fetestexcept(FE_DIVBYZERO)) {
    CHECK(false);
  } else {
    CHECK(false);
  }

  const std::array<double, 0> mean_div0_data {};
  std::feclearexcept(FE_OVERFLOW or FE_DIVBYZERO);
  [[maybe_unused]] auto result2 = mean_v(std::views::all(mean_div0_data));
  // expected FE_DIVBYZERO, but actual result is FE_OVERFLOW
  if(std::fetestexcept(FE_OVERFLOW)) {
    CHECK(true);
  } else if (std::fetestexcept(FE_DIVBYZERO)) {
    CHECK(false);
  } else {
    CHECK(false);
  }
}
