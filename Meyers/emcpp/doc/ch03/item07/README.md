# [Initialization](https://en.cppreference.com/w/cpp/language/initialization)

*Initialization* of a variable provides its nitial value at the time of construction.

| Initializer
|--------
| `(` expression-list `)`
| `=` expression
| `{` initializer-list `}`

Depending on context, the initializer may invoke:

- value initialization
- direct initialization
- copy initialization
- list initialization
- aggregate initialization
- referene initialization

## Aggregate initialization

An *aggregate* is one of the following types:

- array type
- class type (typically `struct` or `uion`), that has
  - no user-declated constructors (or inherited constructors)
  - no private or protected direct non-static data members
  - no virtual base classes
  - no private or protected direct base classes
  - no virtual member functions

| Syntax
|------------
| T object `={`arg1, arg2,...`}`;
| T object `{`arg1, arg2,...`}`;
| T object `={`.des1`=`arg1, .des2`{`arg2`}`,...`}`;
| T object `{`.des1`=`arg1, .des2`{`arg2`}`,...`}`;

```c++
struct A {
    int x;
    struct B {
        int i, j;
    } b;
} a {1, {2,3}};
```

Each designator in a designated initializer must name a direct non-static data member:

```c++
struct A { int x; int y; int z; };
 
A a{.y = 2, .x = 1}; // error; not the same order
A b{.x {1}, .z = 2}; // ok, b.y initialized to 0
```

All designators used in the expression must appear in the same order as the data members.

## Reference initialization

Reference initialization binds a reference to an object. Once initialized, a reference cannot be changed to refer to another object.

A lvalue or rvalue reference variable is initialized when it is declared with an initializer:

| lvalue ref                       | ravlue ref
|----------------------------------|-------------------
| T`&` ref `=` target;             | T`&&` ref `=` target;
| T`&` ref `={`arg1, arg2,...`}`;  | T`&&` ref` ={`arg1, arg2,...`}`;
| T`&` ref `{`target`}`;           | T`&&` ref `{`target`}`;
| T`&` ref `{`arg1, arg2,...`}`;   | T`&&` ref `{`arg1, arg2,...`}`;

A lvalue or rvalue reference parameter is initialized in a function call when the function parameter has referece type:

| given           | call
|-----------------|-------
| R f(T`&` arg);  | f`(`target`)`
|                 | f`({`arg1, arg2,...`})`
| R f(T`&&` arg); | f`(`target`)`
|                 | f`({`arg1, arg2,...`})`

A lvalue or rvalue reference parameter is initialized in the `return` statement when the function returns a referece type:

| inside    | return
|-----------|--------------------
| T`&` f()  | `return` target;
| T`&&` f() | `return` target;

A non-static data member of lvalue or rvalue reference type is initialized using a member initializer of constructor:

| in a class | constructor definition
|------------|--------------------------
| T`&` ref;  | class::class(...) : ref`(`target`)` {...}
| T`&&` ref; | class::class(...) : ref`(`target`)` {...}

> `: ref{target}` ?
