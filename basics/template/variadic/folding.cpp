#include <concepts>

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<Numeric T, Numeric... Ts>
constexpr auto sum(T init, Ts... rest)
{
  return (init + ... + rest);
}

void sum_test()
{
  static_assert(10 == sum(1, 2, 3, 4));
  static_assert(10.0 == sum(1, 2, 3, 4.0));
  static_assert(10.0 == sum(1.0, 2, 3, 4));
  static_assert(10.0 == sum(1, 2, 3.0, 4));
  static_assert(10.0 == sum(1.0, 2.0, 3.0, 4.0));
}

template<Numeric... Ts>
constexpr auto unaryLeftMinus(Ts... pack)
{
  return (... - pack);
}

template<Numeric... Ts>
constexpr auto unaryRightMinus(Ts... pack)
{
  return (pack - ...);
}

template<Numeric T, Numeric... Ts>
constexpr auto binaryLeftMinus(T init, Ts... pack)
{
  return (init - ... - pack);
}

template<Numeric T, Numeric... Ts>
constexpr auto binaryRightMinus(T init, Ts... pack)
{
  return (pack - ... - init);
}

void fold_test()
{
  // (((1 - 2) - 3) - 4) = -8
  static_assert(-8 == unaryLeftMinus(  1, 2, 3, 4));
  // (1 - (2 - (3 - 4))) = -2
  static_assert(-2 == unaryRightMinus( 1, 2, 3, 4));
  // (((1 - 2) - 3) - 4) = -8
  static_assert(-8 == binaryLeftMinus( 1, 2, 3, 4));
  // ((2 - (3 - 4)) - 1) = 2 
  static_assert( 2 == binaryRightMinus(1, 2, 3, 4));
}

int main() {}
