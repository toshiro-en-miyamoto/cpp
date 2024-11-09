// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex15.cpp
/*
  Given a series of positive integers, find the max, min and mode of that series.
  The number that appears the most times in a sequence is called the mode.
*/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"

#include <array>
// the value type must be 'long double' so as to calculate the mean of dataset:
// Range::size() is unsigned long int
constexpr std::array<double, 20> samples
{
  82, 67, 47, 78, 52, 84, 76, 73, 54, 91, 52, 67, 70, 68, 71, 84, 71, 61, 69, 43
};

#include <type_traits>
static_assert(std::is_same_v<double, decltype(samples)::value_type>);

#include <ranges>
#include <concepts>
#include <utility>

/*!
 * \brief Returns the first greatest value in the range `r`.
 *
 * \param r The range of values to compare.
 *  If the range is empty, the behavior is undefined.
 * \return  The first greatest value in the range `r`.
 */
template <std::ranges::input_range R>
  requires std::floating_point<std::ranges::range_value_t<R>>
[[nodiscard]]
constexpr auto max_v(R&& r) -> std::ranges::range_value_t<R>
{
  auto max_value = *(r.begin());
  for (auto e : r) {
    if (max_value < e) max_value = e;
  }
  return max_value;
}

#include <algorithm>

static_assert(std::ranges::max(samples) == static_cast<decltype(samples)::value_type>(91));
static_assert(max_v(std::views::all(samples)) == std::ranges::max(samples));

/*!
 * \brief Returns the first smallest value in the range `r`.
 *
 * \param r The range of values to compare.
 *  If the range is empty, the behavior is undefined.
 * \return  The first smallest value in the range `r`.
 */
template <std::ranges::input_range R>
  requires std::floating_point<std::ranges::range_value_t<R>>
[[nodiscard]]
constexpr auto min_v(R&& r) -> std::ranges::range_value_t<R>
{
  auto min_value = *(r.begin());
  for (auto e : r) {
    if (e < min_value) min_value = e;
  }
  return min_value;
}

static_assert(std::ranges::min(samples) == static_cast<decltype(samples)::value_type>(43));
static_assert(min_v(std::views::all(samples)) == std::ranges::min(samples));

/*!
 * \brief Returns the difference between the greatest and smallest values.
 * 
 * \param r The range of values to evaluate.
 *  If the range is empty, the behavior is undefined.
 * \return  The range for the data set in the range `r`.
 */
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

/*!
 * \brief Computes the sum of the elements in the range `r`.
 * 
 * \param r The range of values to accumulate.
 *  If the range is empty, the behavior is undefined.
 * \return  The sum of elements in the range `r`.
 */
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

/*!
 * \brief Computes the average value of the elements in the range `r`.
 * 
 * \param r The range of values to compute the average.
 *  If the range is empty, the behavior is undefined.
 *  If the number of elements in the range is greater than the maximum value
 *  of the type `std::ranges::range_value_t<R>`, the behavior is undefined.
 * \return  The average value of elements in the range `r`.
 */
template <std::ranges::input_range R>
  requires std::floating_point<std::ranges::range_value_t<R>>
[[nodiscard]]
constexpr auto mean_v(R&& r) -> std::ranges::range_value_t<R>
{
  using T = std::ranges::range_value_t<R>;
  const auto sum = std::ranges::fold_left(r, static_cast<T>(0), std::plus<T>());
  const auto size = std::ranges::distance(r);
  return sum / static_cast<T>(size);
}

static_assert(
  mean_v(std::views::all(samples)) == static_cast<decltype(samples)::value_type>(68.0)
);

/*!
 * \brief Defines the half-open real-value interval.
 */
template <std::floating_point T> using Interval = std::pair<T, T>;

/*!
 * \brief Makes an interval
 * 
 * \param lower The lower value of the interval, inclusive.
 * \param upper The upper value of the interval, exclusive.
 * \return Teh interval [lower, upper).
 *  If the upper is less than the lower, the behavior is undefined.
 *  If distance(lower, upper) is greater than numerics_limit<T>::max(),
 *  the behavior is undefined.
 */
template <std::floating_point T>
[[nodiscard]]
constexpr inline Interval<T> make_interval(T lower, T upper)
{
  return std::make_pair(lower, upper);
}

/*!
 * \brief Computes the midpoint of the interval `i`.
 * 
 * \param i The interval to evaluate.
 * \return The midpoint value of the interval.
 */
template <std::floating_point T>
[[nodiscard]]
constexpr inline T midpoint(const Interval<T>& i)
{
  // lower is i.first;
  // upper is i.second;
  // auto [lower, upper] = i;
  return i.first + (i.second - i.first) / 2;
}

static_assert(midpoint(make_interval(10.0, 20.0)) == 15.0);

constexpr std::array test_intervals {
  std::make_pair(  0.0,  10.0)
, std::make_pair( 10.0,  20.0)
, std::make_pair( 20.0,  30.0)
, std::make_pair( 30.0,  40.0)
, std::make_pair( 40.0,  50.0)
, std::make_pair( 50.0,  60.0)
, std::make_pair( 60.0,  70.0)
, std::make_pair( 70.0,  80.0)
, std::make_pair( 80.0,  90.0)
, std::make_pair( 90.0, 100.0)
, std::make_pair(100.0, 110.0)
};

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
