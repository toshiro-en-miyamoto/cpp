# Chapter 1 Safe Features

In short, widespread adoption of safe features is a low-risk proposition. All of these features are easy to understand and use profitably and hard to misuse; hence, formal training is generally not required. An organization need not be concerned about incorporating safe features into a predominantly C++03 codebase maintained primarily by those largely unfamiliar with modern features. If you’re new to the features of modern C++, by all means start here.

# C++11 Attribute Syntax

Generalized Attribute Support:
- A new syntax for annotating code with attributes affords the portable provision of supplementary information for compiler implementations and external tools.

```c++
[[nodiscard]] inline int f();
```

[Standard attributes](https://en.cppreference.com/w/cpp/language/attributes):

| since | standard attribute    | description
|-------|-----------------------|-------------
| C++11 | `noreturn`            | indicates that the function does not return
| C++11 | `carries_dependency`  | indicates that dependency chain in release-consume `std::memory_order` propagates in and out of the function
| C++14 | `deprecated`          | indicates that the use of the name or entity declared with this attribute is allowed, but discouraged for some reason (`deprecated("reason")`)
| C++17 | `fallthrough`         | indicates that the fall through from the previous case label is intentional and should not be diagnosed by a compiler that warns on fall-through
| C++17 | `nodiscard`           | encourages the compiler to issue a warning if the return value is discarded
| C++17 | `maybe_unused`        | suppresses compiler warnings on unused entities, if any
| C++20 | `likely`              | indicates that the compiler should optimize for the case where a path of execution through a statement is more or less likely (`unlikely`) than any other path of execution
| C++20 | `no_unique_address`   | indicates that a non-static data member need not have an address distinct from all other non-static data members of its class
| C++20 | `assume`              | specifies that an expression will always evaluate to `true` at a given point

Use Cases:
- Prompting useful compiler diagnostics
- Hinting at additional optimization opportunities
- Stating explicit assumptions in code to achieve better optimizations
- Using attributes to control external static analysis
- Creating new attributes to express semantic properties

Potential Pitfalls:
- Unrecognized attributes have implementation-defined behavior
- Some attributes, if misused, can affect program correctness

# C++11 Consecutive `>`s

Consecutive Right-Angle Brackets:
- In the context of template argument lists, `>>` is parsed as two separate closing angle brackets.

```c++
// C++03
std::vector<std::vector<int>> v0;   // annoying compile-time error in C++03
std::vector<std::vector<int> > v1;  // OK
// C++11
std::vector<std::vector<int>> v0;               // OK
std::vector<std::vector<std::vector<int>>> v1;  // OK
```

Use Cases:
- Avoiding annoying whitespace when composing template types

Potential Pitfalls:
- Some C++03 programs might stop compiling in C++11
- The meaning of a C++03 program can, in theory, silently change in C++11

# C++11 `decltype`

Operator for Extracting Expression Types:
- The keyword `decltype` enables the compile-time inspection of (a) the declared type of an entity or (b) the type and value category of an expression.

```c++
int i;
decltype(i)   l = i; // (a) int
decltype((i)) m = i; // (b) int&
```

Use Cases:
- Avoiding unnecessary use of explicit `typename`s
- Expressing type-consistency explicitly
- Creating an auxiliary variable of generic type
- Determining the validity of a generic expression

Potential Pitfalls:
- `decltype(x)` and `decltype((x))` will sometimes yield different results

Annoyances:
- Mechanical repetition of expressions might be required

# C++11 Defaulted Functions

Using `= default` for Special Member Functions:
- The keyword `default` annotating a declaration of a special member function instructs the compiler to attempt to generate the function automatically.

```c++
struct S4
{
  S4(const S4&) = default;             // copy constructor
  S4& operator=(const S4&) = default;  // copy-assignment operator

  // has no effect on other four special member functions, i.e.,
  // implicitly generates the default constructor, the destructor,
  // the move constructor, and the move-assignment operator
};
```

Use Cases:
- Restoring the generation of a special member function suppressed by another
- Making class APIs explicit at no runtime cost
- Preserving type triviality
- Physically decoupling the interface from the implementation

Potential Pitfalls:
- Defaulted special member functions cannot restore trivial copy-ability (`std::is_trivially_copyable`)

Annoyances:
- Generation of defaulted functions is not guaranteed

# C++11 Delegating Ctors

Constructors Calling Other Constructors:
- The use of the name of the class in the initializer list of that class’s constructor enables delegating initialization to another constructor of the same class.

```c++
struct S0 {
  int d_i;

  S0(int i) : d_i(i) {}   // non-delegating
  S0()      : S0(0)  {}   // delegates to S0(int i)
};
```

Use Cases:
- Avoiding code duplication among constructors

Potential Pitfalls:
- Delegation cycles
- Suboptimal factoring

# C++11 Deleted Functions

Using `= delete` for Arbitrary Functions:
- The keyword `delete` annotating a function’s first declaration makes any attempt to use or even access it ill formed.

```c++
void g(double) { }
void g(int) = delete;
```

Use Cases:
- Suppressing special member function generation
- Preventing a particular implicit conversion
- Preventing all implicit conversions
- Hiding a structural, non-polymorphic base class’s member function

Annoyances:
- Deleting a function declares it

# C++11 `explicit` Operators

Explicit Conversion Operators:
- Ensure that a user-defined type is convertible to another type only in contexts where the conversion is made obvious in the code.

As of C++11, we can now use the `explicit` specifier when declaring conversion operators as well as converting constructors, thereby forcing the client to request conversion explicitly, e.g., using direct initialization or `static_cast`:

```c++
struct S0 { explicit operator int(); };

void g()
{
  S0 s0;
  int i = s0;                    // Error, copy initialization
  int k(s0);                     // OK, direct initialization
  double d = s0;                 // Error, copy initialization
  int j = static_cast<int>(s0);  // OK, static cast
  if (s0) { }                    // Error, contextual conversion to bool
  double e(s0);                  // Error, direct initialization
}
```

Use Cases:
- Enabling contextual conversions to `bool` as a test for validity

Potential Pitfalls:
- Sometimes implicit conversion is indicated
- Sometimes a named function is better

# C++11 Function `static`

Thread-Safe Function-Scope `static` Variables:
- Initialization of function-scope `static` objects is now guaranteed to be free of data races in the presence of multiple concurrent threads.

```c++
int f(int i)
{
  static int iLocal = 1;  // Object is initialized only once, on the first call.
  return iLocal;          // The same iLocal value is returned on every call.
}
```

Use Cases:
- Meyers Singleton

Potential Pitfalls:
- *static storage duration* objects are not guaranteed to be initialized
- Dangerous recursive initialization
- Subtleties with recursion
- Depending on order-of-destruction of local objects after `main` returns

Annoyances:
- Overhead in single-threaded applications

# C++11 Local Types

Local/Unnamed Types as Template Arguments:
- C++11 allows function-scope and unnamed types to be used as template arguments.
- Historically, types without linkage, i.e., local and unnamed types, were forbidden as template arguments.

```c++
template <typename T>
void f(T) {}              // function template

struct {} obj;            // object obj of unnamed C++ type

void g()
{
  struct S {};            // local type

  f(S());
  f(obj);
}
```

Use Cases:
- Encapsulating a type within a function
- Instantiating templates with local function objects as type arguments
- Configuring algorithms via lambda expressions

# C++11 `long long`

The `long long` (≥64 bits) Integral Type:
- A new fundamental integral type, `long long` is guaranteed to have at least 64 bits on all platforms.

```c++
#include <climits>
static_assert(64 <= CHAR_BIT * sizeof(long long));
```

Use Cases:
- Storing values that won’t safely fit in 32 bits

Potential Pitfalls:
- Relying on the relative sizes of `int`, `long`, and `long long`

# C++11 `noreturn`

The `[[noreturn]]` Attribute:
- The standard attribute `[[noreturn]]` indicates that the function to which it pertains does not return normally.

```c++
[[noreturn]] void f()
{
  throw 1;
}
```

Use Cases:
- Better compiler diagnostics
- Improved runtime performance

Potential Pitfalls:
- `[[noreturn]]` can inadvertently break an otherwise working program
- Misuse of `[[noreturn]]` on function pointers

# C++11 `nullptr`

The Null-Pointer-Literal Keyword:
- The `nullptr` literal, unlike `0` or `NULL`, unambiguously denotes a null-address value.

```c++
#include <cstddef>          // std::nullptr_t
void g(int){}               // (1)
void g(std::nullptr_t){}    // (2)

int main()
{
  g(0);                     // OK, (1)
  g(nullptr);               // OK, (2)
  g(NULL);                  // error, ambiguous (1) or (2)
} 
```

Use Cases:
- Improvement of type safety
- Disambiguation of `(int)0` vs. `(T*)0` during overload resolution
- Overloading for a literal null pointer

# C++11 `override`

The `override` Member-Function Specifier:
- Decorating a function in a derived class with the contextual keyword `override` ensures that a `virtual` function having a compatible declaration exists in one or more of its base classes.

```c++
struct Base
{
  virtual void f(int);
};

struct Derived : Base
{
  void f()    override;   // error, Base::f() not found
  void f(int) override;   // OK, explicitly overrides
};
```

Use Cases:
- Ensuring that a member function of a base class is being overridden

Potential Pitfalls:
- Lack of consistency across a codebase

# C++11 Raw String Literals

Syntax for Unprocessed String Contents:
- Supplying a leading `R` adjacent to a double-quoted string having augmented delimiters — typically `"(` and `)"` — disables interpretation of special characters, obviating escaping each individually.

```c++
const char* productIdRegex = R"([0-9]{5}\(".*"\))";
```

Use Cases:
- Embedding code in a C++ program

Potential Pitfalls:
- Unexpected indentation
- Encoding of new lines and whitespace

# C++11 `static_assert`

Compile-Time Assertions:
- This compile-time analog to the classic runtime `assert` causes compilation to terminate with a user-supplied error message whenever its constant-expression argument evaluates to `false`.

```c++
#include <climits>      // CHAR_BIT
#include <type_traits>  // std::is_integral, std::is_unsigned

template <typename T>
T rotateLeft(T x)
{
  static_assert(std::is_integral<T>::value, "T must be an integral type.");
  static_assert(std::is_unsigned<T>::value, "T must be an unsigned type.");

  return (x << 1) | (x >> (sizeof(T) * CHAR_BIT - 1));
}
```

Use Cases:
- Verifying assumptions about the target platform
- Preventing misuse of class and function templates

Potential Pitfalls:
- Static assertions in templates can trigger unintended compilation failures
- Misuse of static assertions to restrict overload sets

Annoyances:
- Mandatory string literal

# C++11 Trailing Return

Trailing Function Return Types:
- This syntactically more convenient yet semantically equivalent alternative of using `->` to declare a function’s return type after its parameter list enables that type to refer to the individual parameters by name along with other class or namespace members without explicit qualification.

When using the alternative,
- trailing-return-type syntax, any `const`, `volatile`, and reference qualifiers (see Section 3.1.“Ref-Qualifiers”) are placed to the left of the `-> <return-type>`, and
- any contextual keywords, such as `override` and `final` are placed to its right:

```c++
struct Base
{
  virtual int e() const;     // const qualifier
  virtual int g() &;         // lvalue-reference qualifier
  virtual int h() &&;        // rvalue-reference qualifier
};

struct Derived : Base
{
  auto e() const -> int override;  // override contextual keyword
  auto g() &     -> int override;  // override      "        "
  auto h() &&    -> int final;     // final         "        "
};
```

Use Cases:
- Function template whose return type depends on a parameter type
- Avoiding having to qualify names redundantly in return types
- Improving readability of declarations involving function pointers

# C++11 Unicode Literals

Unicode String Literals:
- C++11 introduces a portable mechanism for ensuring that a literal is encoded as UTF-8, UTF-16, or UTF-32.
- C++20 fundamentally changes how UTF-8 string literals work, by introducing a new non-aliasing `char8_t` character type whose representation is guaranteed to match `unsigned char`.

| encoding  | syntax      | type
|-----------|-------------|--------
| UTF-8     | `u8"Hello"` | `char` (`char8_t` since C++20)
| UTF-16    | `u"Hello"`  | `char16_t`
| UTF-32    | `U"Hello"`  | `char32_t`

Use Cases:
- Guaranteed-portable encodings of literals

Potential Pitfalls:
- Embedding Unicode graphemes
- Lack of library support for Unicode
- Problematic treatment of UTF-8 in the type system
- C++20 `char8_t`

# C++11 `using` Aliases

Type/Template Aliases (Extended `typedef`):
- The `using` keyword may now be used to introduce type aliases and alias templates, providing a more general alternative to `typedef` that might also improve readability, especially for function aliases.

```c++
using Type1 = int;            // typedef int Type1;
using Type2 = double;         // typedef double Type2;
using Type3 = void(*)();      // typedef void(*Type3)();

struct S { int i; void f(); };

using Type4 = void(S::*)();   // typedef void(S::*Type4)();
using Type5 = int S::*;       // typedef int S::*Type5;
```

Use Cases:
- Simplifying convoluted `typedef` declarations
- Binding arguments to template parameters
- Providing a shorthand notation for type traits

## Providing a shorthand notation for type traits

As an example, consider a simple type trait that adds a pointer to a given type:

```c++
template <typename T>
struct add_pointer
{
  typedef T* type;
};
```

To use the trait above, the `add_pointer` class template must be instantiated, and its nested `type` alias must be accessed. Furthermore, in the generic context, it has to be prepended with the `typename` keyword:

```c++
template <typename T>
void f()
{
  T t;
  typename add_pointer<T>::type p = &t;
}
```

The syntactical overhead of `add_pointer` can be removed by creating an alias template for its nested type alias, such as `add_pointer_t`:

```c++
template <typename T>
using add_pointer_t = typename add_pointer<T>::type;
```

Using `add_pointer_t` instead of `add_pointer` results in shorter code devoid of boilerplate:

```c++
void g()
{
  int i;
  add_pointer_t<int> p = &i;
}
```

# C++14 Aggregate Init

Aggregates Having Default Member Initializers:
- C++14 enables the use of *aggregate initialization* with classes employing default member initializers.
- Note that since C++11, direct list initialization can be used to perform aggregate initialization; see Section 2.1.“Braced Init”.
- Prior to C++14, classes that used default member initializers (see Section 2.1.“Default Member Init”), were not considered *aggregate types*. Because `A` but not `S` is considered an aggregate in C++11, instances of `A` can be created via aggregate initialization, whereas instances of `S` cannot.

```c++
// aggregate type in C++11 and C++14
struct A
{
  int  i;
  bool b;
};

// aggregate type in C++14 but not C++11
struct S
{
  int  i;
  bool b = false;
};

A a = { 100, true };  // OK, in both C++11 and C++14
S s = { 100, true };  // Error in C++11; OK in C++14
```


Use Cases:
- Configuration `struct`s

Potential Pitfalls:
- Empty list-initializing members do not use the default initializer

Annoyances:
- Syntactical ambiguity in the presence of brace elision

# C++14 Binary Literals

Binary Literals: The `0b` Prefix:
- The `0b` (or `0B`) prefix, modeled after `0x`, enables integer literals to be expressed in base 2.

```c++
int i = 0b11110000;  // equivalent to 240, 0360, or 0xF0
```

Use Cases:
- Bit masking and bitwise operations
- Replicating constant binary data

# C++14 `deprecated`

The `[[deprecated]]` Attribute:
- The standard `[[deprecated]]` attribute is used to portably indicate that a particular entity is no longer recommended and to actively discourage its use.

Use Cases:
- Discouraging use of an obsolete or unsafe entity

Potential Pitfalls:
- Interaction with treating warnings as errors

# C++14 Digit Separators

The Digit Separator (`'`):
- A digit separator is a single-character token (`'`) that can appear as part of a numeric literal without altering its value.

```c++
unsigned int u = 1'000'000u;
long double  e = 20'812.80745'23204;
int        hex = 0x8C'25'00'F9;
int        bin = 0b1001'0110'1010'0111;
```

Use Cases:
- Grouping digits together in large constants

# C++14 Variable Templates

Templated Variable Declarations/Definitions:
- Traditional template syntax is extended to define, in namespace or class (but not function) scope, a family of like-named variables that can be instantiated explicitly.

```c++
#include <cassert>

template <typename T>
const T pi(3.1415926535897932385);

int main()
{
  const float  PI_FLOAT  = 3.1415927;
  const double PI_DOUBLE = 3.141592653589793;
  assert(pi<float>  == PI_FLOAT);
  assert(pi<double> == PI_DOUBLE);
}
```

Note that while variable templates do not add new functionality, they significantly reduce the boilerplate associated with achieving the same goals without them.

```c++
// C++03 (obsolete)
#include <cassert>

template <typename T>
struct Pi {
  static const T value;
};

template <typename T>
const T Pi<T>::value(3.1415926535897932385);

int main()
{
  const float  PI_FLOAT  = 3.1415927;
  const double PI_DOUBLE = 3.141592653589793;
  assert(Pi<float>::value  == PI_FLOAT);
  assert(Pi<double>::value == PI_DOUBLE);
}
```

Use Cases:
- Parameterized constants
- Reducing verbosity of type traits

Potential Pitfalls:
- Recursive variable template initializations require `const` or `constexpr`

Annoyances:
- Variable templates do not support template template parameters

## Reducing verbosity of type traits

As of C++17, the Standard Library provides a more convenient way of inspecting the result of a type trait, by introducing variable templates named the same way as the corresponding traits but with an additional `_v` suffix:

```c++
// C++11/14
bool dc1 = std::is_default_constructible<T>::value;

// C++17
bool dc2 = std::is_default_constructible_v<T>;
```
