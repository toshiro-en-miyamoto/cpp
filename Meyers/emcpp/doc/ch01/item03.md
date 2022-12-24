# Item 3: Understand `decltype`

Suppose we’d like to write a function that takes a container that supports indexing via square brackets (i.e., the use of `[]`) plus an index, then authenticates the user before returning the result of the indexing operation.

A trailing return type has the advantage that the function’s parameters can be used in the specification of the return type.

```c++
template<typename Container, typename Index>
auto auth_and_access(Container& c, Index i)
    -> decltype(c[i])
{
    auth();
    return c[i];
}
```

In C++14 we can omit the trailing return type, leaving just the leading `auto`. Compilers will deduce the function’s return type from the function’s implementation:(Return type deduction)

```c++
template<typename Container, typename Index>
auto auth_and_access(Container& c, Index i)
{
    auth();
    return c[i];        // return type deduced from c[i]
}

std::deque<int> d;
auth_and_access(d, 5) = 10;
```

Here, `d[5]` returns an `int&`, but `auto` return type deduction for `auth_and_access` will strip off the reference, thus yielding a return type of `int`. That `int`, being the return value of a function, is an rvalue, and the code above thus attempts to assign 10 to an rvalue `int`. That’s forbidden in C++, so the code won’t compile.

To get `auth_and_access` to work as we’d like, we need to use `decltype` type deduction for its return type, i.e., to specify that `auth_and_access` should return exactly the same type that the expression `c[i]` returns.

```c++
template<typename Container, typename Index>
decltype(auto) auth_and_access(Container& c, Index i)
{
    auth();
    return c[i];        // return type deduced from c[i]
}
```

## [Trailing return type](https://en.cppreference.com/w/cpp/language/function)

Trailing return type is useful if the return type depends on argument names, such as:

```c++
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u)
{
    return t + u;
}
```

It is also useful if the return type is complicated, such as:

```c++
void f0(const std::string& arg = "world!")
{
    std::cout << "Hello, " << arg << '\n';
}

// function returning a pointer to f0, pre-C++11 style
void (*fp03())(const std::string&)
{
    return f0;
}
 
// function returning a pointer to f0, with C++11 trailing return type
auto fp11() -> void(*)(const std::string&)
{
    return f0;
}
```

## [`decltype` specifier](https://en.cppreference.com/w/cpp/language/decltype)

| Syntax                        |  #  | version
|-------------------------------|-----|----
| `decltype (` *entity* `)`     | (1) | since C++11
| `decltype (` *expression* `)` | (2) | since C++11

1. If the argument is an unparenthesized id-expression or an unparenthesized class member access expression, then `decltype` yields the type of the *entity* named by this expression.
2. If the argument is any other expression of type `T`, and
    - (a) if the value category of *expression* is *xvalue*, then `decltype` yields `T&&`;
    - (b) if the value category of *expression* is *lvalue*, then `decltype` yields `T&`;
    - (c) if the value category of *expression* is *prvalue*, then `decltype` yields `T`.

Note that if the name of an object is parenthesized, it is treated as an ordinary lvalue expression, thus `decltype(x)` and `decltype((x))` are often different types.

```c++
struct A { double x; };
const A* a;

decltype(a->x) y;
// type of y is double (declared type, #1)
static_assert(std::is_same_v<decltype(y), double>);

decltype((a->x)) z = y;
// type of z is const double& (lvalue expression, #2.b)
static_assert(std::is_same_v<decltype(z), const double&>);
```

If the return type does not use `decltype(auto)`, the deduction follows the rules of *template argument deduction*:

```c++
const int& f1(const int* p) { return *p; }
static_assert(std::is_same_v<decltype(f1), const int&(const int*)>);

auto f2(const int* p) { return f1(p); }
static_assert(std::is_same_v<decltype(f2), int(const int*)>,
    "Just returning auto isn't perfect forwarding.");
```

If the return type is `decltype(auto)`, the return type is as what would be obtained if the expression used in the `return` statement were wrapped in `decltype`:

```c++
decltype(auto) f3(const int* p) { return f1(p); }
static_assert(std::is_same_v<decltype(f3), const int&(const int*)>,
    "Returning decltype(auto) perfectly forwards the return type.");
```

## [Return type deduction](https://en.cppreference.com/w/cpp/language/function#Return_type_deduction)

If the decl-specifier-seq of the function declaration contains the keyword `auto`, trailing return type may be omitted, and will be deduced by the compiler from the type of the expression used in the `return` statement.

If the return type does not use `decltype(auto)`, the deduction follows the rules of *template argument deduction*:

```c++
int x = 1;

auto f1() { return x; }
static_assert(std::is_same_v<decltype(f1), int()>);

const auto& f2() { return x; }
static_assert(std::is_same_v<decltype(f2), const int&()>);
```

If the return type is `decltype(auto)`, the return type is as what would be obtained if the expression used in the `return` statement were wrapped in `decltype`:

```c++
int x = 1;

decltype(auto) f3() { return x; }
static_assert(std::is_same_v<decltype(f3), int()>);

decltype(auto) f4() { return (x); }
static_assert(std::is_same_v<decltype(f4), int&()>);
```
