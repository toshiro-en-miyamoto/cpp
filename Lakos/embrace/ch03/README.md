# Chapter 3 Unsafe Features

Although most of the features presented in this chapter have the potential to add value, all come with a profoundly high risk of being misused or a disproportionately high training cost — not only for implementors, but for maintainers too — and thus are considered unsafe.

# C++11 `carries_dependency`

The `[[carries_dependency]]` Attribute:
- Associating the `[[carries_dependency]]` attribute with individual parameters or the return value of a function provides a means to manually identify them as components of *data dependency chains* to enable, primarily across translation units, use of the lighter-weight *release-consume synchronization paradigm* as an optimization over the more conservative *release-acquire paradigm*.

Use Cases:
- Producer-consumer programming pattern

Potential Pitfalls:
- No practical use on current platforms

# C++11 `final`

Prohibiting Overriding and Derivation:
- The `final` specifier can be used to disallow either (1) overriding one or more virtual member functions from within derived types or (2) deriving from a type altogether.

Description:
- `final` virtual member functions
- `final` on destructors
- `final` pure virtual functions
- `final` and its interactions with `virtual` and `override`
- `final` user-defined types

Use Cases:
- Suppressing derivation to ensure portability
- Improving performance of concrete classes
- Restoring performance lost to mocking
- Improving performance in a protocol hierarchy

Potential Pitfalls:
- Contextual keywords are contextual
- Systemic lost opportunities for reuse
- Attempting to defend against the hiding of a non `virtual` function

Annoyances:
- Making empty types `final` precludes the empty-base-class optimization

# C++11 `friend`

Extended `friend` Declarations:
- The target of a `friend` declaration has been extended to allow designation of (1) a type alias, (2) a named template parameter, or (3) any previously declared class type such that if the target is not found the `friend` declaration will simply fail to compile rather than introduce a new declaration into the enclosing scope.
- A `friend` declaration located within a given user-defined type (UDT) grants a designated type (or free function) access to private and protected members of that class. Because the extended `friend` syntax does not affect function friendships, this feature section addresses extended friendship only between types.

Use Cases:
- Safely declaring a previously declared type to be a friend
- Using the PassKey idiom to enforce initialization
- Granting a specific type access to a single `private` function
- Curiously recurring template pattern
  - Synthesizing equality using the curiously recurring template pattern
  - Compile-time polymorphism using the curiously recurring template pattern
  - Compile-time visitor using the curiously recurring template pattern

Potential Pitfalls:
- Long-distance friendship

# C++11 `inline namespace`

Transparently Nested Namespaces:
- An `inline namespace` is a nested namespace whose member entities closely behave as if they were declared directly within the enclosing namespace.

  ```c++
  namespace n
  {
    inline namespace v2
    {
      struct T {};
      int d;
    }
    // using namespace v2;  // redundant
  }
  ```

Use Cases:
- Facilitating API migration
- Link-safe ABI versioning
- Build modes and ABI link safety
- Enabling selective `using` directives for short-named entities

Potential Pitfalls:
- `inline-namespace`–based versioning doesn’t scale
- Relying on `inline namespace`s to solve library evolution
- Inconsistent use of `inline` keyword is ill formed, no diagnostic required

Annoyances:
- Inability to redeclare across namespaces impedes code factoring
- Only one namespace can contain any given `inline namespace`

# C++11 `noexcept` Specifier

The `noexcept` Function Specification:
- A function having a `noexcept` exception specification denotes a programmatically observable, runtime-enforced guarantee that no thrown C++ exception will ever escape that function’s body.
- We may choose to decorate a function to indicate that it cannot exit via a thrown exception; any exception that would have escaped will instead be caught automatically at runtime, and `std::terminate` will be invoked. We provide this programmatically accessible annotation by inserting the `noexcept` exception specification [...].

Use Cases:
- Declaring non-throwing move operations
- Defining a `noexcept swap`
- A wrapper that provides `noexcept` move operations
- Reducing object-code size

Potential Pitfalls:
- Overly strong contracts guarantees
- Conflating `noexcept` with *nofail*
- Accidental `terminate`
- Forgetting to use the `noexcept` operator in the `noexcept` specifier
- Imprecise expressions in a `noexcept` specification
- Unrealizable runtime performance benefits
  - The hot path and the cold path
  - Older exception-handling implementations
  - The zero-cost exception model
  - Practically realizable performance improvements

Annoyances:
- Algorithmic optimization is conflated with reducing object-code size
- Code duplication
- Exception specifications are not part of a function’s type
- ABI changes in future versions of C++
- Exception specifications do not trigger *SFINAE*

# C++11 Ref-Qualifiers

Reference-Qualified Member Functions:
- Qualifying a non `static` member function with either an `&` or `&&` refines its signature based on the value category — i.e., lvalue or rvalue, respectively — of the expression used to evoke it, thus enabling two distinct overloaded implementations of that member function.

  ```c++
  struct Class2
  {
    void mf() &;  // restricted to lvalue expressions
    void mf() &&; // restricted to rvalue expressions
  };
  ```

Use Cases:
- Returning a sub-object of an rvalue
- Forbidding modifying operations on rvalues
- Optimizing immutable types and builder classes

Potential Pitfalls:
- Forbidding modifications to rvalues breaks legitimate use cases

Annoyances:
- Providing ref-qualified overloads may be a maintenance burden

# C++11 `union`

Unions Having Non-Trivial Members:
- Any non-reference type is permitted to be a member of a `union`.
- Prior to C++11, only trivial types — e.g., fundamental types, such as `int` and `double`, enumerated or pointer types, or a C-style array or `struct` (a.k.a. a POD) — were allowed to be members of a `union`.
- C++11 relaxes such restrictions on `union` members, such as `d_s` below, allowing any type other than a reference type to be a member of a `union`.

  ```c++
  union U0
  {
    int         d_i;  // OK
    std::string d_s;  // compile-time error in C++03 (OK as of C++11)
  };
  ```

Use Cases:
- Implementing a *sum type* as a discriminated `union`

Potential Pitfalls:
- Inadvertent misuse can lead to latent undefined behavior at runtime

# C++14 `auto` Return

Function (`auto`) Return-Type Deduction:
- The return type of a function can be deduced from the `return` statements in its definition if a *placeholder* (e.g., `auto`) is used in place of the return type in the function’s prototype.
- C++11 provides a limited capability for determining the return type of a function from the function’s arguments using the `decltype` operator, typically in a trailing return type:

  ```c++
  template <typename Container, typename Key>
  auto search11(const Container& c, const Key& k) -> decltype(c.find(k))
  {
    return c.find(k);
  }
  ```

- As of C++14, the return type of a function can instead be deduced directly from the `return` statement(s) inside the function definition:

  ```c++
  template <typename Container, typename Key>
  auto search14(const Container& c, const Key& k)
  {
    return c.find(k);  // Return type is deduced here.
  }
  ```

Use Cases:
- Complicated return types
- Let the compiler apply the rules
- Returning a lambda expression
- Delaying return-type deduction

Potential Pitfalls:
- Negative impacts on abstraction and insulation
- Reduced clarity

Annoyances:
- Implementation-order sensitivity
- No SFINAE in function body

# C++14 `decltype(auto)`

Deducing Types Using `decltype` Semantics:
- n a C++14 variable declaration, `decltype(auto)` can act as a placeholder type that is deduced to exactly match the type of the variable’s initializer, preserving the initializer’s value category, unlike the `auto` placeholder.

```c++
int&    f2();
C&&     f3();
C       c1;
const C cc1;

auto           v1  = 4;      // deduced as int
decltype(auto) v2  = 4;      //    "     " int

auto           v3  = f2();   //    "     " int
decltype(auto) v4  = f2();   //    "     " int&

auto           v5  = f3();   //    "     " C
decltype(auto) v6  = f3();   //    "     " C&&

auto           v7  = cc1;    //    "     " C
decltype(auto) v8  = cc1;    //    "     " const C

auto           v9  = (cc1);  //    "     " C
decltype(auto) v10 = (cc1);  //    "     " const C&
```

Use Cases:
- Exact capture of an expression’s type and value category
- Return type of a proxy iterator or moving iterator

Potential Pitfalls:
- Hidden dangling references
- Poor signaling of intent

Annoyances:
- `decltype(auto)` stands alone
