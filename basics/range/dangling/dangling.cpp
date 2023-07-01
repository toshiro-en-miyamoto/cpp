// g++ dangling.cpp -std=c++20
#include <array>
#include <algorithm>
#include <type_traits>
#include <ranges>

using the_array_type = const std::array<int, 4>;
constexpr the_array_type static_array{0, 1, 2, 3};

int main()
{
  auto array_by_value = [] { return std::array{0, 1, 2, 3}; };
  static_assert(array_by_value()[3] == 3);
  auto it_by_value = std::ranges::max_element(array_by_value());
  static_assert(std::is_same_v<
    decltype(it_by_value), std::ranges::dangling
  >);

  constexpr auto it_persist = std::ranges::max_element(static_array);
  static_assert(std::ranges::borrowed_range<decltype(static_array) &>);
  static_assert(std::is_same_v<
    decltype(it_persist),
    std::ranges::borrowed_iterator_t<the_array_type &> const
  >);
  static_assert(*it_persist == 3);
}
