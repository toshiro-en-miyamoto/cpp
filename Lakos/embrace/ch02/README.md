# Chapter 2 Conditionally Safe Features

# C++11 `alignas`

The `alignas` Specifier:
- The keyword `alignas` can be used in the declaration of a class type, a data member, an enumeration, or a variable to strengthen its alignment.
- Each object type in C++ has an alignment requirement that restricts the addresses at which an object of that type is permitted to reside within the virtual-memory-address space. The alignment requirement is imposed by the object type on all objects of that type. The `alignas` specifier provides a means of specifying a stricter alignment requirement than dictated by the type itself for a particular variable of the type or an individual data member of a user-defined type (UDT).

```c++
alignas(8) int i;      // i is aligned on an 8-byte address boundary.
```

Use Cases:
- Creating a sufficiently aligned object buffer
- Ensuring proper alignment for architecture-specific instructions
- Avoiding false sharing among distinct objects in a multithreaded program
- Avoiding false sharing within a single-thread-aware object

Potential Pitfalls:
- Under-specifying alignment is not universally reported
- Incompatibly specifying alignment is *ill formed, no diagnostic required* (IFNDR)
- Applying `alignas` to a type might be misleading
- Overlooking alternative approaches to avoid false sharing

# C++11 `alignof`

The `alignof` Operator:
- The keyword `alignof` serves as a compile-time operator used to query the alignment requirements of a type on the current platform.
- The `alignof` operator, when applied to a type, evaluates to an integral constant expression that represents the alignment requirement of its argument type. Similar to `sizeof`, the compile-time value of alignof is of type `std::size_t`.

```c++
static_assert(alignof(char)   == 1, "");  // guaranteed to be 1
static_assert(alignof(short)  == 2, "");  // platform-dependent
static_assert(alignof(int)    == 4, "");  //     "        "
static_assert(alignof(double) == 8, "");  //     "        "
static_assert(alignof(void*)  >= 4, "");  //     "        "

struct S0 { };                            // sizeof(S0) is  1; alignof(S0) is  1
struct S1 { char c; };                    // sizeof(S1) is  1; alignof(S1) is  1
struct S2 { short s; };                   // sizeof(S2) is  2; alignof(S2) is  2
struct S3 { char c; short s; };           // sizeof(S3) is  4; alignof(S3) is  2
struct S4 { short s1; short s2; };        // sizeof(S4) is  4; alignof(S4) is  2
struct S5 { int i; char c; };             // sizeof(S5) is  8; alignof(S5) is  4
struct S6 { char c1; int i; char c2; };   // sizeof(S6) is 12; alignof(S6) is  4
struct S7 { char c; short s; int i; };    // sizeof(S7) is  8; alignof(S7) is  4
struct S8 { double d; };                  // sizeof(S8) is  8; alignof(S8) is  8
struct S9 { double d; char c; };          // sizeof(S9) is 16; alignof(S9) is  8
struct SA { long double ld; };            // sizeof(SA) is 16; alignof(SA) is 16
struct SB { long double ld; char c; };    // sizeof(SB) is 32; alignof(SB) is 16
```

Use Cases:
- Probing the alignment of a type during development
- Determining if a given buffer is sufficiently aligned
- Monotonic memory allocation

Annoyances:
- `alignof` is defined only on types

# C++11 `auto` Variables

Variables of Automatically Deduced Type:
- The keyword `auto` was repurposed in C++11 to act as a placeholder type such that, when used in place of a type as part of a variable declaration, the compiler will deduce the variable’s type from its initializer.
  ```c++
  auto pi = 3.14f;  // Type of pi is deduced to be float.
  ```
- Just as how function template argument deduction never deduces a reference type for its by-value argument, a variable declared with an unqualified auto is never deduced to have a reference type.
  ```c++
  int  val = 3;
  int& ref = val;
  auto tmp = ref;   // Type of tmp is deduced to be int, not int&.
  ```
- Augmenting `auto` with reference qualifiers and cv-qualifiers, however, enables us to control whether the deduced type is a reference and whether it is `const`
  ```c++
  auto val = 3;             // int
  const auto cval = val;    // const int
  auto& ref = val;          // int&
  auto& cref1 = cval;       // const int&
  ```

Use Cases:
- Ensuring variable initialization
- Avoiding redundant type name repetition
- Preventing unexpected implicit conversions
- Declaring variables of implementation-defined or compiler-synthesized types
- Declaring variables of complex and deeply nested types
- Improving resilience to library code changes

Potential Pitfalls:
- Compromised readability
- Unintentional copies
- Unexpected conversions and the lack of expected ones
- Lack of interface restrictions
- Important properties of fundamental types might be hidden
- Surprising deduction for list initialization
- Deducing built-in arrays is problematic

Annoyances:
- `auto` is disallowed for non-static data members
- Not all template argument deduction constructs are allowed for `auto`

# C++11 Braced Init

Braced-Initialization Syntax: `{}`:
- Braced initialization, a generalization of C++03 initialization syntax, was designed with the intention of being used safely and uniformly in any initialization context.

```c++
int ai[] =
{         //     C++03   C++11
  5,    // (0) OK      OK
  5.0,  // (1) OK      Error, narrowing double to int conversion is not allowed.
  5.5,  // (2) OK      Error, narrowing double to int conversion is not allowed.
  "5",  // (3) Error   Error, no const char* to int conversion exists.
};
```

Use Cases:
- Defining a value-initialized variable
- Avoiding the most vexing parse
- Uniform initialization in generic code
- Uniform initialization in factory functions
- Uniform member initialization in generic code

Potential Pitfalls:
- Inadvertently calling initializer-list constructors
- Implicit move and named return value optimization might be disabled in `return` statements
- Surprising behavior of aggregates having deleted constructors

Annoyances
- Narrowing aggregate initialization might break C++03 code
- No easy way to allow narrowing conversions
- Breaks macro-invocation syntax
- No copy list initialization in member initializer lists
- Accidental meaning for explicit constructors passed multiple arguments
- Obfuscation due to opaque use of braced list initializers
- `auto` deduction and braced initialization
- Compound assignment but not arithmetic operators accept braced lists

# C++11 `constexpr` Functions

Compile-Time Invocable Functions:
- Functions decorated with `constexpr` are eligible to be invoked as part of a *constant expression*.

```c++
constexpr int factorial(int n)  // can be evaluated in a constant expression
{
  return n == 0 ? 1 : n * factorial(n - 1);
}

static_assert(factorial(5) == 120, "");
        // OK, factorial(5) is evaluated at compile time since arguments of
        // static_assert must be constant expressions.
```

Use Cases:
- A better alternative to function-like macros
- Compile-time string traversal
- Precomputing tables of data at compile time

Potential Pitfalls:
- Low compiler limits on compile-time evaluation
- Difficulty implementing `constexpr` functions
- Prematurely committing to `constexpr`
- Overzealous use
- One time is cheaper than compile time or run time

Annoyances:
- Penalizing run time to enable compile time
- `constexpr` member functions are implicitly `const`-qualified (C++11 only)

# C++11 `constexpr` Variables

Compile-Time Accessible Variables:
- A variable or variable template of literal type can be declared to be `constexpr`, ensuring it is initialized and can be used at compile time.
- Variables of all built-in types and certain user-defined types, collectively known as *literal types*, can be declared `constexpr`, allowing them to be initialized at compile-time and subsequently used in constant expressions.
  ```c++
  struct S  // simple (aggregate) literal type
  {
    int i;  // built-in integer data member
  };

  void test1()
  {
    constexpr S s{1};  // OK, literal type constant expression initialized
    s = S();           // Error, constexpr implies const.
    static_assert(s.i == 1, "");  // OK, sub-objects of constexpr objects are
  }
  ```

Use Cases:
- Alternative to enumerated compile-time integral constants
- Non-integral symbolic numeric constants
- Storing `constexpr` data structures
- Diagnosing undefined behavior at compile time

Potential Pitfalls:
- `constexpr` function pitfalls

Annoyances:
- `static` member variables require external definitions
- No static `constexpr` member variables defined in their own class

# C++11 Default Member Init

Default `class/union` Member Initializers:
- Non-static class data members might specify a default initializer, which is used for constructors that don’t initialize the member explicitly.

```c++
struct S2
{
  int d_i = 1;
  int d_j = 1;

  S2() { }                           // Initialize d_i with 1, d_j with 1.
  S2(int) : d_i(2) { }               // Initialize d_i with 2, d_j with 1.
  S2(int, int) : d_i(2), d_j(3) { }  // Initialize d_i with 2, d_j with 3.
};
```

Use Cases:
- Concise initialization of simple `struct`s
- Ensuring initialization of a non `static` data member
- Avoiding boilerplate repetition across multiple constructors
- Making default values obvious for documentation purposes

Potential Pitfalls:
- Loss of insulation
- Inconsistent sub-object initialization

Annoyances:
- Parenthesized direct-initialization syntax cannot be used
- Limitations of applicability
- Loss of triviality
- Loss of aggregate status
- Default member initializer does not deduce array size

# C++11 `enum class`

Strongly Typed, Scoped Enumerations:
- An `enum class` is an alternative enumeration type that provides simultaneously (1) an enclosing scope for its enumerators and (2) stronger typing compared to a classic `enum`.

```c++
enum class  Ec { A, B, C };

Ec e0 = A;      // Error, A not found
Ec e1 = Ec::A;  // OK
```

Use Cases:
- Avoiding unintended implicit conversions to arithmetic types
- Avoiding namespace pollution
- Improving overloading disambiguation
- Encapsulating implementation details within the enumerators themselves

Potential Pitfalls:
- Strong typing of an `enum class` can be counterproductive
- Misuse of `enum class` for collections of named constants
- Misuse of `enum class` in association with bit flags
- Misuse of `enum class` in association with iteration
- External use of opaque enumerators

Annoyances:
- Scoped enumerations do not necessarily add value

# C++11 `extern template`

Explicit-Instantiation Declarations:
- The `extern template` prefix can be used to suppress implicit generation of local object code for the definitions of particular specializations of class, function, or variable templates used within a translation unit, with the expectation that any suppressed object-code-level definitions will be provided elsewhere within the program by template definitions that are instantiated explicitly.

```c++
#include <vector>

extern template class std::vector<int>;
    // Suppress depositing of any object code for std::vector<int> into the
    // .o file for this translation unit.
```

Use Cases:
- Reducing template code bloat in object files
- Insulating template definitions from clients

Potential Pitfalls:
- Corresponding explicit-instantiation declarations and definitions
- Accidentally making matters worse

Annoyances:
- No good place to put definitions for unrelated classes
- All members of an explicitly defined template class must be valid

# C++11 Forwarding References

Forwarding References (`T&&`):
- A forwarding reference (`T&&`) — distinguishable from an rvalue reference (`&&`) — is a distinct, special kind of reference that (1) binds universally to the result of an expression of any value category and (2) preserves aspects of that value category so that the bound object can be moved from, if appropriate.

```c++
#include <type_traits>
#include <utility>  // std::move

struct S { /* some type that might benefit from being able to be moved */ };
void g(const S&);   // target function; overload for const S lvalues
void g(S&&);        // target function; overload for S rvalues only

template <typename T>
void f(T&& forRef)      // forRef is a forwarding reference.
{
  if (std::is_lvalue_reference<T>::value)  // using a C++11 type trait
  {
    g(forRef);             // propagates forRef as an lvalue
  }                          // invokes g(const S&)
  else
  {
    g(std::move(forRef));  // propagates forRef as an rvalue
  }                          // invokes g(S&&)
}
```

Use Cases:
- Perfectly forwarding an expression to a downstream consumer
- Handling multiple parameters concisely
- Perfect forwarding for generic factory functions
- Wrapping initialization in a generic factory function
- Emplacement
- Decomposing complex expressions

Potential Pitfalls:
- Surprising number of template instantiations with string literals
- `std::forward<T>` can enable move operations
- A perfect-forwarding constructor can hijack the copy constructor

Annoyances:
- Forwarding references look just like rvalue references
- Meta-functions are required in constraints


# C++11 Generalized PODs

Trivial and Standard-Layout Types:
- The classical notion of POD has been expanded and refined to comprise two important new categories, trivial types (i.e., both trivially default constructible and trivially copyable) and standard-layout types, each of which characterizes objects that can be accessed and manipulated in ways not supported for more general user-defined types.

```c++
#ifdef __cplusplus
extern "C"
{
#endif

  // UDT compatible with both C++ and C
  typedef struct { int x; double y; } PODtype;

  // C function prototype (has C linkage)
  void printPODtype(PODtype* data);

#ifdef __cplusplus
}
#endif
```

Use Cases:
- Vertical encoding within a `union` (POD)
- Vertical encoding for non-trivial types (standard layout)
- Translating a C++-only type to C (standard layout)
- Navigating a compound object using `offsetof` (standard layout)
- Secure buffer (trivially constructible)
- Compile-time constructible, literal types (trivially destructible)
- Skippable destructors (notionally trivially destructible)
- Fixed-capacity string (trivially copyable)
- Elements of fixed-capacity string (trivial)

Potential Pitfalls:
- Exporting bitwise copies of PODs
- Requiring PODs or even just trivial types
- Using the wrong type trait
- Using sloppy terminology
- Using `memcpy` on objects having `const` or reference sub-objects
- Conflating arbitrary values with indeterminate values
- Ineligible use of `std::memcpy`
- Naive copying via other than `std::memcpy`
- Misuse of `union`s
- Abuse of `reinterpret_cast`
- Aggressive use of `offsetof`

Annoyances:
- The C++ Standard has not yet stabilized in this area
- Relevant standard type traits are unreliable
- An `std::pair` or `std::tuple` of PODs is not a POD

# C++11 Inheriting Ctors

Inheriting Base-Class Constructors:
- The term *inheriting constructors* refers to the use of a using declaration to expose nearly all of the constructors of a base class in the scope of a derived class.

```c++
struct B0
{
  B0() = default;
  B0(int)         { }
  B0(int, int)    { }

private:
  B0(const char*) { }
};

struct D0 : B0
{
  using B0::B0;  // using declaration
  D0(double d);  // suppress implicit default constructor
};

D0 t(1);     // OK, inherited from B0::B0(int)
D0 u(2, 3);  // OK, inherited from B0::B0(int, int)
D0 v("hi");  // Error, Base constructor is declared private.
```

Use Cases:
- Avoiding boilerplate code when employing structural inheritance
- Avoiding boilerplate code when employing implementation inheritance
- Implementing a strong `typedef`
- Incorporating reusable functionality via a mix-in

Potential Pitfalls:
- Newly introduced constructors in the base class can silently alter program behavior
- Inheriting *implicit* constructors

Annoyances:
- Inherited constructors cannot be selected individually
- Access levels of inherited constructors are the same as in base class
- Flawed initial specification led to diverging early implementations

# C++11 `initializer_list`

List Initialization `std::initializer_list<T>`:
- The C++ Standard Library’s `std::initializer_list` class template supports lightweight, compiler-generated arrays of non-modifiable values that are initialized in source code similarly to built-in, C-style arrays using the generalized braced initialization syntax.
- Built-in arrays can be initialized via brace-enclosed lists of values

  ```c++
  int data[] = { 0, 1, 1, 2, 3, 5, 8, 13 };
  ```

- C++11 extends this concept to allow such lists of values to be provided to user-defined types (UDTs). The compiler arranges for the values to be stored in an unnamed C-style array of `const` elements and provides access to that array via an object of type `std::initializer_list` instantiated on the element type.

  ```c++
  void test()
  {
    printNumbers({ 1, 2, 3, 4 });
  }

  #include <initializer_list>
  #include <iostream>

  void printNumbers(std::initializer_list<int> il)
  {
    std::cout << "{";
    for (const int* ip = il.begin(); ip != il.end(); ++ip)
    {
      std::cout << ' ' << *ip;
    }
    std::cout << " } [size = " << il.size() << ']';
  }
  ```

Use Cases:
- Convenient population of standard containers
- Providing support for braced lists in user code
- Functions consuming a variable number of arguments of the same type
- Iterating over a fixed number of objects

Potential Pitfalls:
- Dangling references to temporary underlying arrays

Annoyances:
- Initializer lists must sometimes be homogeneous
- `std::initializer_list` constructor suppresses the implicitly declared default
- Initializer lists represent `const` objects
- Overloaded free-function templates `begin` and `end` are largely vestigial

# C++11 Lambdas

Anonymous Function Objects (Closures):
- Lambda expressions provide a means of defining function objects at the point where they are needed, enabling a powerful and convenient way to specify callbacks or local functions.

```c++
void sortBySalary2(std::vector<Employee>& employees)
{
  std::sort(
    employees.begin(),
    employees.end(),
    [](const Employee& e1, const Employee& e2)
    {
      return e1.salary < e2.salary;
    }
  );
}
```

Use Cases:
- Interface adaptation, partial application, and currying
- Emulating local functions
- Emulate user-defined control constructs
- Variables and control constructs in expressions
- Use with `std::function`
- Event-driven callbacks
- Recursion
- Stateless lambdas

Potential Pitfalls:
- Dangling references
- Overuse
- Mixing captured and non-captured variables
- Local variables in unevaluated contexts can yield surprises

Annoyances:
- Debugging
- Can’t capture `*this` by copy
- Confusing mix of immediate and deferred-execution code
- Trailing punctuation

# C++11 `noexcept` Operator

Asking if an Expression Cannot `throw`:
- Using the `noexcept` keyword in its operator form affords a standard programmatic means of querying, at compile time, whether a given expression — typically involving a function call — can be relied upon never to emit a C++ exception.
- C++11 introduces an alternative exception-specification mechanism for arbitrary free functions, member functions, and lambda expressions. Instead of specifying a list of exceptions that may be thrown, whether any exception may be thrown is specified.

  | C++03                                   | C++11 equivalent              |
  |-----------------------------------------|-------------------------------|
  | `void f0();`                            | `void g0();`                  |
  | `void f1() throw();`                    | `void g1() noexcept;`         |
  | `void f2() throw(std::bad_exception);`  | `void g2() noexcept(false);`  |
  | `void f3() throw(int, double);`         | `void g3() noexcept(false);`  |

- As with C++03, the lack of any annotation is the equivalent of saying anything might be thrown (except for destructors, which are `noexcept` by default)

  ```c++
  static_assert(!noexcept(g0()));
  static_assert( noexcept(g1()));
  static_assert(!noexcept(g2()));
  static_assert(!noexcept(g3()));
  ```

Use Cases:
- Appending an element to an `std::vector`
- Enforcing a `noexcept` contract using `static_assert`
- Implementing `std::move_if_noexcept`
- Implementing `std::vector::push_back(T&&)`

Potential Pitfalls:
- Using the `noexcept` operator directly
- The `noexcept` operator doesn’t consider function bodies

Annoyances:
- The noexcept operator is too sensitive for direct use
- The strong exception-safety guarantee itself
- Change in unspecified behavior when an `std::vector` grows
- Destructors, but not move constructors, are `noexcept` by default
- Older compilers invade the bodies of `constexpr` functions
- Exception-specification constraints in class hierarchies

# C++11 Opaque `enum`s

Opaque Enumeration Declarations:
- Any enumeration with a fixed underlying type can be declared without being defined, i.e., declared without its enumerators.

  ```c++
  enum State : int;     // opaque declaration of enumeration (new in C++11)

  class Proctor
  {
    State d_state;      // opaque classical enumerated type (BETTER IDEA)
    ...
  }
  ```

- But we can do even better. First, we will want to nest the enumerated State type within the private section of the proctor to avoid needless namespace pollution. Then, because the numerical values of the enumerators are not relevant, we can more closely model our intent by nesting a more strongly typed `enum class` instead:

  ```c++
  class Proctor
  {
    enum class State;   // forward (nested) declaration of type-safe enumeration
    State d_state;      // opaque (modern) enumerated data type (BEST IDEA)
    ...
  }
  ```

Use Cases:
- Using opaque enumerations within a header file
- Dual-Access: Insulating some external clients from the enumerator list
- Cookie: Insulating all external clients from the enumerator list

Potential Pitfalls:
- Re-declaring an externally defined enumeration locally
- Inciting local enumeration declarations: An attractive nuisance

Annoyances:
- Opaque enumerations are not completely type safe

# C++11 Range `for`

Range-Based `for` Loops
- A more abstract form of `for` loops based on ranges provides a simplified and compact syntax for iterating through every member in a given sequence of objects.
- The C++11 range-based `for` loop (sometimes colloquially referred to as the *foreach* loop) is a more concise loop notation tuned for traversing the elements of a container or other sequential range. A range-based `for` loop works with ranges and elements rather than iterators or indexes:

  ```c++
  void f3(const std::vector<std::string>& vec)
  {
    for (const std::string& s : vec)
    {
      // ...
    }
  }
  ```

Use Cases:
- Iterating over all elements of a container
- Sub-ranges
- Range generators
- Iterating over simple values

Potential Pitfalls:
- Lifetime of temporary objects in the range expression
- Inadvertent copying of elements
- Simple and reference-proxy behaviors can be different

Annoyances:
- No access to the state of the iteration
- Adapters are required for many tasks
- No support for sentinel iterator types
- Only argument-dependent lookup (ADL) lookup

# C++11 Rvalue References

Move Semantics and Rvalue References (`&&`):
- A new form of reference, rvalue reference (`&&`), that complements C++03 lvalue references (`&`), is used via overloading to enable move semantics, a potential optimization in which the operation may safely assume that the internal representation of an object may be repurposed instead of copied.
- To support creating references to objects that are at the end of their lifetimes and to enable modification of such objects, rvalue references were added in C++11. For any type `T`, the type `T&&` is an rvalue reference to `T`.
- The primary distinction between an lvalue reference and an rvalue reference is that
  - non `const` rvalue references can bind to temporary objects:

    ```c++
    int&& rri1 = 7;    // OK, rvalue reference to temporary having value 7
    int&& rri2 = f();  // OK, rvalue reference to temporary returned by f
    ```

  - An rvalue reference will not, however, bind to lvalues:

    ```c++
    int   j;
    int&& rrj1 = j;    // Error, cannot bind rvalue reference to lvalue
    int*  pj   = &j;
    int&& rrj2 = *pj;  // Error, cannot bind rvalue reference to lvalue
    ```

Use Cases:
- Move operations as optimizations of copying
  - Creating a low-level value-semantic type (VST)
  - Creating a high-level VST
  - Creating a generic value-semantic container type
- Move-only types
  - Implementing a move-only type
- Passing around resource-owning objects by value
  - Passing movable objects by value
  - Return by value
- Sink arguments
  - Factories having sink arguments
- Identifying value categories

Potential Pitfalls:
- Sink arguments require copying
- Disabling named return value optimization (NRVO)
- Failure to std::move a named rvalue reference
- Repeatedly calling `std::move` on a named rvalue reference
- Returning `const` rvalues pessimizes performance
- Move operations that throw
- Some moves are equivalent to copies
- Making a non-copyable type movable without just cause
- Implementing a move-only type without employing `std::unique_ptr`
- Inconsistent expectations on moved-from objects
- Requiring owned resources to be valid

Annoyances:
- RVO and NRVO require a declared copy or move constructor
- `std::move` does not move
- Visual similarity to forwarding references
- Standard Library requirements on a moved-from object are overly strict
- Lack of destructive move

# C++11 Underlying Type

Explicit Enumeration Underlying Type:
- The underlying type of an enumeration is the fundamental integral type used to represent its enumerated values, which can be specified explicitly in C++11.

```c++
#include <cstdint>  // std::int8_t

enum Month : std::int8_t  // user-provided underlying type (GOOD IDEA)
{
  e_JAN = 1, e_FEB, e_MAR,
  e_APR    , e_MAY, e_JUN,
  e_JUL    , e_AUG, e_SEP,
  e_OCT    , e_NOV, e_DEC
};

static_assert(sizeof(Month) == 1 && alignof(Month) == 1, "");
```

Use Cases:
- Ensuring a compact representation where enumerator values are salient

Potential Pitfalls:
- External use of opaque enumerators
- Subtleties of integral promotion

# C++11 User-Defined Literals

User-Defined Literal Operators:
- C++11 allows developers to define a new suffix for a numeric, character, or string literal, enabling a convenient lexical representation of the value of a user-defined type (UDT) or even a novel notation for the value of a built-in type.
- C++11 extends the notion of type suffix to include user-definable identifiers with a leading underscore (e.g., `_name`, `_time`, `_temp`):

  ```c++
  class Temperature { /*...*/ };

  // define suffix _F
  Temperature operator""_F(long double degrees) { /*...*/ }
  // define suffix _C
  Temperature operator""_C(long double degrees) { /*...*/ }

  Temperature t1 = 32.0_F;
  Temperature t2 = 0.0_C;
  ```

- New suffixes in the *Standard Library* (starting with C++14) make it easier to write software using standard strings, units of time, and complex numbers. Note that, because these are standard UDL suffixes, their names do not have a leading underscore.

  ```c++
  #include <string>
  using namespace std::literals::string_literals;
  const char*    s1 =   "hello";    // decays to (const char*)
  std::string    s2 =   "hello"s;   // std::string("hello")
  std::string    s3 = u8"hello"s;   // std::string(u8"hello")   C++14
  std::u16string s4 =  u"hello"s;   // std::u16string(u"hello")
  std::u32string s5 =  U"hello"s;   // std::u32string(U"hello")
  std::u8string  s6 = u8"hello"s;   // std::u8string(u8"hello") C++20
  ```

- The `<chrono>` header also defines literal suffixes (in namespace `std::literals::chrono_literals`) having familiar names for time units.

  | literal | helper type | definition |
  |---------|-------------|------------|
  | `operator""h`   | `hours`     | `duration<..., std::ratio<3600>>` |
  | `operator""min` | `minutes`   | `duration<..., std::ratio<60>>`   |
  | `operator""s`   | `seconds`   | `duration<...>`                   |
  | `operator""ms`  | `milliseconds`  | `duration<..., std::milli>`   |
  | `operator""us`  | `microseconds`  | `duration<..., std::micro>`   |
  | `operator""ns`  | `nanoseconds`   | `duration<..., std::nano>`    |

Use Cases:
- Wrapper classes
- User-defined numeric types
- User-defined types having string representations
- Unit conversions and dimensional units
- Test drivers

Potential Pitfalls:
- Overuse
- Preprocessor surprises

Annoyances:
- No conversion from floating-point to integer UDL
- Potential suffix-name collisions
- Confusing raw with string UDL operators
- No UDL operator templates for string literals
- No way to parse a leading `-` or `+`
- Parsing numbers is hard

# C++11 Variadic Templates

Variable-Argument-Count Templates:
- Language-level support for traditional templates is extended to enable both class and function templates to accept arbitrary numbers of template arguments and for function templates to access and process arbitrarily many argument objects of heterogeneous types via a new syntactic construct, the *parameter pack*.

  ```c++
  template <typename... Ts> class C;
  template <typename... Ts> int f1a(Ts...);
  template <typename... Ts> int f1b(Ts&...);
  template <typename... Ts> int f1c(const Ts&...);
  template <typename... Ts> int f1d(Ts* const*...);
  template <typename... Ts> int f1e(Ts&&...);
  template <typename T, typename... Ts> void good(T, Ts...);
  ```

Use Cases:
- Generic variadic functions
- Processing variadic arguments in order
- Object factories
- Hooking function calls
- Tuples
- Variant types
- Advanced traits

Potential Pitfalls:
- Accidental use of C-style ellipsis
- Undiagnosed errors

Annoyances:
- Unusable functions
- Limitations on expansion contexts
- Parameter packs cannot be used unexpanded
- Expansion is rigid and requires verbose support code
- Linear search for everything

# C++14 `constexpr` Functions

Relaxed Restrictions on `constexpr` Functions:
- C++14 lifts restrictions regarding the use of many language features in the body of a `constexpr` function.

```c++
constexpr int fact14(int x)
{
  if (x <= 2) {       // Error in C++11; OK in C++14
    return 1;
  }

  int temp = x - 1;   // Error in C++11; OK in C++14
  return x * fact14(temp);
}
```

Use Cases:
- Non-recursive `constexpr` algorithms
- Optimized meta-programming algorithms

# C++14 Generic Lambdas

Lambdas Having a Templated Call Operator:
- C++14 extends the lambda expression syntax of C++11 to allow a templated definition of the function call operator belonging to the closure type.

  ```c++
  auto identity = [](auto a) { return a; };
  int         a4 = identity(42);       // OK, a4 == 42
  double      a5 = identity(3.14);     // OK, a5 == 3.14
  const char* a6 = identity("hi");     // OK, strcmp(a6, "hi") == 0
  ```

- Generic lambdas accomplish this compile-time polymorphism by defining their function call operator — `operator()` — as a template. Recall that the result of a lambda expression is a closure object, an object of unique type having a function call operator; i.e., the closure type is a unique functor class.

Use Cases:
- Reusable lambda expressions
- Terse, robust lambdas
- Recursive lambdas
- Conditional instantiation

Annoyances:
- Cannot use full power of template-argument deduction
- Difficulty in constraining multiple arguments

# C++14 Lambda Captures

Lambda-Capture Expressions:
- An *init capture* expression enables a lambda to add a data member, initialized with an arbitrary expression, to its closure.
- In C++11, lambda expressions can capture variables in the surrounding scope either by copy or by reference:

  ```c++
  void lambda11()
  {
    int i = 0;
    auto f0 = [i]{ };   // Capture i by copy.
    auto f1 = [&i]{ };  // Capture i by reference.
  }
  ```

- C++14 extends the lambda-introducer syntax to support implicit creation of data members inside a closure using an arbitrary initializer:

  ```c++
  void lambda14()
  {
    // synthesize an int data member, j;
    // copy-initialized with 10.
    auto f2 = [j = 10]{ };

    // synthesize a char data member, c;
    // direct-initialized with 'a'.
    auto f3 = [c{'a'}]{ };

    // Error, u initializer is missing for lambda capture.
    auto f4 = [u]{ };

    // Error, v's type cannot be deduced.
    auto f5 = [v{}]{ };

    int i = 0;

    // OK, capture i by copy as j.
    auto f6 = [j = i]{ };

    // OK, capture i by reference as ir.
    auto f7 = [&ir = i]{ };
  }
  ```

Use Cases:
- Moving (as opposed to copying) objects into a closure
- Providing mutable state for a closure
- Capturing a modifiable copy of an existing `const` variable

Potential Pitfalls:
- Forwarding an existing variable into a closure always results in an object

Annoyances:
- There’s no easy way to synthesize a `const` data member
- `std::function` supports only copyable callable objects
