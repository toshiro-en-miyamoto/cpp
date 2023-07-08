# `constexpr`

Reference:
- 15.13 Template Metaprogramming, *C++20 for Programmers*, Paul and Harvey Deitel

A goal of modern C++ is to do work at compile-time to enable the compiler to optimize your code for runtime performance. Much of this optimization work is done via *template metaprogramming* (TMP):

- computing values at compile-time with metafunctions,
- computing values at compile-time with `constexpr` functions,
- using type traits at compile-time via the `constexpr if` statement to optimize runtime performance and
- manipulating types at compile-time with metafunctions.

C++ templates are proved to be *Turing complete*, so anything that can be computed, can be computed at compile-time with C++ template metaprogramming.

## Metafunctions

You can perform compile-time calculations with *metafunctions*. [...] Metafunctions are implemented as class templates — typically using `struct`s.

There are two types of metafunctions:

- a *value metafunction*
- a *type metafunction*

A value metafunction is a class template with a public `static constexpr` data member typically named `value`. The class template uses its template arguments to compute value at compile-time.

```c++
// a value metafunction
template<int N>
struct factorial1
{
  static constexpr long long value
  { N * factorial1<N - 1>::value };
};

// factorial1 specialization for 0!
template<>
struct factorial1<0>
{
  static constexpr long long value{ 1 };
};

int main()
{
  static_assert(120 == factorial1<5>::value);
}
```

The preceding value metafunction `factorial1` is implemented using the recursive factorial definition:

```
n! = n * (n - 1)!
```

In meta-function recursion, you specify the *base case*. For the recursive factorial calculation, the base case is `0!` with a full template specialization `factorial1<0>`. Such a specialization uses the notation `template <>` to indicate that all the template’s arguments will be specified explicitly.

None of what the compiler does during template specialization modifies variable values after they’re initialized. **There are no mutable variables in template metaprograms**. [...] All the values processed in this example are compile-time constants.

## `constexpr` functions

A function declared `constexpr` can be called at compile-time or runtime.

> In C++20, you can declare a function `consteval` to indicate that it can be called only at compile-time to produce a constant.

Many computations performed with metafunctions are easier to implement using traditional functions that are declared `constexpr`. [...] In fact, various members of the C++ standard committee prefer `constexpr`-based metaprogramming over template-based metaprogramming.

```c++
// a recursive constexpr function
constexpr long long factorial2r(int n)
{
  if (n <= 1) {
    return 1;
  } else {
    return n * factorial2r(n -1);
  }
}

// an iterative constexpr function
constexpr long long factorial2i(int n)
{
  long long factorial{ 1 };

  for (long long i{ 2 }; i <= n; ++i) {
    factorial *= i;
  }

  return factorial;
}

int main()
{
  static_assert(120 == factorial2r(5));
  static_assert(120 == factorial2i(5));
}
```

## `constexpr if` Statements

`distance1()` shows *concept-based function overloading* to optimize runtime execution. In this example, the compiler chose the correct overloaded function template to call based on the template parameter’s type constraints.

```c++
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
```

`distance2()` shows how you could implement similar functionality prior to concepts.

```c++
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
```

The template argument `It` is constrained to be at least `std::input_iterator`, which allows the O(n) approach in the `else` block to work. In fact, it works for all iterator types, because [iterators](https://en.cppreference.com/w/cpp/iterator/iterator_tags) are defined in a hierarchical manner:

```c++
struct input_iterator_tag { };
struct output_iterator_tag { };

struct forward_iterator_tag
  : public input_iterator_tag { };

struct bidirectional_iterator_tag
  : public forward_iterator_tag { };

struct random_access_iterator_tag
  : public bidirectional_iterator_tag { };

struct contiguous_iterator_tag
  : public random_access_iterator_tag { };
```

`distance2()` uses a C++17 compile-time `constexpr if` statement to determine the actual type of the passed argument. If the type is `random_access`, then the compiler instantiates the `O(1)` code (and discard the `O(n)` code). Otherwise, the `O(n)` code is instantiated (the `O(1)` discarded).

```c++
  if constexpr (std::is_same_v<
    std::random_access_iterator_tag,
    typename std::iterator_traits<It>::iterator_category
  >) {
    // O(1) code
  } else {
    // O(n) code
  }
```

`std::iterator_traits<It>::iterator_category` returns the iterator tag of `It` type.

```c++
#include <array>
#include <list>
#include <cassert>

int main()
{
  const std::array a{1, 2, 3, 4, 5};
  const std::list  l{1, 2, 3};

  using IT_TAG_A = std::iterator_traits
    <decltype(a.begin())>::iterator_category;

  assert((std::is_same_v
    <std::random_access_iterator_tag, IT_TAG_A>));
  assert((std::is_base_of_v
    <std::random_access_iterator_tag, IT_TAG_A>));

  using IT_TAG_L = std::iterator_traits
    <decltype(l.begin())>::iterator_category;
  
  assert((std::is_same_v
    <std::bidirectional_iterator_tag, IT_TAG_L>));
  assert((std::is_base_of_v
    <std::bidirectional_iterator_tag, IT_TAG_L>));

  assert(5 == distance1(a.begin(), a.end()));
  assert(3 == distance1(l.begin(), l.end()));

  assert(5 == distance2(a.begin(), a.end()));
  assert(3 == distance2(l.begin(), l.end()));
}
```

As you can see, both `is_same_v` and `is_base_of_v` work to check iterator tags.
