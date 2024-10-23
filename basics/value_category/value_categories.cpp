// g++ -c -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion value_categories.cpp

#include <type_traits>

template <typename>   struct is_prvalue_expr       : std::true_type  {};
template <typename T> struct is_prvalue_expr<T&>   : std::false_type {};
template <typename T> struct is_prvalue_expr<T&&>  : std::false_type {};

template <typename>   struct is_lvalue_expr        : std::false_type {};
template <typename T> struct is_lvalue_expr<T&>    : std::true_type  {};
template <typename T> struct is_lvalue_expr<T&&>   : std::false_type {};

template <typename>   struct is_xvalue_expr        : std::false_type {};
template <typename T> struct is_xvalue_expr<T&>    : std::false_type {};
template <typename T> struct is_xvalue_expr<T&&>   : std::true_type  {};

#include <utility>

constexpr int   get_value()  { static int i = 42; return i; }
constexpr int&  get_ref()    { static int i = 42; return i; }
constexpr int&& get_refref() { static int i = 42; return std::move(i); }

constexpr void f()
{
  int   a{42};
  int&  b{a};
  int&& r{std::move(a)};

  // expressions that have non-reference type
  // expressions that belong to prvalue value category
  static_assert(is_prvalue_expr<decltype((42))>::value);
  static_assert(is_prvalue_expr<decltype((get_value()))>::value);
  static_assert(std::is_integral_v<decltype(42)>);
  static_assert(std::is_integral_v<decltype(get_value())>);

  // expressions that have non-reference type
  // expressions that belong to lvalue value category
  static_assert(is_lvalue_expr<decltype((a))>::value);
  static_assert(std::is_integral_v<decltype(a)>);

  // expressions that have rvalue reference type
  // expressions that belong to lvalue value category
  static_assert(is_lvalue_expr<decltype((r))>::value);
  static_assert(std::is_rvalue_reference_v<decltype(r)>);

  // expressions that have rvalue reference type
  // expressions that belong to xvalue value category
  static_assert(is_xvalue_expr<decltype((std::move(a)))>::value);
  static_assert(is_xvalue_expr<decltype((get_refref()))>::value);
  static_assert(std::is_rvalue_reference_v<decltype(std::move(a))>);
  static_assert(std::is_rvalue_reference_v<decltype(get_refref())>);

  // expressions that have lvalue reference type
  // expressions that belong to lvalue value category
  static_assert(is_lvalue_expr<decltype((b))>::value);
  static_assert(is_lvalue_expr<decltype((get_ref()))>::value);
  static_assert(std::is_lvalue_reference_v<decltype(b)>);
  static_assert(std::is_lvalue_reference_v<decltype(get_ref())>);
}
