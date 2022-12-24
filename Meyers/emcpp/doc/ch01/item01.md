# Item 1: Understand template type deduction

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

## Case 1: `ParamType` is a Reference or Pointer

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

## Case 2: `ParamType` is a Universal Reference

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

## Case 3: `ParamType` is Neither a Pointer nor a Reference

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

## Array Arguments

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

## Function Arguments

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

## Summary of Item 1

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
