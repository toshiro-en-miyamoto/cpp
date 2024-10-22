// g++ -c -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion value_categories.cpp

#include <type_traits>

template <typename>   struct is_prvalue       : std::true_type  {};
template <typename T> struct is_prvalue<T&>   : std::false_type {};
template <typename T> struct is_prvalue<T&&>  : std::false_type {};

template <typename>   struct is_lvalue        : std::false_type {};
template <typename T> struct is_lvalue<T&>    : std::true_type  {};
template <typename T> struct is_lvalue<T&&>   : std::false_type {};

template <typename>   struct is_xvalue        : std::false_type {};
template <typename T> struct is_xvalue<T&>    : std::false_type {};
template <typename T> struct is_xvalue<T&&>   : std::true_type  {};

#include <utility>

int         get_value () { int i = 42; return i; }
const int&  get_ref ()   { int i = 42; return i; }
int&&       get_refref() { int i = 42; return std::move(i); }

void f()
{
  int   a{42};
  int&  b{a};
  int&& r{std::move(a)};

  // expressions that belong to prvalue value category
  static_assert(is_prvalue<decltype((42))>::value);
  static_assert(is_prvalue<decltype((get_value()))>::value);

  // expressions that belong to lvalue value category
  static_assert(is_lvalue<decltype((a))>::value);
  static_assert(is_lvalue<decltype((b))>::value);
  static_assert(is_lvalue<decltype((r))>::value);
  static_assert(is_lvalue<decltype((get_ref()))>::value);

  // expressions that belong to xvalue value category
  static_assert(is_xvalue<decltype((std::move(a)))>::value);
  static_assert(is_xvalue<decltype((get_refref()))>::value);

  // expressions that have lvalue reference type
  static_assert(std::is_lvalue_reference_v<decltype(b)>);
  static_assert(std::is_lvalue_reference_v<decltype(get_ref())>);
  static_assert(std::is_same_v<int&, decltype(b)>);
  static_assert(std::is_same_v<const int&, decltype(get_ref())>);

  // expressions that have rvalue reference type
  static_assert(std::is_rvalue_reference_v<decltype(r)>);
  static_assert(std::is_rvalue_reference_v<decltype(std::move(a))>);
  static_assert(std::is_rvalue_reference_v<decltype(get_refref())>);
  static_assert(std::is_same_v<int&&, decltype(r)>);
  static_assert(std::is_same_v<int&&, decltype(std::move(a))>);
  static_assert(std::is_same_v<int&&, decltype(get_refref())>);
}
