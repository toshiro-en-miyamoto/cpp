// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex15.cpp
/*
  Given a series of positive integers, find the max, min and mode of that series.
  The number that appears the most times in a sequence is called the mode.
*/
#include <array>
constexpr std::array<double, 20> samples {
  81, 67, 47, 78, 52, 84, 76, 73, 54, 91, 52, 67, 70, 68, 71, 84, 71, 61, 69, 43
};

#include <algorithm>
#include <ranges>
#include <concepts>

template <std::ranges::input_range R>
requires std::totally_ordered<std::ranges::range_value_t<R>>
[[nodiscard]]
constexpr std::ranges::range_value_t<R> max_v(R&& r) {
  std::ranges::range_value_t<R> v = *(r.begin());
  for (auto e : r) {
    if (v < e) v = e;
  }
  return v;
}

static_assert(std::ranges::max(samples) == 91);
static_assert(max_v(std::views::all(samples)) == std::ranges::max(samples));

template <std::ranges::input_range R>
requires std::totally_ordered<std::ranges::range_value_t<R>>
[[nodiscard]]
constexpr std::ranges::range_value_t<R> min_v(R&& r) {
  std::ranges::range_value_t<R> v = *(r.begin());
  for (auto e : r) {
    if (v > e) v = e;
  }
  return v;
}

static_assert(std::ranges::min(samples) == 43);
static_assert(min_v(std::views::all(samples)) == std::ranges::min(samples));

template <std::ranges::input_range R>
[[nodiscard]]
inline constexpr auto range_v(R&& r) {
  return std::distance(r.begin(), r.end());
}

static_assert(samples.size() == 20);
static_assert(std::views::all(samples).size() == samples.size());
static_assert(range_v(std::views::all(samples)) == samples.size());

template <typename T>
concept dividable = requires (T a, std::size_t b) {
  a + b;  a - b;  a * b;  a / b;
};

enum class division_error {
  div0,  overflow
};

#include <expected>

template <std::ranges::input_range R>
requires std::totally_ordered<std::ranges::range_value_t<R>>
        && dividable<std::ranges::range_value_t<R>>
[[nodiscard]]
constexpr auto mean_v(R&& r)
  -> std::expected<std::ranges::range_value_t<R>, division_error> 
{
  auto sum
  = std::ranges::fold_left(r, 0, std::plus<std::ranges::range_value_t<R>>());
  if (r.size() == 0) {
    return std::unexpected(division_error::div0);
  } else {
    return sum / r.size();
  }
}

#include <numeric>  // sum = 1359; mean = 67.95
constexpr auto samples_sum = std::accumulate(samples.begin(), samples.end(), 0.0);
static_assert(samples_sum / samples.size() == 67.95);
static_assert(mean_v(std::views::all(samples)) == 67.95);

int main() {
}
