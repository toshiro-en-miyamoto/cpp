# Constant Expressions

[Constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression) are the expressions that can be evaluated at compile time. Such expressions can be used as non-type template arguments, array sizes, and in other contexts that require constant expressions, e.g.

```c++
#include <array>
void constant_array()
{
    std::size_t n = 1;
//  std::array<int, n> a1;  // compile-time error
    const std::size_t cn = 2;
    std::array<int, cn> a2; // OK: cn is a constant expression
}
```


## [`const` objects](https://en.cppreference.com/w/cpp/language/cv)

For any type `T`, there are five distinct types:

-   `const T`
-   `volatile T`
-   `const volatile T`
-   reference type
-   function type

`const` objects cannot be modified: attempt to do so directly is a compile-time error, and attempt to do so indirectly results in undefined behavior.

```c++
int main()
{
    int n1 = 0;
    n1 = 1;
    const int& r1 = n1;
//  r1 = 2;                         // compile-time error
    const_cast<int&>(r1) = 2;       // ok

    const int n2 = 0;
//  n2 = 2;                         // compile-time error
    const int& r2 = n2;
//  r2 = 2;                         // compile-time error
    const_cast<int&>(r2) = 2;       // undefined behavior
}

#include <type_traits>

static_assert(std::is_const_v<int> == false);
static_assert(std::is_const_v<const int> == true);

// false because the pointer to int can be changed
static_assert(std::is_const_v<const int*> == false);

void f1(const int* p)
{
    p = nullptr;
//  *p = 1;                         // compile-time error
}

// as the pointer to int cannot be changed
static_assert(std::is_const_v<int* const> == true);

void f2(int* const p)
{
//  p = nullptr;                    // compile-time error
    *p = 1;
}
```

## [Constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization)

A variable is *constant-initialized* if

-   either
    -   it has an initializer or
    -   its default-initialization results in some initialization being performed,
    and
-   its initialization full-expression is a constant expression.

```c++
#include <array>
 
struct S
{
    static const int c;
};

// not a constant expression: S::c has no preceding initializer
const int d = 10 * S::c;

// constant initialization, guaranteed to happen first
const int S::c = 5;

int main()
{
//  std::array<int, d> a2;    // compile-time error
    std::array<int, S::c> a1; // OK
}
```

## [`constexpr`](https://en.cppreference.com/w/cpp/language/constexpr)

`constexpr` specifier declares that it is possible to evaluate the value of the variable at compile time. `constexpr` implies:

- `const` when used in an object declaration
- `inline` when used in a static data member declaration

`constexpr` specifier declares that it is possible to evaluate the value of the function at compile time. `constexpr` implies:

- `const` when used in a non-static member function
- `inline` when used in a function declaration
