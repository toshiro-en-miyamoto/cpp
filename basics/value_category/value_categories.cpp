// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion value_categories.cpp

#include <utility>

template <typename T> struct is_prvalue       : std::true_type  {};
template <typename T> struct is_prvalue<T&>   : std::false_type {};
template <typename T> struct is_prvalue<T&&>  : std::false_type {};

template <typename T> struct is_lvalue        : std::false_type {};
template <typename T> struct is_lvalue<T&>    : std::true_type  {};
template <typename T> struct is_lvalue<T&&>   : std::false_type {};

template <typename T> struct is_xvalue        : std::false_type {};
template <typename T> struct is_xvalue<T&>    : std::false_type {};
template <typename T> struct is_xvalue<T&&>   : std::true_type  {};

#include <type_traits>

void f()
{
  int   a{42};
  int&  b{a};
  int&& r{std::move(a)};

  // expression '42' is prvalue
  // so is a literal (except for string literal)
  static_assert(is_prvalue<decltype((42))>::value);

  // expression 'a' is lvalue
  static_assert(is_lvalue<decltype((a))>::value);

  // expression 'b' is lvalue
  static_assert(is_lvalue<decltype((b))>::value);
  // type of variable 'b' is lvalue reference
  static_assert(std::is_lvalue_reference_v<decltype(b)>);

  // expression `r` is lvalue
  static_assert(is_lvalue<decltype((r))>::value);
  // type of variable 'r' is rvalue reference
  static_assert(std::is_rvalue_reference_v<decltype(r)>);

  // expression 'std::move(a)' is xvalue
  static_assert(is_xvalue<decltype((std::move(a)))>::value);
}


