# Folding Expressions

References:
- 15.12.2 Variadic Function Templates and an Intro to C++17 Fold Expressions, *C++20 for Programmers*, Paul and Harvey Deitel

Variadic function templates enable you to define functions that can receive any number of arguments. The `sum()` uses variadic function templates to sum one or more arguments.

```c++
#include <concepts>

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<Numeric T, Numeric... Ts>
constexpr auto sum(T init, Ts... pack)
{
  return (init + ... + pack);
}
```

The preceding code use a **binary left fold** `(init + ... + pack)`, which sums `init` and the zero or more arguments in the parameter pack `pack`.

- Fold expressions must be parenthesized.
- A binary-left-fold expression has two binary operators, which must be the same (`+` in this case).

The argument to the left to the first operator (`init`) is the expression's **initial value**. The `...` expands the parameter pack (`pack`) to the right of the second operator, separating each parameter in the parameter pack from the next with the binary operator.

The C++ Core Guidelines recommends using

- variadic function templates for arguments of mixed types, and
- `initializer_list`s for functions that receive variable numbers of arguments of the same type.

There are four fold-expression types—the parentheses are required in each:

- a unary left fold
  ```
  (... o pack)
  (((1 o 2) o 3) o 4)
  ```
- a unary right fold
  ```
  (pack o ...)
  (1 o (2 o (3 o 4)))
  ```
- a binary left fold
  ```
  (init o ... o pack)
  (((init o 1) o 2) o 3)
  ```
- a binary right fold
  ```
  (pack o ... o init)
  ((1 o (2 o 3)) o init)
  ```
