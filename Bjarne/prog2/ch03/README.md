# Objects, Types, and Values

## Assignment and initialization

- Initialization: giving a variable its initial value
- Assignment: giving a variable a new value

These operations are so similar that C++ alows us to use the same notation (the `=`) for both:

```c++
std::string s = "howdy!";
s = "G'day";
```

However, logically assignment and initialization are different. You can tell the two apart by the type specification (like `int` or `string`) that always starts an initialization; an assignment does not have that. In principle, an initialization always finds the variable empty. On the other hand, an assignment (in principle) must clear out (*destroy*) the old value from the variable before putting in the new value.

You can think of the variable as a kind of small box and the value as a concrete thing, such as a coin, that you put into it. Before initialization, the box is empty, but after initialization it always holds a coin so that to put a new coin in, you (i.e., the assignment operator) first have to remove the old one (*destroy the old value*).

## Type safety

```c++
double x = 2.7;
int y = x;          // y becomes 2
```

A `double`-to-`int` conversion truncates (always rounds down, toward zero) rather than using the conventional `4/5` rounding. What happens is perfectly predictable, but there is nothing in the `int y = x`; to remind us that information (the `.7`) is thrown away.

Conversions from `int` to `char` don’t have problems with truncation — neither `int` nor `char` can represent a fraction of an integer. However, a `char` can hold only very small integer values. On a PC, a `char` is 1 byte whereas an `int` is 4 bytes:

So, we can’t put a large number, such as `1000`, into a `char` without loss of information: the value is **narrowed.** For example:

```c++
int a = 1000;
char b = a;         // becomes -24 on some machines
```

C++11 introduced an initialization notation that outlaws narrowing conversions. For example, we could (and should) rewrite the troublesome examples above using a `{}`-list notation, rather than the `=` notation:

```c++
double x {2.7};     // OK
int y {x};          // error: double -> int might narrow
int a {1000};       // OK
char b {a};         // error: int -> char might narrow
```

When the initializer is an integer literal, the compiler can check the actual value and accept values that do not imply narrowing:

```c++
char b1 {1000};     // error: narrowing assuming 8-bit chars
char b2 {48};       // OK
```

The `{}`-list-based notation is known as *universal and uniform initialization*.
