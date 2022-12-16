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

## List initialization

List initialization initializes an object from *braced-init-list*.

| Direct list initialization
|----------------------------
| T object `{`arg1, arg2,...`}`;
| T `{`arg1, arg2,...`}`
| `new` T `{`arg1, arg2,...`}`
| class { T member `{`arg1, arg2,...`}`; };
| class::class() : member `{`arg1, arg2,...`}` {...}

| Copy list initialization
|----------------------------
| T object `={`arg1, arg2,...`}`;
| function(`{`arg1, arg2,...`}`)
| `return {`arg1, arg2,...`}`;
| object[`{`arg1, arg2,...`}`]
| object = `{`arg1, arg2,...`}`
| U(`{`arg1, arg2,...`}`)
| class { T member = `{`arg1, arg2,...`}`; };

1. If `T` is an aggregate class and the braced-init-list has a single element of the same or derived type (possibly cv-qualified), the object is initialized from that element (by copy-initialization for copy-list-initialization, or by direct-initialization for direct-list-initialization).

```c++
struct Topic1
{
    int n;
    char c;
};

void topic1()
{
    // topic 3: aggregate initialization
    constexpr Topic1 ic1 {1, 'a'};

    constexpr Topic1 ic2 {ic1};
    static_assert(ic2.n == 1);

    constexpr Topic1 ic3 = {ic1};
    static_assert(ic3.n == 1);
}
```

2. Otherwise, if `T` is a character array and the braced-init-list has a single element that is an appropriately-typed string literal, the array is initialized from the string literal as usual.

```c++
void topic2()
{
    constexpr char a[] {"abc"};
    static_assert(a[2] == 'c');
}
```

3. Otherwise, if T is an aggregate type, aggregate initialization is performed.

4. Otherwise, if the braced-init-list is empty and `T` is a class type (`class`, `struct`, or `union`) with a default constructor, value-initialization is performed.

```c++
struct Topic4
{
    constexpr Topic4()
        : n {4}, c {'b'}
    {}

    int n;
    char c;
};

void topic4()
{
    constexpr Topic4 a {};
    static_assert(a.n == 4);
    static_assert(a.c == 'b');
}
```

5. Otherwise, if `T` is a specialization of `std::initializer_list`, the `T` object is direct-initialized or copy-initialized, depending on context, from a prvalue of the same type initialized from (until C++17) the braced-init-list.

6. Otherwise, the constructors of `T` are considered, in two phases:

- All constructors that take `std::initializer_list` as the only argument, or as the first argument if the remaining arguments have default values, are examined, and matched by overload resolution against a single argument of `type std::initializer_list`

```c++
template<std::integral T>
struct Topic6a
{
    const T sum;
    const T a, b, c;

    constexpr Topic6a(std::initializer_list<T> list)
        : sum {
            std::accumulate(
                list.begin(), list.end(), 0, std::plus<T>()
            )}
        , a {0}, b {0}, c {0}
    {}

    constexpr Topic6a(T v0, T v1, T v2, T v3)
        : sum {v0}
        , a {v1}, b {v2}, c {v3}
    {}
};

void topic6a()
{
    constexpr Topic6a<int> a {1, 2, 3, 4};
    static_assert(10 == a.sum);
}
```

- If the previous stage does not produce a match, all constructors of `T` participate in overload resolution against the set of arguments that consists of the elements of the braced-init-list, with the restriction that only non-narrowing conversions are allowed.

```c++
template<typename T>
struct Topic6b
{
    const T sum;
    const T a, b, c;

    constexpr Topic6b(T v0, T v1, T v2, T v3)
        : sum {v0}
        , a {v1}, b {v2}, c {v3}
    {}
};

void topic6b()
{
    constexpr Topic6b<int> b {1, 2, 3, 4};
    static_assert(1 == b.sum);
}
```

- If this stage produces an explicit constructor as the best match for a copy-list-initialization, compilation fails (note, in simple copy-initialization, explicit constructors are not considered at all).

7. Otherwise, if `T` is an enumeration type ...

8. Otherwise (if T is not a class type)


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
