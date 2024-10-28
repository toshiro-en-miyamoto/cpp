# Lifetime

Every object and reference has a *lifetime*, which is a runtime property: for any object or reference, there is a point of execution of a program when its lifetime begin, and there is a moment when it ends.

Topics to be discussed include:

- object and reference lifetime
- temporary object lifetime
- implicit-lifetime types
- dangling references

## Object and reference lifetime

The lifetime of an object begins when:

- storage with the proper alignment and size for its type is obtained, and
- its initialization (if any) is complete, including
  - default initialization via no constructor or
  - trivial default constructor,
- except that
  - an array object's lifetime may also begin if it is allocated by `std::allocator::allocate`.

The lifetime of an object ends when:

- the object is destroyed (if it is of a non-class type),
- the destructor call starts (if it is of a class type), or
- the storage which the object occupies is released or reused

The lifetime of a reference:

- begins when its initialization is complete and
- ends as if it were a scalar object.

## Temporary object lifetime

Temporary objects are created when a prvalue is materialized so that it can be used as glvalue, which occurs in the following situations:

- with the following tools:

  ```c++
  #include <type_traits>

  template <typename>   struct is_prvalue_expr       : std::true_type  {};
  template <typename T> struct is_prvalue_expr<T&>   : std::false_type {};
  template <typename T> struct is_prvalue_expr<T&&>  : std::false_type {};

  template <typename>   struct is_lvalue_expr        : std::false_type {};
  template <typename T> struct is_lvalue_expr<T&>    : std::true_type  {};
  template <typename T> struct is_lvalue_expr<T&&>   : std::false_type {};

  template <typename>   struct is_xvalue_expr        : std::false_type {};
  template <typename T> struct is_xvalue_expr<T&>    : std::false_type {};
  template <typename T> struct is_xvalue_expr<T&&>   : std::true_type  {};
  ```

- binding a reference to a prvalue

  ```c++
  #include <string>
  std::string s1 = "Test";
  const std::string& r2 = s1 + s1;
  static_assert(is_prvalue_expr<decltype((s1 + s1))>::value);
  ```

- initializing an object of type `std::initializer_list<T>` from a brace-enclosed initializer list

  ```c++
  #include <initializer_list>
  std::initializer_list<int> il{ 1, 2, 3 };
  ```

- when performing member access on a class prvalue

  ```c++
  struct S { int m; } s;
  static_assert(is_lvalue_expr<decltype((s.m))>::value);
  static_assert(is_prvalue_expr<decltype((S()))>::value);
  static_assert(is_xvalue_expr<decltype((S().m))>::value);
  ```

- when performing an array-to-pointer conversion or subscripting on an array prvalue

  ```c++
  int* get_array() {
    static int a[] = {1, 2, 3};
    return a;
  }
  static_assert(is_prvalue_expr<decltype((get_array()))>::value);
  static_assert(is_lvalue_expr<decltype((get_array()[1]))>::value);
  ```

- for unevaluated operands in `sizeof` and `typeid`
- when a prvalue appears as a discarded-value expression 
  - A [discarded-value expression](https://en.cppreference.com/w/cpp/language/expressions#Discarded-value_expressions) is an expression that is used for its side-effects only.
  - The value calculated from such expression is discarded.
  - Such expressions include the full-expression of any expression statement.

All temporary objects are destroyed as the last step in evaluating the full-expression that (lexically) contains the point where they were created. There are the following exceptions from that:

- The lifetime of a temporary object may be extended by binding to a reference.

  ```c++
  void f() {
    std::string s1 = "Test";
    const std::string& r2 = s1 + s1;    // creates a temporary object 's1 + s1'
    std::cout << r2;                    // its lifetime is extended
  }                                     // lifetime of r2 and the temporary ends
  ```

- The lifetime of a temporary object created in a structured binding declaration (introduced by the initializer for a variable with unique name) is extended to the end of the structured binding declaration.

  ```c++
  #include <set>
  std::string f(std::set<std::string> s, std::string e) {
    if (auto [iter, success] = s.insert(e); success) {
      return "'" + *iter + "' inserted.";
    } else {
      return "'" + *iter + "' already exists.";
    } // lifetime of 'iter' and 'success` is extended
  }
  ```

- The lifetime of a temporary object created in the range-initializer of a [range-for](https://en.cppreference.com/w/cpp/language/range-for) statement that would otherwise be destroyed at the end of the range-initializer is extended to the end of the loop body.

  ```c++
  #include <list>
  using T = std::list<int>;
  const T& f1(const T& t) { return t; }
  T g();

  void foo() {
    for (auto& e : f1(g())) {}    // possibly dangling reference to a temporary
    // the temporary was destroyed at the end of the full expression ‘f1(g()())’

    for (T l = f1(g()); auto& e : l) {}
  }
  ```

## Implicit-lifetime types

The following types are collectively called [implicit-lifetime types](https://en.cppreference.com/w/cpp/named_req/ImplicitLifetimeType):

- scalar types
- implicit-lifetime class types
- array types
- cv-qualified versions of these types

The following types are collectively called [scalar types](https://en.cppreference.com/w/cpp/named_req/ScalarType):

- arithmetic types
- enumeration types
- pointer types
- pointer-to-member types
-`std::nullptr_t`
- cv-qualified versions of these types

An [implicit-lifetime class](https://en.cppreference.com/w/cpp/language/classes#Implicit-lifetime_class) is a class that

- is an aggregate whose destructor is not user-provided, or

  ```c++
  // S is an aggregate without user-provided destructor
  struct S { int x, y; };
  ```

- has at least one trivial eligible constructor and a trivial, non-deleted destructor

  ```c++
  struct B { B() {} };
  static_assert(std::is_trivial_v<B> == false);
  ```

## Default constructors

A [default constructor](https://en.cppreference.com/w/cpp/language/default_constructor) is a constructor which can be called with no arguments.

*Implicitly-declared default constructor*: If there is no user-declared constructor or constructor template for a class type, the compiler implicitly declare a default constructor as an `inline public` member of its class.

*Implicitly-defined default constructor*: If the implicitly-declared or explicitly-defaulted constructor is not defined as deleted, it is defined (that is, a function body is generated and compiled) by the compiler if odr-used or needed for constant evaluation, and

- it has the same effect as a user-defined constructor with empty body and empty initializer list,

  ```c++
  struct A {
    int x;
    A(int x = 1): x(x) {}       // user-defined default constructor
  };
  ```

- that is, it calls the default constructors of the bases and of the non-static members of this class.

  ```c++
  struct B : A {
    // B::B() is implicitly-defined, calls A::A()
  };
  struct C {
    A a;
    // C::C() is implicitly-defined, calls A::A()
  }
  ```

If the requirements of a `constexpr` function, the generated constructor is `constexpr`.

If some user-defined constructors are present, the user may still force the automatics generation of a default constructor by the compiler that would be implicitly-declared otherwise with the keyword `default`.

```c++
struct D : A {
  D(int y) : A(y) {}
  // D::D() is not declared because another constructor exists
};
struct E : A {
  E(int y) : A(y) {}
  E() = default;    // explicitly defaulted, calls A::A()
};
```

*Deleted default constructor*: The implicitly-declared or explicitly-defaulted default constructor for class `T` is defined as deleted, if any of the following conditions is satisfied:

- `T` has a non-static data member of reference type without a default initializer.
- `T` has a non-variant non-static non-const-default-constructible data member of const-qualified type without a default member initializer.
- etc.

  ```c++
  struct F {
    int& ref;     // reference member
    const int c;  // const member
    // F::F() is implicitly defined as deleted
  };
  ```

If no user-defined constructors are present and the implicitly-declared default constructor is not trivial, the user may still inhibit the automatic generation of an implicitly-defined constructor by the compiler with the keyword `delete`.

```c++
struct G {
  // user-defined constructor prevents implicit generation
  // of a default constructor
  G(const G&) {}
  // G::G() is implicitly defined as deleted
};
struct H {
  H(const H&) = delete;
  // H::H() is implicitly defined as deleted
};
struct I {
  I(const I&) = default;
  // I::I() is implicitly defined as deleted
};
```

*Trivial default constructor*: A trivial default constructor is a constructor that performs no action. All data types compatible with the C language (POD types) are trivially default-constructible.

A default constructor is eligible if all following conditions are satisfied:

- It is not deleted.
- Its associated constraints (if any) are satisfied.
- Among all default constructors whose associated constraints are satisfied, it is more constrained than any other default constructor.

Triviality of eligible default constructors determines whether the class is an implicit-lifetime type, and whether the class is a trivial type.