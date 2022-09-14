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
constexpr int max = 17;     // a litrral is a constant expression
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

- 4.3.3 Conversion
