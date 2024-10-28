// g++ -c -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion lifetime2.cpp
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

#include <string>
std::string s1 = "Test";
const std::string& r2 = s1 + s1;
static_assert(is_prvalue_expr<decltype((s1 + s1))>::value);

#include <initializer_list>
std::initializer_list<int> il{ 1, 2, 3 };

struct S { int m; } s;
static_assert(is_lvalue_expr<decltype((s.m))>::value);
static_assert(is_prvalue_expr<decltype((S()))>::value);
static_assert(is_xvalue_expr<decltype((S().m))>::value);  // bug of intellisense

int* get_array() {
  static int a[] {1, 2, 3};
  return a;
}
static_assert(is_prvalue_expr<decltype((get_array()))>::value);
static_assert(is_lvalue_expr<decltype((get_array()[1]))>::value);

#include <set>
std::string f(std::set<std::string> s, std::string e) {
  if (auto [iter, success] = s.insert(e); success) {
    return "'" + *iter + "' inserted.";
  } else {
    return "'" + *iter + "' already exists.";
  } // lifetimes of 'iter' and 'success` are extended
}


// https://en.cppreference.com/w/cpp/language/range-for
// Temporary range initializer
// If range-initializer returns a temporary, its lifetime is
// extended until the end of the loop.
#include <list>
using T = std::list<int>;
const T& f1(const T& t) { return t; }
T g();

void foo() {
  for (auto& e : f1(g())) {}    // possibly dangling reference to a temporary
  // the temporary was destroyed at the end of the full expression ‘f1(g()())’

  for (T l = f1(g()); auto& e : l) {}
}

// S is an aggregate without user-provided destructor
struct S { int x, y; };
// static_assert(std::is_implicit_lifetime_v<S>);

struct A { int m; };
static_assert(std::is_trivial_v<A> == true);
 
struct B { B() {} };
static_assert(std::is_trivial_v<B> == false);
