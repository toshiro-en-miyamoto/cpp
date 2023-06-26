# Item 2: Understand `auto` type deduction

`auto` type deduction is template type deduction, with only one curious exception.

In Item 1, template type deduction is explained using this general function template. In the call to `f(expr)`, compilers use `expr` to deduce types of `T` and `ParamType`.

```c++
template<typename T>
void f(ParamType param);

f(expr);
```

When a variable declared using `auto`, `auto` plays the role of `T` in the template, and the type specifier for the variable acts as `ParamType`.

```c++
auto x = 27;
const auto cx = x;
const auto& rx = x;

auto&& refref1 = x;
auto&& refref2 = cx;
auto&& refref3 = 27;
```

| for       | `ParamType` | Item 1 | `T`          | variable type
|-----------|-------------|--------|--------------|------------------
| `x`       | `T`         | Case 3 | `int`        | `int`
| `cx`      | `const T`   | Case 3 | `int`        | `const int`
| `rx`      | `const T&`  | Case 1 | `int`        | `const int&`
| `refref1` | `T&&`       | Case 2 | `int&`       | `int&`
| `refref2` | `T&&`       | Case 2 | `const int&` | `const int&`
| `refref3` | `T&&`       | Case 2 | `int`        | `int&&`

Array and function names decay into pointers for non-reference type specifiers. That happens in `auto` type deduction:

```c++
const char name[] = "R. N. Briggs";
auto arr1 = name;
auto& arr2 = name;

void func(int, char);
auto func1 = func;
auto& func2 = func;
```

| for     | `ParamType` | Item 1 | `T` and variable type
|---------|-------------|--------|----------------------
| `arr1`  | `T`         | Array  | `const char*`
| `arr2`  | `T&`        | Array  | `const char (&)[13]`
| `func1` | `T`         | Func   | `void (*)(int, char)`
| `func2` | `T&`        | Func   | `void (&)(int, char)`

## [`auto` type deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Other_contexts)

*Template argument deduction* is used in declarations of variables, when deducing the meaning of the `auto` specifier from the variable's initializer.

```c++
const auto& x = 1 + 2;      // P = const U&, A = 1 + 2:
```

The parameter `P` is obtained as follows: in `T`, the declared type of the variable that includes `auto`, every occurrence of `auto` is replaced with an imaginary type template parameter `U`. The argument `A` is the initializer expression.

The same rules as for calling `f(1 + 2)` where `f` is:

```c++
template<typename U> void f(const U& u);
```

which deduced `U = int`, the type of `u` is `const int&`.

## `auto` with braced-init-list

Four syntaxes, but only one result; an `int` with value 27.

```c++
int x1 = 27;        // since C++98: copy-initialization
int x2(27);         // since C++98: direct-initialization

int x3 = {27};      // since C++11: copy-list-initialization
int x4{27};         // since C++11: direct-list-initialization
```

You can replace `int` with `auto` in the above variable declarations:

```c++
auto x1 = 27;
auto x2(27);
auto x3 = {27};
auto x4{27};
```

These declarations all complies, but a *special type deduction rule for `auto`* is applied to `x3`:

| syntax            | type                         | value
|-------------------|------------------------------|--------------
| `auto x1 = 27;`   | `int`                        | `27`
| `auto x2(27);`    | `int`                        | `27`
| `auto x3 = {27};` | `std::initializer_list<int>` | `{27}`
| `auto x4{27};`    | `int`                        | `27`

> A special exception is made for type deduction using the keyword `auto`, which deduces any braced-init-list as `std::initializer_list` in copy-list-initialization.  [List-initialization](https://en.cppreference.com/w/cpp/language/list_initialization)

> In direct-list-initialization (but not in copy-list-initialization), when deducing the meaning of the `auto` from a braced-init-list, the braced-init-list must contain only one element, and the type of `auto` will be the type of that element. [auto type deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Other_contexts)

```c++
auto x1 = {3};  // x1 is std::initializer_list<int>
auto x2{1, 2};  // error: not a single element
auto x3{3};     // x3 is int
// before N3922, x2 and x3 were both std::initializer-list<int>
```

> A braced-init-list is not an expression and therefore has no type, e.g. `decltype({1,2})` is ill-formed. Having no type implies that template type deduction cannot deduce a type that matches a braced-init-list, so given the declaration `template<class T> void f(T);`, the expression `f({1,2,3})` is ill-formed. [List-initialization](https://en.cppreference.com/w/cpp/language/list_initialization)

```c++
template<typename T>
void f(T);

f({11, 23, 9});             // can't deduce type for T
```

However, if you specify in the template function that takes a `std::initializer_list<T>`, template type deduction will deduce what `T` is:

```c++
template<typename T>
void f(std::initializer-list<T>);

f({11, 23, 9});             // T: int, std::initializer-list<int>
```

## `auto` in a structured binding (since C++17)

The `auto` specifier may also be used in a structured binding declaration.

```c++
#include <typeinfo>
#include <utility>
#include <boost/ut.hpp>

using type_ref = const std::type_info&;
using type_pair = std::pair<type_ref, type_ref>;

template<typename T> type_pair ff_val(T param)
{ return {typeid(T), typeid(param)}; }

void f()
{
  const char name[] = "Briggs";
  auto [type_arg, param_arg] = ff_val(name);

  using ch_ptr_t = const char*;
  expect(type_arg  == typeid(ch_ptr_t));
  expect(param_arg == typeid(ch_ptr_t));
}
```
