// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex15.cpp
/*
  Given a series of positive integers, find the max, min and mode of that series.
  The number that appears the most times in a sequence is called the mode.
*/

#include <array>
// the value type must be 'long double' so as to calculate the mean of dataset:
// Range::size() is unsigned long int
constexpr std::array<long double, 20> samples {
  82, 67, 47, 78, 52, 84, 76, 73, 54, 91, 52, 67, 70, 68, 71, 84, 71, 61, 69, 43
};

#include <type_traits>
static_assert(std::is_same_v<long double, decltype(samples)::value_type>);

#include <algorithm>
#include <ranges>
#include <concepts>

template <std::ranges::input_range R, class T = std::ranges::range_value_t<R>>
  requires std::totally_ordered<T>
[[nodiscard]]
constexpr T max_v(R&& r) {
  T v = *(r.begin());
  for (auto e : r) {
    if (v < e) v = e;
  }
  return v;
}

static_assert(std::ranges::max(samples) == static_cast<decltype(samples)::value_type>(91));
static_assert(max_v(std::views::all(samples)) == std::ranges::max(samples));

template <std::ranges::input_range R, class T = std::ranges::range_value_t<R>>
  requires std::totally_ordered<T>
[[nodiscard]]
constexpr T min_v(R&& r) {
  T v = *(r.begin());
  for (auto e : r) {
    if (v > e) v = e;
  }
  return v;
}

static_assert(std::ranges::min(samples) == static_cast<decltype(samples)::value_type>(43));
static_assert(min_v(std::views::all(samples)) == std::ranges::min(samples));

template <std::ranges::input_range R, class T = std::ranges::range_value_t<R>>
  requires std::totally_ordered<T> and std::floating_point<T>
[[nodiscard]]
constexpr T range_v(R&& r) {
  auto [min, max] = std::ranges::minmax(r);
  return max - min;
}

static_assert(
  range_v(std::views::all(samples)) == static_cast<decltype(samples)::value_type>(91 - 43)
);

template <std::ranges::input_range R, class T = std::ranges::range_value_t<R>>
  requires std::totally_ordered<T> and std::floating_point<T>
[[nodiscard]]
inline constexpr T sum_v(R&& r) {
  return std::ranges::fold_left(r, static_cast<T>(0), std::plus<T>());
}


static_assert(
  sum_v(std::views::all(samples)) == static_cast<decltype(samples)::value_type>(1360)
);

#include <stdexcept>

template <std::ranges::input_range R, class T = std::ranges::range_value_t<R>>
  requires std::totally_ordered<T> and std::floating_point<T>
[[nodiscard]]
constexpr T mean_v(R&& r) {
  auto sum = std::ranges::fold_left(r, static_cast<T>(0), std::plus<T>());
  if (std::ranges::empty(r)) {
    // todo: the string literal to be retrieved from a injected resource
    throw std::logic_error{"dataset must not be empty"};
  } else {
    return sum / r.size();
  }
}

static_assert(
  mean_v(std::views::all(samples)) == static_cast<decltype(samples)::value_type>(68.0)
);

int main()
{
}
