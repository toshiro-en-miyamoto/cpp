# Declarations and definitions

References:

- [Declarations, C++ reference](https://www.cppreference.com/w/cpp/language/declarations.html)
- [Definitions and ODR, C++ reference](https://www.cppreference.com/w/cpp/language/definition.html)

Declarations are how names are introduced (or re-introduced) into the C++ program.

Definitions are declarations that fully define the entity introduced by the declaration. Every declaration is a definition, except for the following:

- A function declaration without a function body:
  ```c++
  int f(int);   // declares, but does not define f
  ```
- A parameter declaration in a function declaration that isn't a definition:
  ```c++
  int f(int x); // declares, but does not define f and x
  int f(int x)  // defines f and x
  {
    return x + a;
  }
  ```
- Any declaration with an extern storage class specifier without an initializer:
  ```c++
  extern const int a;     // declares, but does not define a
  extern const int b = 1; // defines b
  ```
- Declaration of a non-inline static data member inside a class definition:
  ```c++
  struct S
  {
    int n;               // defines S::n
    static int i;        // declares, but does not define S::i
    inline static int x; // defines S::x
  };                     // defines S
  int S::i;              // defines S::i
  ```
- Declaration of a class name by forward declaration:
  ```c++
  struct S;             // declares, but does not define S
  ```
- Declaration of a template parameter:
  ```c++
  template<typename T> // declares, but does not define T
  ```
- An explicit instantiation declaration (an "extern template"):
  ```c++
  extern template
  f<int, char>; // declares, but does not define f<int, char>
  ```
- An explicit specialization whose declaration is not a definition:
  ```c++
  template<>
  struct A<int>; // declares, but does not define A<int>
  ```
- A type alias declaration:
  ```c++
  using S2 = S; // declares, but does not define S2
                // S may be incomplete
  ```
- A `using`-directive:
  ```c++
  namespace vec
  {
    template<typename T>
    class vector      // defines vec::vector
    {
        // ...
    };
  }
  using vec::vector;  // declares, but does not define vec::vector
  ```

## One definition rule (ODR)

Only one definition of any variable, function, class type, enumeration type, concept(since C++20) or template is allowed in any one translation unit. Some of these may have multiple declarations, but only one definition is allowed.

One and only one definition of every non-inline function or variable that is odr-used (see below) is required to appear in the entire program (including any standard and user-defined libraries).

*odr-used* is informally defines as follows:

- An object is odr-used if its value is read (unless it is a compile time constant) or written, its address is taken, or a reference is bound to it.
- A reference is odr-used if it is used and its referent is not known at compile time.
- A function is odr-used if a function call to it is made or its address is taken.

If an entity is odr-used, its definition must exist somewhere in the program; a violation of that is usually a link-time error.
