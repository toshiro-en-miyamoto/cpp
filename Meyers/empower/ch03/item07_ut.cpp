// g++ item07_ut.cpp -std=c++20
#include <concepts>
#include <initializer_list>
#include <numeric>
#include <functional>

template<typename T>
struct Topic6b
{
  const T sum;
  const T a, b, c;
  /*
   * If the previous stage does not produce a match, all
   * constructors of T participate in overload resolution
   * against the set of arguments that consists of the
   * elements of the braced-init-list, with the restriction
   * that only non-narrowing conversions are allowed.
   */
  constexpr Topic6b(T v0, T v1, T v2, T v3)
    : sum {v0}
    , a {v1}, b {v2}, c {v3}
  {}
};

void topic6b()
{
  constexpr Topic6b<int> b {1, 2, 3, 4};
  static_assert(1 == b.sum);
}

template<std::integral T>
struct Topic6a
{
  const T sum;
  const T a, b, c;
  /*
   * All constructors that take `std::initializer_list` as the
   * only argument, or as the first argument if the remaining
   * arguments have default values, are examined, and matched
   * by overload resolution against a single argument of
   * `type std::initializer_list`
   */
  constexpr Topic6a(std::initializer_list<T> list)
    : sum {
      std::accumulate(
        list.begin(), list.end(), 0, std::plus<T>()
      )}
    , a {0}, b {0}, c {0}
  {}

  constexpr Topic6a(T v0, T v1, T v2, T v3)
    : sum {v0}
    , a {v1}, b {v2}, c {v3}
  {}
};

void topic6a()
{
  constexpr Topic6a<int> a {1, 2, 3, 4};
  static_assert(10 == a.sum);
}

struct Topic4
{
  constexpr Topic4()
    : n {4}, c {'b'}
  {}

  int n;
  char c;
};

void topic4()
{
  /*
   * Otherwise, if the braced-init-list is empty and T is a class
   * type with a default constructor, value-initialization is
   * performed.
   */
  constexpr Topic4 a {};
  static_assert(a.n == 4);
  static_assert(a.c == 'b');
}

void topic2()
{
  /*
   * If T is a character array and the braced-init-list has a
   * single element that is an appropriately-typed string literal,
   * the array is initialized from the string literal as usual.
   */
  constexpr char a[] {"abc"};
  static_assert(a[2] == 'c');
}

struct Topic1
{
  int n;
  char c;
};

void topic1()
{
  // topic 3: aggregate initialization
  constexpr Topic1 ic1 {1, 'a'};

  /*
   * If T is an aggregate class and the braced-init-list has
   * a single element of the same or derived type (possibly
   * cv-qualified), the object is initialized from that element
   * (by copy-initialization for copy-list-initialization, or
   * by direct-initialization for direct-list-initialization).
   */
  constexpr Topic1 ic2 {ic1};
  static_assert(ic2.n == 1);

  constexpr Topic1 ic3 = {ic1};
  static_assert(ic3.n == 1);
}

int main()
{}
