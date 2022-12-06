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
```

| argument             | call    | `T`         | `param`
|----------------------|---------|-------------|------------
| `int x = 27;`        | `f(x)`  | `int`       | `int&`
| `const int cx = x;`  | `f(cx)` | `const int` | `const int&`
| `const int& rx = x;` | `f(rx)` | `const int` | `const int&`

Even though `rx`'s type is a reference, `T` is deduced to be a non-reference.

```c++
template<typename T>
void f(const T& param);
```

| argument             | call    | `T`   | `param`
|----------------------|---------|-------|------------
| `int x = 27;`        | `f(x)`  | `int` | `const int&`
| `const int cx = x;`  | `f(cx)` | `int` | `const int&`
| `const int& rx = x;` | `f(rx)` | `int` | `const int&`

The constness of `cx` and `rx` continues to be respected, but because we're now assuming that `param` is a reference-to-const, there's no longer a need for `const` to be deduced as part of `T`.

As before, `rx`'s reference-ness is ignored during type deduction.

```c++
template<typename T>
void f(T* param);
```

| argument              | call    | `T`         | `param`
|-----------------------|---------|-------------|------------
| `int x = 27;`         | `f(&x)` | `int`       | `int*`
| `const int *px = &x;` | `f(px)` | `const int` | `const int*`

Everything's just obvious! Which is exactly what you want in a type deduction system.

### Case 2: `ParamType` is a Universal Reference

Universal reference parameters are declared like rvalue references, but they behave differently when lvalue arguments are passed in.

```c++
template<typename T>
void f(T&& param);
```

If `expr` is an lvalue, both `T` and `ParamType` are deduced to lvalue references.

| argument             | call    | `T`          | `param`
|----------------------|---------|--------------|------------
| `int x = 27;`        | `f(x)`  | `int&`       | `int&`
| `const int cx = x;`  | `f(cx)` | `const int&` | `const int&`
| `const int& rx = x;` | `f(rx)` | `const int&` | `const int&`

If `expr` is an rvalue, the case 1 rules apply.

| argument             | call    | `T`          | `param`
|----------------------|---------|--------------|------------
| `expr` is an rvalue  | `f(27)` | `int`        | `int&&`

When universal references are in use, type deduction distinguishes between lvalue arguments and rvalue arguments. That never happens for non-universal references.

### Case 3: `ParamType` is Neither a Pointer nor a Reference


