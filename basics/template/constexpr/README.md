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
  static constexpr long long value {
    N * factorial1<N - 1>::value
  };
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

## `constexpr if`


