# Deducing Types

## Item 1: Understand template type deduction

```c++
template<typename T>
void f(ParamType param);

f(expr);
```

During compilation, compilers use `expr` to deduce two types: one for `T` and one for `ParamType`.

The type deduced for `T` is dependent no just on the type of `expr`, but also the form of `ParamType`.

- `ParamType` is a pointer or reference type, but not a universal reference.
- `ParamType` is a unicersal reference.
- `ParamType` is neither a pointer nor a reference.

### Case 1: `ParamType` is a Reference or Pointer

In this case, type deduction works like this:

1. If `expr`'s type is a reference, ignore the reference part.
2. Then pattern-match `expr`'s type against `ParamType` to determine `T`.


```c++
template<typename T>
void f(T& param);

int x = 27;
const int cx = x;
const int& rx = x;
```

| signature     | call    | `T`         | `param`
|---------------|---------|-------------|------------
| `f(T& param)` | `f(x)`  | `int`       | `int&`
| `f(T& param)` | `f(cx)` | `const int` | `const int&`
| `f(T& param)` | `f(rx)` | `const int` | `const int&`

Even though `rx`'s type is a reference, `T` is deduced to be a non-reference.

```c++
template<typename T>
void f(const T& param);

int x = 27;
const int cx = x;
const int& rx = x;
```

The constness of `cx` and `rx` continues to be respected, but because we're now assuming that `param` is a reference-to-const, there's no longer a need for `const` to be deduced as part of `T`.

| signature           | call    | `T`   | `param`
|---------------------|---------|-------|------------
| `f(const T& param)` | `f(x)`  | `int` | `const int&`
| `f(const T& param)` | `f(cx)` | `int` | `const int&`
| `f(const T& param)` | `f(rx)` | `int` | `const int&`

As before, `rx`'s reference-ness is ignored during type deduction.

```c++
template<typename T>
void f(T* param);

int x = 27;
const int *px = &x;
```

| signature     | call     | `T`         | `param`
|---------------|----------|-------------|------------
| `f(T* param)` | `f(&x)`  | `int`       | `int*`
| `f(T* param)` | `f(px)`  | `const int` | `const int*`

Everything's just obvious! Which is exactly what you want in a type deduction system.

### Case 2: `ParamType` is a Universal Reference

Universal reference parameters are declared like rvalue references, but they behave differently when lvalue arguments are passed in.

```c++
template<typename T>
void f(T&& param);

int x = 27;
const int cx = x;
const int& rx = x;
```

If `expr` is an lvalue, both `T` and `ParamType` are deduced to lvalue references.

| signature      | call    | `T`          | `param`
|----------------|---------|--------------|------------
| `f(T&& param)` | `f(x)`  | `int&`       | `int&`
| `f(T&& param)` | `f(cx)` | `const int&` | `const int&`
| `f(T&& param)` | `f(rx)` | `const int&` | `const int&`

If `expr` is an rvalue, the case 1 rules apply.

| signature      | call    | `T`          | `param`
|----------------|---------|--------------|------------
| `f(T&& param)` | `f(27)` | `int`        | `int&&`

When universal references are in use, type deduction distinguishes between lvalue arguments and rvalue arguments. That never happens for non-universal references.

### Case 3: `ParamType` is Neither a Pointer nor a Reference

When `ParamType` is neither a pointer nor a reference, we’re dealing with pass-by-value:

```c++
template<typename T>
void f(T param);

int x = 27;
const int cx = x;
const int& rx = x;
```

That means that `param` will be a copy of whatever is passed in — a completely new object.

1. As before, if `expr`'s type is a reference, ignore the reference part.
2. If, after ignoring `expr`'s reference-ness, `expr` is `const`, ignore that, too.

| argument     | call    | `T`   | `param`
|--------------|---------|-------|------------
| `f(T param)` | `f(x)`  | `int` | `int`
| `f(T param)` | `f(cx)` | `int` | `int`
| `f(T param)` | `f(rx)` | `int` | `int`

Even though `cx` and `rx` represent `const` values, `param` isn’t `const`. That makes sense. `param` is an object that’s completely independent of `cx` and `rx` — a copy of `cx` or `rx`.

### Array Arguments

Here, the `const char*` ponter `p` is being initialized with `name`, which is a `const char[13]`. These types, `const char*` and `const char[13]`, are not the same, but because of the *array-to-pointer decay rule*, the code compiles.

```c++
const char name[] = "J. P. Briggs";
const char* p = name;
```

We begin with the observation that there is no such thing as a function parameter that’s an array. Yes, yes, the syntax is legal (1), but the array declaration is treated as a pointer declaration, meaning that `f()` could equivalently be declared like this (2):

```c++
void f(int param[]);    // (1)
void f(int* param);     // (2)

//  error: redefinition of ‘void f(int*)’
```

But what if an array is passed to a template taking a by-value parameter?

```c++
template<typename T>
void f(T param);

const char name[] = "J. P. Briggs";
```

Because array parameter declarations are treated as if they were pointer parameters, the type of an array that’s passed to a template function by value is deduced to be a pointer type.

| signature    | call      | `T`           | `param`
|--------------|-----------|---------------|------------
| `f(T param)` | `f(name)` | `const char*` | `const char*`

Although functions can’t declare parameters that are truly arrays, they can declare parameters that are references to arrays!

```c++
template<typename T>
void f(T& param);

const char name[] = "J. P. Briggs";
```

That type includes the size of the array, so in this example, `T` is deduced to be `const char [13]`. The type of argument is a reference to the array; `const char (&)[13]`.

| signature     | call      | `T`              | `param`
|---------------|-----------|------------------|----------------------
| `f(T& param)` | `f(name)` | `const char[13]` | `const char (&)[13]`

### Function Arguments

Function types can decay into function pointers.

```c++
template<typename T>
void f_val(T param);

template<typename T>
void f_ref(T& param);

void func(int, char);
```

| signature         | call          | `T`                   | `param`
|-------------------|---------------|-----------------------|----------------------
| `f_val(T param)`  | `f_val(func)` | `void (*)(int, char)` | `void (*)(int, char)`
| `f_ref(T& param)` | `f_ref(func)` | `void (&)(int, char)` | `void (&)(int, char)`

`void (*)(int, char)` is a pointer to functions, and `void (&)(int, char)` is a reference to functions.

### Summary of Item 1

```c++
template<typename T>
void f_val(T param);

template<typename T>
void f_ptr(T* param);

template<typename T>
void f_ref(T& param);

template<typename T>
void f_cref(const T& param);

template<typename T>
void f_refref(T&& param);

int x = 27;
const int cx = x;
const int& rx = x;
const int *px = &x;

const char name[] = "J. P. Briggs";
void func(int, char);
```

Case 1: During template type deduction, arguments that are references are treated as non-references, i.e., their reference-ness is ignored.

| signature                | call           | `T`         | `param`
|--------------------------|----------------|-------------|------------
| `f_ref(T& param)`        | `f_ref(x)`     | `int`       | `int&`
| `f_refref(T&& param)`    | `f_refref(27)` | `int`       | `int&&`
| `f_ref(T& param)`        | `f_ref(cx)`    | `const int` | `const int&`
| `f_ref(T& param)`        | `f_ref(rx)`    | `const int` | `const int&`
| `f_cref(const T& param)` | `f_cref(x)`    | `int`       | `const int&`
| `f_cref(const T& param)` | `f_cref(cx)`   | `int`       | `const int&`
| `f_cref(const T& param)` | `f_cref(rx)`   | `int`       | `const int&`
| `f_ptr(T* param)`        | `f_ptr(&x)`    | `int`       | `int*`
| `f_ptr(T* param)`        | `f_ptr(px)`    | `const int` | `const int*`

Case 2: When deducing types for universal reference parameters, lvalue arguments get special treatment.

| signature             | call           | `T`          | `param`
|-----------------------|----------------|--------------|------------
| `f_refref(T&& param)` | `f_refref(x)`  | `int&`       | `int&`
| `f_refref(T&& param)` | `f_refref(cx)` | `const int&` | `const int&`
| `f_refref(T&& param)` | `f_refref(rx)` | `const int&` | `const int&`

Case 3: When deducing types for by-value parameters, `const` arguments are treated as non-`const`.

| signature        | call        | `T`   | `param`
|------------------|-------------|-------|------------
| `f_val(T param)` | `f_val(x)`  | `int` | `int`
| `f_val(T param)` | `f_val(cx)` | `int` | `int`
| `f_val(T param)` | `f_val(rx)` | `int` | `int`

During template type deduction, arguments that are array or function names decay to pointers, unless they’re used to initialize references.

| signature         | call          | `T`                   | `param`
|-------------------|---------------|-----------------------|----------------------
| `f_val(T param)`  | `f_val(name)` | `const char*`         | `const char*`
| `f_ref(T& param)` | `f_ref(name)` | `const char[13]`      | `const char (&)[13]`
| `f_val(T param)`  | `f_val(func)` | `void (*)(int, char)` | `void (*)(int, char)`
| `f_ref(T& param)` | `f_ref(func)` | `void (&)(int, char)` | `void (&)(int, char)`

## Item 2: Understand `auto` type deduction

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

auto&& uref1 = x;
auto&& uref2 = cx;
auto&& uref3 = 27;
```

| for     | `ParamType` | Item 1 | `T`          | variable type
|---------|-------------|--------|--------------|------------------
| `x`     | `T`         | Case 3 | `int`        | `int`
| `cx`    | `const T`   | Case 3 | `int`        | `const int`
| `rx`    | `const T&`  | Case 1 | `int`        | `const int&`
| `uref1` | `T&&`       | Case 2 | `int&`       | `int&`
| `uref2` | `T&&`       | Case 2 | `const int&` | `const int&`
| `uref3` | `T&&`       | Case 2 | `int`        | `int&&`

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

### `auto` with braced-init-list

Four syntaxes, but only one result; an `int` with value 27.

```c++
int x1 = 27;        // since C++98: copy-initialization
int x2(27);         // since C++98: direct-initialization

int x3 = {27};      // since C++11: copy-list-initialization
int x4{27};         // since C++11: directo-list-initialization
```

You can replace `int` with `auto` in the above variable declarations:

```c++
auto x1 = 27;
auto x2(27);
auto x3 = {27};
auto x4{27};
```

These declarations all complies, but a *special type deduction rule for `auto`* is applied to `x3`:

| sytax             | type                         | value
|-------------------|------------------------------|--------------
| `auto x1 = 27;`   | `int`                        | `27`
| `auto x2(27);`    | `int`                        | `27`
| `auto x3 = {27};` | `std::initializer_list<int>` | `{27}`
| `auto x4{27};`    | `int`                        | `27`

> A special exception is made for type deduction using the keyword `auto`, which deduces any braced-init-list as `std::initializer_list` in copy-list-initialization.  [List-initialization](https://en.cppreference.com/w/cpp/language/list_initialization)

> In direct-list-initialization (but not in copy-list-initalization), when deducing the meaning of the `auto` from a braced-init-list, the braced-init-list must contain only one element, and the type of `auto` will be the type of that element. [auto type deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Other_contexts)

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
