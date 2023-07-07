// g++ -std=c++20 distance.cpp
#include <iterator>

// using concept-based function overloading
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

// using constexpr if statement
template<std::input_iterator It>
auto distance2(It begin, It end)
{
  if constexpr (std::is_same_v<
    std::random_access_iterator_tag,
    typename std::iterator_traits<It>::iterator_category
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
  const std::array a{1, 2, 3, 4, 5};
  const std::list  l{1, 2, 3};

  using ITER_CONCEPT_A
  = std::iterator_traits<decltype(a.begin())>::iterator_category;
  assert((std::is_same_v<std::random_access_iterator_tag, ITER_CONCEPT_A>));

  using ITER_CONCEPT_L = std::iterator_traits<decltype(l.begin())>::iterator_category;
  assert((std::is_same_v<std::bidirectional_iterator_tag, ITER_CONCEPT_L>));

  assert(5 == distance1(a.begin(), a.end()));
  assert(3 == distance1(l.begin(), l.end()));

  assert(5 == distance2(a.begin(), a.end()));
  assert(3 == distance2(l.begin(), l.end()));
}
