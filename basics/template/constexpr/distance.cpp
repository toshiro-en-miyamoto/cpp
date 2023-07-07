// g++ -std=c++20 distance.cpp
#include <iterator>

template<std::input_iterator It>
constexpr auto distance1(It begin, It end)
{
  std::ptrdiff_t count{ 0 };
  for (auto& it{ begin }; it != end; ++it) {
    ++count;
  }
  return count;
}

template<std::random_access_iterator It>
constexpr auto distance1(It begin, It end)
{
  return end - begin;
}

template<std::input_iterator It>
auto distance2(It begin, It end)
{
  if constexpr (std::is_same_v<
    std::random_access_iterator_tag,
    It::iterator_category
  >) {
    return end - begin;
  } else {
    std::ptrdiff_t count{ 0 };
    for (auto& it{ begin }; it != end; ++it) {
      ++count;
    }
    return count;
  }
}

#include <array>
#include <list>
#include <cassert>

int main()
{
  std::array a{1, 2, 3, 4, 5};
  std::list  l{1, 2, 3};

  // assert(5 == distance1(a.begin(), a.end()));
  assert(3 == distance1(l.begin(), l.end()));

  assert(5 == distance2(a.begin(), a.end()));
  assert(3 == distance2(l.begin(), l.end()));
}
