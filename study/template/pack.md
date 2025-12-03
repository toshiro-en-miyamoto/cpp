# Parameter packs

This article primarily references to

- Chapter 3. Variadic Templates, Template Metaprogramming with C++, Marius Bancila, 2022.
- [Pack, C++ reference](https://www.cppreference.com/w/cpp/language/parameter_pack.html)

*Variadic arguments* allow a function to accept any number of extra arguments. A function is a variadic if the last parameter of its parameter list is an ellipsis (`...`), such as

```c++
int printf(const char* format, ...);
```

Alternatives:

- *Variadic templates* can also be used to create functions that take variable number of arguments. They are often the better choice because they do not impose restrictions on the types of the arguments, do not perform integral and floating-point promotions, and are type safe.
- If all variable arguments share a common type, a `std::initializer_list` provides a convenient mechanism (albeit with a different syntax) for accessing variable arguments. In this case however the arguments cannot be modified since `std::initializer_list` can only provide a const pointer to its elements.

([Variadic arguments, C++ reference](https://en.cppreference.com/w/cpp/language/variadic_arguments.html))

## An introduction to the parameter pack

The following code exemplifies how parameter packs are used:

```c++
// a variadic function template
template <typename T, typename... A>    // [1]
constexpr T sum(T a, A... args)         // [2]
{
  return a + sum(args...);              // [3]
}

// an explicit specialization
template <typename T>
constexpr T sum(T a)
{
  return a;
}

static_assert(sum(1, 5, 3, -4, 9) == 14);
```

The ellipsis (`...`) is used in three different places, with different meanings, in the implementation of a variadic function template:

1.  specifying a *template parameter pack*: `typename... A`
2.  specifying a *function parameter pack*: `A... args`
3.  expanding the function parameter pack: `args...`

The result of such a parameter expansion is a comma-separated list of zero or more expressions.

What we have here are two overloads for the `sum` function. Although variadic function template implementations look like using some sort of compile-time recursion mechanism (in this case the overload with one parameter acting as the end case), in fact, they're only relying on overloaded functions, instantiated from the template and the set of provided arguments.

From the call `sum(1, 5, 3, -4, 9)`, the compiler is instantiating a set of overloaded functions with five, four, three, two arguments, and one argument. Conceptually, it is the same as having the following set of overloaded functions:

```c++
// generated from the primary template
int sum(int a, b, c, d, e) { return a + sum(b, c, d, e); }
int sum(int    b, c, d, e) { return b + sum(   c, d, e); }
int sum(int       c, d, e) { return c + sum(      d, e); }
int sum(int          d, e) { return d + sum(         e); }
// generated from the explicit specialization
int sum(int             e) { return e; }
```

## `sizeof...` operator

In the next example, the `sizeof…` operator is used (the line `[1]`) to implement the end of the recursion pattern of the variadic function template `sum` with the help of a `constexpr if` statement. If the number of the arguments in the parameter pack is zero (meaning there is a single argument to the function) then we are processing the last argument, so we just return the value. Otherwise, we add the first argument to the sum of the remaining ones.

```c++
template <typename T, typename... A>
constexpr T sum(T a, A... args)
{
  if constexpr (sizeof...(args) == 0)   // [1]
    return a;
  else
    return a + sum(args...);
}

static_assert(sum(1, 5, 3, -4, 9) == 14);
```

This is semantically equivalent to, but on the other hand more concise than, the previous classical approach.

Note that the difference between `sizeof...(args)` and `sizeof(arg)...`:

```c++
template <typename... Ts>
constexpr auto get_type_sizes()
{
  return
  std::array<std::size_t, sizeof...(Ts)> {sizeof(Ts)...};
}

static_assert(
  get_type_sizes<short, int, long>()
  == std::array<std::size_t, 3> {2, 4, 8}
);
```

In this snippet,

- `sizeof...(Ts)` evaluates to 3 at compile-time
- `sizeof(Ts)...` is expanded to `sizeof(T1), sizeof(T2), sizeof(T3)`, a comma-separated list of three `sizeof()` expressions

## Pack

This section references [Pack, C++ reference](https://en.cppreference.com/w/cpp/language/parameter_pack.html).

A pattern followed by an ellipsis is expanded into zero or more instantiations of the pattern, where the name of the pack is replaced by each of the elements from the pack, in order. Instantiations are comma-separated.

```c++
constexpr int g(int*, double*, const char**)
{ return 1; }

template <typename... T>
constexpr int f(T... args)
{ return g(&args...); }
  // "&args..." is a pack expansion
  // "&args" is its pattern

static_assert(f(1, 0.2, "a") == 1);
```

Template parameter packs appear in template lists of every kind of templates: function template; class template; alias template; variable template; and concept.

| Syntax: template parameter pack   | description                                 |
|-----------------------------------|---------------------------------------------|
| *type*... *pack-name*             | a constant template parameter pack          |
| `typename`... *pack-name*         | a type template parameter pack              |
| *type-constraint*... *pack-name*  | a constrained type template parameter pack  |
| `template <` p `>typename`... *pack-name* | a template template parameter pack  |

```c++
template <typename... T>
struct Tuple {};

Tuple<> t0;               // OK
Tuple<int, float> t2;     // T ⟹ int, float
```

Function parameter packs with a form of declarator appear in a function parameter list of a variadic function template.

| Syntax: function parameter pack   | description                                       |
|-----------------------------------|---------------------------------------------------|
| *pack-name*...                    | pack param name is optional, see `index_sequence` |
| *pack-name*... *pack-param-name*  | such as `f(T... args)`                            |

```c++
template <typename... T>
void f(T... args);

f();                    // OK
f(2, 1.0);              // T... args ⟹ int 2, double 1.0
```

The class template `std::integer_sequence` represents a compile-time sequence of integers. When used as an argument to a function template, the parameter pack `I` can be deduced and used in pack expansion. A helper alias template `std::index_sequence` is defined for the common case of `std::integer_sequence` where the sequence is of `std::size_t`.

```c++
template <typename T, std::size_t... I>
constexpr auto array_to_tuple(std::array<T, sizeof...(I)> a, std::index_sequence<I...>)
{
  return std::make_tuple(a[I]...);
}

constexpr std::array a{1,2,3};
constexpr auto tuple2 = array_to_tuple(a, std::make_index_sequence<a.size()>{});
```

Pack expansions appear in a body of a template.

| Syntax: pack expansion    | description                                 |
|---------------------------|---------------------------------------------|
| *pattern*...              | the pattern must include at least one pack  |

```c++
template <typename... T>
constexpr int f(T... args)
{
  return g(&args...);
  // "&args..." is a pack expansion
  // "&args" is its pattern
}
```

## Expansion loci

This section references [Pack, C++ reference](https://en.cppreference.com/w/cpp/language/parameter_pack.html).

Depending on where the expansion takes place, the resulting comma-separated list is a different kind of list. The following is the list of all allowed contexts:

- Function parameter list
  - In a function parameter list, if an ellipsis appears in a parameter declaration, the parameter declaration is the pattern:
    ```c++
    template <typename... T>
    void f(T... args);
    ```
- Function argument list
  - the largest expression or brace-enclosed initializer list to the left of the ellipsis is the pattern that is expanded:
    | pack                | expanded to                 |
    |---------------------|-----------------------------|
    | `f(args...);`       | `f(E1, E2, E3);`            |
    | `f(&args...);`      | `f(&E1, &E2, &E3);`         |
    | `f(n, ++args...);`  | `f(n, ++E1, ++E2, ++E3);`   |
  - inside the parentheses of a direct initializer, the rules are identical to the rules for a function call expression:
    | pack                  | expanded to                     |
    |-----------------------|---------------------------------|
    | `Class c1(&args...);` | `Class::Class(&E1, &E2, &E3);`  |
  - in a brace-enclosed initializer list, a pack expansion may appear as well: since initializer lists guarantee sequencing, this can be used to call a function on each element of a pack, in order:
    | pack |
    |------|
    | `int dummy[sizeof...(Ts)] {(std::cout << args, 0)...};` |
- Template parameter list
  - Pack expansion in a template parameter list expands to a constant template parameter list:
    ```c++
    template<T... Values>
    struct apply {};
    // such as <int, char, int(&)[5]>
    ```
- Template argument list
  - Pack expansions can be used anywhere in a template argument list, provided the template has the parameters to match the expansion:
    | pack                        | expanded to                 |
    |-----------------------------|-----------------------------|
    | `container<A, B, C...> t1;` | `container<A, B, E1, E2>`   |
    | `container<C..., A, B> t1;` | `container<E1, E2, A, B>`   |
- Lambda captures
  - Pack expansion may appear in the capture clause of a lambda expression:
    ```c++
    template<class... Args>
    void f(Args... args)
    {
      auto lm = [&, args...] { return g(args...); };
      lm();
    }
    ```
- The `sizeof...` operator
  - it is classified as a pack expansion as well:
- Alignment specifier
  - `alignas(pack...)` is equivalent to multiple alignas specifiers applied to the same declaration, one for each member of the parameter pack, which can be either type or constant parameter pack. ([`alignas` specifier](https://www.cppreference.com/w/cpp/language/alignas.html))
- Fold expressions
  - This topic will be discussed later in this article.
- `using`-declarations
  - In using declarations, ellipsis may appear in the list of declarators, this is useful when deriving from a template parameter pack:
    ```c++
    template<typename... bases>
    struct X : bases...
    {
      using bases::g...;
    };
    X<B, D> x; // OK: B::g and D::g introduced
    ```

## Class templates

When implementing variadic function templates we used a recursion pattern with two overloads, one for the general case and one for ending the recursion. The same approach has to be taken with variadic class templates, except that we need to use specialization for this purpose.

```c++
namespace my {

// the primary template
template <typename T, typename... Ts>
struct tuple
{
  constexpr tuple(const T& t, const Ts&... ts)
  : value{t}, rest{ts...} {}

  constexpr std::size_t size() const
  { return 1 + rest.size(); }

  T value;
  tuple<Ts...> rest;
};

// the partial specialization
template <typename T>
struct tuple<T>
{
  constexpr tuple(const T& t)
  : value{t} {}

  constexpr std::size_t size() const
  { return 1; }

  T value;
};

}; // namespace my

constexpr my::tuple<int> one(42);
constexpr my::tuple<int, double> two(42, 53.0);
constexpr my::tuple<int, double, char> three(42, 53.0, 'a');

static_assert(  42 == one.value);
static_assert(  42 == two.value);
static_assert(53.0 == two.rest.value);
static_assert(  42 == three.value);
static_assert(53.0 == three.rest.value);
static_assert( 'a' == three.rest.rest.value);
```

The first class is the primary template. [...] a `tuple` of `N` elements will contain the first element and another `tuple`; this second `tuple`, in turn, contains the second element and yet another `tuple`; this third nested `tuple` contains the `rest`. And this pattern continues until we end up with a `tuple` with a single element. This is defined by the partial specialization `tuple<T>`. 

- `tuple<int> one(42);`
  instantiates one `tuple` definition
  - from the partial specialization
    ```c++
    struct tuple<int>
    {
      int value;
    };
    ```
- `tuple<int, double> two(42, 42.0);`
  instantiates two `tuple` definitions
  - from the primary template
    ```c++
    struct tuple<int, double>
    {
      int value;
      tuple<double> rest;
    };
    ```
  - from the partial specialization
    ```c++
    struct tuple<double>
    {
      double value;
    };
    ```
- `tuple<int, double, char> three(42, 42.0, 'a');`
  instantiates three `tuple` definitions
  - from the primary template
    ```c++
    struct tuple<int, double, char>
    {
      int value;
      tuple<double, char> rest;
    };

    struct tuple<double, char>
    {
      double value;
      tuple<char> rest;
    };
    ```
  - from the partial specialization
    ```c++
    struct tuple<char>
    {
      char value;
    };
    ```

Although this works, accessing elements through the `rest` member, such as in `three.rest.rest.value`, is very cumbersome. And the more elements a tuple has the more difficult it is to write code in this way. Therefore, we'd like to use some helper function to simplify accessing the elements of a tuple. The following is a snippet of how the previous could be transformed:

```c++
static_assert(  42 == my::get<0>(one));
static_assert(  42 == my::get<0>(two));
static_assert(53.0 == my::get<1>(two));
static_assert(  42 == my::get<0>(three));
static_assert(53.0 == my::get<1>(three));
static_assert( 'a' == my::get<2>(three));
```

`get<N>` is a variadic function template that takes a tuple as an argument and returns a reference to the element at the N index in the tuple.

```c++
template <std::size_t N, typename... Ts>
constexpr const typename nth_type<N, Ts...>::value_type&
get(const tuple<Ts...>& t) noexcept;
```

Its implementation, however, requires some helper types.

- class template `nth_type` for retrieving the type of the N-th element in the tuple
  ```c++
  namespace my {
  template <std::size_t N, typename T, typename... Ts>
  struct nth_type : nth_type<N-1, Ts...>
  {
    static_assert(N < sizeof...(Ts) + 1);
  };

  template <typename T, typename... Ts>
  struct nth_type<0, T, Ts...>
  {
    using value_type = T;
  };
  }; // namespace my
  ```
- class template `getter` for retrieving the value of the N-th element in the tuple
  ```c++
  namespace my {
  template <std::size_t N>
  struct getter
  {
    template <typename... Ts>
    constexpr static const typename nth_type<N, Ts...>::value_type&
    get(const tuple<Ts...>& t) noexcept
    {
      return getter<N-1>::get(t.rest);
    }
  };

  template <>
  struct getter<0>
  {
    template <typename T, typename... Ts>
    constexpr static const T&
    get(const tuple<T, Ts...>& t) noexcept
    {
      return t.value;
    }
  };
  }; // namespace my
  ```

`nth_type` class template as the primary template uses recursive inheritance. And the specialization for the index 0 defines an alias called `value_type` for the first type template.

`getter` class template has a single template parameter, which represents the index of the tuple element we want to access.

With all these defined, we can now provide an actual implementation for the helper variadic function template `get`.

```c++
namespace my {
template <std::size_t N, typename... Ts>
constexpr const typename nth_type<N, Ts...>::value_type&
get(const tuple<Ts...>& t) noexcept
{
  return getter<N>::get(t);
}
}; // namespace my
```

Consider the following snippet:

```c++
constexpr my::tuple<int, double, char> three(42, 53.0, 'a');
my::get<2>(three);
```

Then with calling `get<2>(const tuple<int, double, char>&)`, the compiler instantiates

- a definition from the function template `get`:
  ```c++
  template <>
  constexpr const typename nth_type<2, int, double, char>::value_type&
  my::get(const tuple<int, double, char>& t) noexcept
  {
    return getter<2>::get(t);
  }
  ```

With the definition of `get`, the compiler instantiates:

- two definitions from the primary template for `nth_type`:
  ```c++
  template <>
  struct nth_type<2, int, double, char> : nth_type<1, double, char>
  {};
  template <>
  struct nth_type<1, double, char> : nth_type<0, char>
  {};
  ```
- one definition from the specialization for `nth_type`:
  ```c++
  template <>
  struct nth_type<0, char>
  {
    using value_type = char;
  };
  ```
  - which translates the return type of the previous definition of `get` function as `char`:
  ```c++
  template <>
  constexpr const char&
  my::get(const tuple<int, double, char>& t) noexcept
  {
    return getter<2>::get(t);
  }
  ```

With calling `getter<2>::get(const tuple<int, double, char>&)`, the compiler instantiates:

- two definitions from the primary template for `getter`:
  ```c++
  template <>
  struct my::getter<2>
  {
    template <>
    constexpr static const typename
    nth_type<2, int, double, char>::value_type&
    get(const tuple<int, double, char>& t) noexcept
    {
      return getter<1>::get(t.rest);  // tuple<double, char> rest
    }
  };
  template <>
  struct my::getter<1>
  {
    template <>
    constexpr static const typename
    nth_type<2, int, double, char>::value_type&
    get(const tuple<double, char>& t) noexcept
    {
      return getter<1>::get(t.rest);  // tuple<char> rest
    }
  };
  ```
- and one definition from the specialization for `getter`:
  ```c++
  template <>
  struct getter<0>
  {
    template <>
    constexpr static const char&
    get(const tuple<char>& t) noexcept
    {
      return t.value;                 // char value
    }
  };
  ```
Because implementing variadic templates is often verbose and can be cumbersome, the C++17 standard added fold expressions to ease this task.

## Fold expressions

The instantiation of a fold expression expands the expression `e` as follows:

| fold type     | syntax                  | expended to                               |
|---------------|-------------------------|-------------------------------------------|
| unary right   | `(pack op ...)`         | `(e1 op (... op (eN-1 op eN)))`           |
| unary left    | `(... op pack)`         | `(((e1 op e2) op ...) op eN)`             |
| binary right  | `(pack op ... op init)` | `(e1 op (... op (eN-1 op (eN op init))))` |
| binary left   | `(init op ... op pack)` | `((((init op e1) op e2) op ...) op eN)`   |

Where:

- `op` is any of the designated 32 operators such as `+`
- `pack` is an expression that contains an unexpanded pack
- `init` is an expression that does not contain an unexpanded pack

> Note that the enclosing parentheses are part of the fold expression.

When a unary fold is used with a pack expansion of length zero, only the following operators are allowed:

- Logical AND (`&&`). The value for the empty pack is `true`.
- Logical OR (`||`). The value for the empty pack is `false`.
- The comma operator (`,`). The value for the empty pack is `void()`.

A basic usage, folding variadic arguments over operator`<<`:

```c++
template <typename... Args>
void printer(Args&&.. args)
{
  (std::cout << ... << args) << '\n';
  // Binary left fold
  // Syntax: `(init op ... op pack)`
  // Expansion: `((((o << e1) << e2) << e3) << e4)`
}
```

Another example for using the parameter pack or the variadic arguments in an expression inside a fold expression:

```c++
template <typename T, typename... Args>
void push_back_vec(std::vector<T>& v, Args&&... args)
{
  // a fold over operator&& using the parameter pack
  static_assert((std::is_constructible_v<T, Args&&> && ...));
  // Unary right fold
  // Syntax: `(pack op ...)`
  // Expansion: `(<A1&&> && (<A2&&> && (<A3&&> && <A4&&>)))`

  // a fold over operator, using the variadic arguments
  (v.push_back(std::forward<Args>(args)), ...);
  // Unary right fold
  // Syntax: `(pack op ...)`
  // Expansion: `(pb(e1), (pb(e2), (pb(e3), pb(e4))))`
}
push_back_vec(v, 6, 2, 45, 12);
push_back_vec(v, 1, 2, 9);
// v = {6, 2, 45, 12, 1, 2, 9}
```

([Fold expressions, C++ reference](https://www.cppreference.com/w/cpp/language/fold.html))

For the built-in comma operator, a pair of expression separated by a comma is evaluated left to right.

```c++
template <typename... T>
void print_left(T... args)
{
  (..., (std::cout << args)) << '\n';
  // ((((o << a1), o << a2), ...), o << aN)
}
template <typename... T>
void print_right(T... args)
{
  ((std::cout << args), ...) << '\n';
  // (o << a1, (o << a2, (..., (o << aN))))
}
```

Note that the parameter pack `args` is part of the `(std::cout << args)` expression (using the variadic arguments), which is not a fold expression. A fold expression is `((std::cout << args), ...)`, which is a unary right fold over the comma operator.

> The variadic arguments `args` can be part of a non-fold expression inside a fold expression, as long as it is not expanded (that is, `args` alone, not `args...`).

## Variadic alias templates

An alias template is an alias (another name) for a family of types. A variadic alias template is a name for a family of types with a variable number of template parameters.

The standard library contains a class called `std::integer_sequence` in `<utility>`, which represents a compile-time sequence of integers.

```c++
// T is an integer type to use for the elements of the sequence
// T... Ints represents the sequence
template <typename T, T... Ints>
class integer_sequence;
```

A variadic alias template `std::index_sequence` is defined for the common case where `T` is `std::size_t`:

```c++
// partial specialization for std::size_t
template <std::size_t... Ints>
using index_sequence = std::integer_sequence<std::size_t, Ints...>;
```

Helper alias templates are also defined to simplify things with `index_sequence`:

```c++
// simplifies creation of integer_sequence
template <typename T, T N>
using make_integer_sequence = std::integer_sequence<T, /* 0, 1, 2, ..., N-1 */>;

// simplifies creation of index_sequence
template <std::size_t N>
using make_index_sequence = std::make_integer_sequence<std::size_t, N>;

// convert any type parameter pack into an index sequence of the same length:
template <typename... T>
using index_sequence_for = std::make_index_sequence<sizeof...(T)>;
```

When `std::integer_sequence` is used as an argument to a function template, the parameter pack (`I` in the following snippet) can be deduced and used in pack expansion.

```c++
#include <array>
#include <cstddef>
#include <tuple>
#include <utility>

// this function signature exposes implementation a bit
template <typename T, std::size_t... I>
constexpr auto to_tuple_i(const std::array<T, sizeof...(I)> a, std::index_sequence<I...>)
{
  return std::make_tuple(a[I]...);
}

// this function signature hide implementation
template <typename T, std::size_t N>
constexpr auto to_tuple(const std::array<T, N> a)
{
  return to_tuple_i(a, std::make_index_sequence<N>{});
}

constexpr std::array a1{1,2,3};
constexpr auto t1 = to_tuple(a1);
static_assert(std::is_same_v<decltype(t1), const std::tuple<int, int, int>>);
static_assert(1 == get<0>(t1));
static_assert(2 == get<1>(t1));
static_assert(3 == get<2>(t1));
constexpr auto t1x = to_tuple_i(a1, std::make_index_sequence<a1.size()>{});
static_assert(t1 == t1x);

// this function signature exposes implementation a bit
template <typename... T, std::size_t... N>
constexpr auto to_array_i(const std::tuple<T...> t, std::index_sequence<N...>)
{
  return std::array{get<N>(t)...};
}

// this function signature hide implementation
template <typename... T>
constexpr auto to_array(const std::tuple<T...> t)
{
  return to_array_i(t, std::make_index_sequence<sizeof...(T)>{});
}

constexpr auto t2 = std::make_tuple<int>(4,5,6,7);
constexpr auto a2 = to_array(t2);
static_assert(std::is_same_v<decltype(a2), const std::array<int, 4>>);
static_assert(4 == a2[0]);
static_assert(5 == a2[1]);
static_assert(6 == a2[2]);
static_assert(7 == a2[3]);
constexpr auto
a2x = to_array_i(t2, std::make_index_sequence<std::tuple_size_v<decltype(t2)>>{});
static_assert(a2 == a2x);
```

