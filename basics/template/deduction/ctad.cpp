// g++ -std=c++20 ctad.cpp
#include <concepts>

template<typename T, std::size_t SIZE>
struct array
{
  T data[SIZE];
};

template<typename T, std::same_as<T>... Us>
array(T first, Us... rest) -> array<T, 1 + sizeof...(Us)>;

int main()
{
  constexpr array a{1, 2, 3};
  static_assert(std::is_same_v<int[3], decltype(a.data)>);
}
