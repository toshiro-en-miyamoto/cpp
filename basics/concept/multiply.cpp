#include <concepts>
#include <string>

template<typename T>
  requires std::integral<T> || std::floating_point<T>
T multiply1(T first, T second)
{
  return first * second;
}

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<Numeric T>
T multiply2(T first, T second)
{
  return first * second;
}

auto multiply3(Numeric auto first, Numeric auto second)
{
  return first * second;
}

template<Numeric T1, Numeric T2>
auto multiply4(T1 first, T2 second)
{
  return first * second;
}

int main()
{
  multiply1(5, 7);
  multiply1(2.3, 2.0);
  // multiply1(2.3, 2);
  // multiply1(std::string{"a"}, std::string("b"));

  multiply2(5, 7);
  multiply2(2.3, 2.0);
  // multiply2(2.3, 2);

  multiply3(5, 7);
  multiply3(2.3, 2.0);
  multiply3(2.3, 2);

  multiply4(5, 7);
  multiply4(2.3, 2.0);
  multiply4(2.3, 2);
}
