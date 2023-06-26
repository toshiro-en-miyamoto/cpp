# Computation

## Expressions

```c++
length = 99;
```

We distinguish between
- `length` used on the left-hand side of an assignment or an initialization (the *lvalue* of `length` or *the object named by `length`*) and
- `length` used on the right-hand side of an assignment or initialization (the *rvalue* of `length`, *the value of the object named by `length`, or just *the value of `length`*).

Why should you care about readability? Because you and possibly others will read your code, and ugly code slows down reading and comprehension. Ugly code is not just hard to read, it is also much harder to get correct. Ugly code often hides logical errors. It is slower to read and makes it harder to convince yourself — and others — that the ugly code is correct.

### Constant expressions

```c++
constexpr int max = 17;     // a literal is a constant expression
int val = 19;
max + 2;                    // a constant expression (a const int plus a literal)
val + 2;                    // not a constant expression: it uses a variable
```

A `constexpr` symbolic constant must be given a value that is known at compile time.

```c++
constexpr int max = 100;
void use(int n)
{
  constexpr int c1 = max + 7;     // OK: c1 is 107
  constexpr int c2 = n + 7;       // error: we don't know the value of c2
}
```

But what if we don’t know the value of a constant until run time? For that, C++ offers a second form of constant (a `const`):

```c++
constexpr int max = 100;
void use(int n)
{
  constexpr int c1 = max + 7;     // OK: c1 is 107
  const int c2 = n + 7;           // OK, but don't try to change the value of c2
  // ...
  c2 = 7;                         // error: c2 is a const
}
```

## Statements

Expression statements:

```c++
a = b;
++b;
```

Note that the assignment `=` is an operator so that `a = b` is an expression. We need the terminating semicolon to make the expression a statement.

Declarations:

```c++
int a = 7;
```

Other statements:

- empty statements
- `if`-statements
- `switch`-statements
- `while`-statements
- `for`-statements

## Functions

```c++
for (int i = 0; i < 100, ++i)
  cout << i << '\t' << square(i) << '\n';
```

What was `square(i)`? It is a call of the function called `square` with the argument `i`.

Here is a plausible definition of `square`:

```c++
int square(int x)
{
  return x * x;
}
```

The syntax of a *function definition*:

```
type identifier ( parameter-list ) function-body
```

- type: the type of the return value
- identifier: the name of the function
- parameter-list: a list of parameter in parentheses
- function-body: the statements to be executed

All the information needed to call a function was in the first line of its definition. It is called a *function declaration*. Note that a terminating semicolon is used instead of the function body.

```c++
int square(int x);
```

Given that, we know enough to say

```c++
int result = square(44);
```
