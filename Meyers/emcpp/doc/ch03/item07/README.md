# [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)

A constructor that is not declared with the specifier `explicit` is called a *converting constructor*.

```c++
struct A
{
    A() {}              // converting constructor
    A(int) {}           // converting constructor
    A(int, int) {}      // converting constructor
};

struct B
{
    explicit B() {}             // explicit constructor
    explicit B(int) {}          // explicit constructor
    explicit B(int, int) {}     // explicit constructor
};

int main()
{
    A a1 = 1;       // A::A(int), copy-initialization
    A a2(2);        // A::A(int), direct-initialization
    A a3 {4, 5};    // A::A(int, int), direct-list-init
    A a4 = {4, 5};  // A::A(int, int), copy-list-init

//  B b1 = 1;       // explicit-ctor, not for copy-init
    B b2(2);        // B::B(int), direct-initialization
    B b3 {4,5};     // B::B(int, int), direct-list-init
//  B b4 = {4, 5};  // explicit-ctor, not for copy-list-init
}
```

It is said that a converting constructor specifies an implicit conversion from the types of its arguments (if any) to the type of its class.

Converting constructors include:

- implicitly-declared copy constructors
- implicitly-declared move constructors
- user-defined non-explicit copy constructors
- user-defined non-explicit move constructors

# [Initialization](https://en.cppreference.com/w/cpp/language/initialization)

*Initialization* of a variable provides its nitial value at the time of construction.

- An object is constructed:
  - with no initializer (default initialization)
  - with an empty initializer (value initialization)
- An object is initialized:
  - from explicit set of constructor arguments (direct initialization)
  - from another object (copy initialization)
  - from *braced-init-list* (list initialization)
- An aggregate is initialized:
  - from an initializer list (aggregate initialization)
- Reference initialization binds a reference to an object.

| initialization           | such as
|--------------------------|----------
| default initialization   | `std::string s;`
| value initialization     | `std::string s {};`
| direct initialization    | `std::string s {"ab"};`
| copy initialization      | `std::string s = "ab";`
| list initialization      | `std::string s {'a', 'b'};`
| aggregate initialization | `char a[3] {'a', 'b'};`
| referene initialization  | `char& c = a[0];`

The initial value may be provided in the initializer section of a declarator or a `new` expression. It also takes place during function calls: function parameters and the function return values are also initialized.

| Initializer
|--------
| `(` expression-list `)`
| `=` expression
| `{` initializer-list `}`

## Default initialization
## Value initialization
## Direct initialization 
## Copy initialization

Copy initialization initializes an object from another object.

| Copy initialization
|---------------------
| T object `=` other;
| T object `={`other`}`; (*1)
| f`(`other`)`
| `return` other;
| `throw` object;
| `catch (`object`)`;
| T array[N] `= {`other-sequence`}`;

(*1) As of C++11, this is classified as list-initialization, and narrowing conversion is not allowed.

1.  First, if `T` is a class type and the initializer is a prvalue expression whose cv-unqualified type is the same class as `T`, the initializer expression itself, rather than a temporary materialized from it, is used to initialize the destination object: see copy elision

2.  If `T` is a class type and the cv-unqualified version of the type of `other` is `T` or a class derived from `T`, the *non-explicit constructors* of `T` are examined and the best match is selected by overload resolution. The constructor is then called to initialize the object.

3.  User-defined conversion sequences that can convert from the type of `other` to `T` are examined and the best one is selected through overload resolution

    - if `T` is a class type and the cv-qualified version of the type of `other` is not `T` or derived rom `T`, or
    - if `T` is non-class type but the type of `other` is a class type

    The result of conversion, which is a prvalue expression of the cv-qualified version of `T` if a converting constructor was used, is then used to direct-initialization.

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

1.  If `T` is an aggregate class and the braced-init-list has a single element of the same or derived type (possibly cv-qualified), the object is initialized from that element (by copy-initialization for copy-list-initialization, or by direct-initialization for direct-list-initialization).

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

        constexpr Topic1 ic2 {ic1};     // direct-initalized
        static_assert(ic2.n == 1);

        constexpr Topic1 ic3 = {ic1};   // copy-initialized
        static_assert(ic3.n == 1);
    }
    ```

2.  Otherwise, if `T` is a character array and the braced-init-list has a single element that is an appropriately-typed string literal, the array is initialized from the string literal as usual.

    ```c++
    void topic2()
    {
        constexpr char a[] {"abc"};
        static_assert(a[2] == 'c');
    }
    ```

3.  Otherwise, if T is an aggregate type, aggregate initialization is performed.

4.  Otherwise, if the braced-init-list is empty and `T` is a class type (`class`, `struct`, or `union`) with a default constructor, value-initialization is performed.

    ```c++
    struct Topic4
    {
        constexpr Topic4() : n {4}, c {'b'}
        {}

        int n;
        char c;
    };

    void topic4()
    {
        constexpr Topic4 a {};      // value-initialized
        static_assert(a.n == 4);
    }
    ```

5.  Otherwise, if `T` is a specialization of `std::initializer_list`, the `T` object is direct-initialized or copy-initialized, depending on context, from a prvalue of the same type initialized from (until C++17) the braced-init-list.

6.  Otherwise, the constructors of `T` are considered, in two phases:

    - All constructors that take `std::initializer_list` as the only argument, or as the first argument if the remaining arguments have default values, are examined, and matched by overload resolution against a single argument of `type std::initializer_list`

    - If the previous stage does not produce a match, all constructors of `T` participate in overload resolution against the set of arguments that consists of the elements of the braced-init-list, with the restriction that only non-narrowing conversions are allowed.

    - If this stage produces an **explicit constructor** as the best match for a copy-list-initialization, compilation fails (note, in simple copy-initialization, explicit constructors are not considered at all).

    That is,

    - aggregates copy/move initialize directly from single-element braced-init-list of the same type, but
    - non-aggregates consider `initializer_list` constructors **first**:

    ```c++
    struct X {}; // aggregate

    struct Q     // non-aggregate
    {
        Q() = default;
        Q(Q const&) = default;
        Q(std::initializer_list<Q>) {}
    };

    int main()
    {
        X x;
        X x2 = X{x}; // copy-ctor (not aggregate initialization)

        Q q;
        Q q2 = Q{q}; // initializer-list ctor (not copy ctor)
    }
    ```

7.  Otherwise, if `T` is an enumeration type ...

8.  Otherwise (if `T` is not a class type), if the braced-init-list has only one element and either
    - `T` is not a reference type or
    - `T` is a reference type whose referenced type
      - is same as the type of the element or 
      - is a base class of the type of the element,
   
    `T` is direct-initialized (in direct-list-initialization) or copy-initialized (in copy-list-initialization), except that narrowing conversions are not allowed.

    ```c++
    int n1 {1};     // direct-list-initialization
    int n2 = {1};   // copy-list-initialization
    ```

9.  Otherwise, if `T` is a reference type that is not compatible with the type of the element: a prvalue is generated.
    - The prvalue initializes its result object by copy-list-initialization.
    - The prvalue is then used to direct-initialize the reference (this fails if the reference is a non-const lvalue reference).

    The type of the temporary is the type referenced by `T`.

    ```c++
    int&& r1 = {1};  // binds a rvalue reference to a temporary int
    // int& r2 = {2}; error: cannot bind rvalue to a non-const lval ref
    ```

10. Otherwise, if the braced-init-list has no elements, `T` is value-initialized.

    ```c++
    int n0 {};      // value-initialization to zero
    ```

## Aggregate initialization

Aggregate initialization initializes an *aggregate* from an initializer list. It is a form of list-initialization.

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

| declared   | inside f()
|------------|--------------------
| T`&` f();  | `return` target;
| T`&&` f(); | `return` target;

A non-static data member of lvalue or rvalue reference type is initialized using a member initializer of constructor:

| in a class | constructor definition
|------------|--------------------------
| T`&` ref;  | class::class(...) : ref`(`target`)` {...}
| T`&&` ref; | class::class(...) : ref`(`target`)` {...}

> `: ref{target}` ?
