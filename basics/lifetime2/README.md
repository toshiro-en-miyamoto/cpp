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
