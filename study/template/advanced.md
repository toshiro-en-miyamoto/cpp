# Advanced Template Concepts

## Name binding

The term **name binding** refers to the process of finding the declaration of each name that is used within a template. There are two kinds of names used within a template: **dependent names** and **non-dependent names**. The former are names that depend on the type or value of a template parameter that can be a type, non-type, or template parameter. Names that don’t depend on template parameters are called non-dependent. The name lookup is performed differently for dependent and non-dependent names:

- For dependent names, it is performed at the point of template instantiation.
- For non-dependent names, it is performed at the point of the template definition.

### Two-phase name lookup

When the compiler passes through the definition of a template it needs to figure out whether a name is dependent or non-dependent. Further name lookup depends on this categorization and happens either at the template definition point (for non-dependent names) or the template instantiation point (for dependent names). Therefore, instantiation of a template happens in two phases:

- The first phase occurs at the point of the definition when the template syntax is checked and names are categorized as dependent or non-dependent.
- The second phase occurs at the point of instantiation when the template arguments are substituted for the template parameters. Name binding for dependent names happens at this point.

```c++
template <typename T>
struct base_parser
{
  void init() {}
};
template <typename T>
struct parser : base_parser<T>
{
  void parse()
  {
    // init()                             // [1]
    this->init();                         // [2]
  }
};
int main()
{
  parser<int> p1;
  p1.parse();
}
```

The `parse` member function calls a function called `init` and the intention is that it’s the base-class method `init` that is invoked here. However, the compiler will issue an error [1], because it’s not able to find `init`. The reason this happens is that `init` is a non-dependent name (as it does not depend on a template parameter). Therefore, it must be known at the point of the definition of the `parser` template. Although a `base_parser<T>::init` exists, the compiler cannot assume it’s what we want to call because the primary template `base_parser` can be later specialized and `init` can be defined as something else (such as a type, or a variable, or another function, or it may be missing entirely).

```c++
// a specialization is allowed to override `init` with a variable
template <>
struct base_parser<short>
{
  short init;
};
// but compilation fails with a call to `init` of the specialization
// "error: called object type 'short' is not a function or function pointer"
int main()
{
  parser<short> p3;
  p3.parse();
}
```

Therefore, name lookup does not happen in the base class, only on its enclosing scope, and there is no function called `init` in `parser`.

This problem can be fixed by making `init` a dependent name [2]. This can be done either by prefixing with `this->` or with `base_parser<T>::`. By turning `init` into a dependent name, its name binding is moved from the point of template definition to the point of template instantiation.

### Dependent template names

In the examples seen so far, the dependent name was a function or a member function. However, there are cases when a dependent name is a type.

```c++
template <typename T>
struct base_parser
{
  using value_type = T;
};
template <typename T>
struct parser : base_parser<T>
{
  // value_type v{};                          // [1] error
  // base_parser<T>::value_type v{};          // [2] error
  typename base_parser<T>::value_type v{};    // [2] OK
};
```

Both `value_type` and `base_parser<T>::value_type` do not work, and the compiler is issuing an error.

- `value_type` does not work because it’s a non-dependent name and therefore it will not be looked up in the base class, only in the enclosing scope.
- `base_parser<T>::value_type` does not work either because the compiler cannot assume this is actually a type. A specialization of `base_parser` may follow and `value_type` could be defined as something else than a type.

In order to fix this problem, we need to tell the compiler the name refers to a type. Otherwise, by default, the compiler assumes it’s not a type. This is done with the `typename` keyword, at the point of definition.

There are actually two exceptions to this rule:

1. When specifying a base class
2. When initializing class members

```c++
struct dictionary_traits
{
  using key_type = int;
  using map_type = std::map<key_type, std::string>;
  static constexpr int identity = 1;
};
template <typename T>
struct dictionary : T::map_type             // [1]
{
  int start_key {T::identity};              // [2]
  typename T::key_type next_key;            // [3] until C++17
  T::key_type previous_key;                 // [4] since C++20
};
```

Until C++17, if we want to define yet another member of the type `T::key_type` (see line [3]) we do need to use `typename`.

The requirements for using `typename` have been relaxed in C++20 making the use of type names easier. The compiler is now able to deduce that we are referring to a type name in a multitude of contexts. For instance, defining a member variable as we did on line [3] previously no longer requires prefixing with the `typename` keyword (see line [4]).

In C++20, `typename` is implicit (can be deduced by the compiler) in the following contexts:

- In using declarations [A]
- In the declaration of data members [B]
- In the declaration or definition of function parameters [C]
- In trailing return types
- In default arguments of type-parameters of a template
- In the type-id of a `static_cast`, `const_cast`, `reinterpret_cast`, or `dynamic_cast` statement

```c++
T::key_type previous_key;                             // [B]
using value_type = T::map_type::mapped_type;          // [A]
void add(T::key_type const&, value_type const&) {}    // [C]
```

### Dependent template names

There are cases when the dependent name is a template, such as a function template or a class template. However, the default behavior of the compiler is to interpret the dependent name as a non-type, which leads to errors concerning the usage of the comparison operator `<`.

```c++
template <typename T>
struct base_parser
{
  template <typename U>
  void init()
  {}
};
template <typename T>
struct parser : base_parser<T>
{
  void parse()
  {
    // base_parser<T>::init<int>();                   // [1]
    base_parser<T>::template init<int>();             // [2]
  }
};
```

The attempt to call it using the `base_parser<T>::init<int>()` syntax, as seen at point [1], results in a compiler error. Therefore, we must use the `template` keyword to tell the compiler the dependent name is a template [2].

Keep in mind that the `template` keyword can only follow the scope resolution operator (`::`), member access through pointer (`->`), and the member access (`.`). Examples of correct usage are:

- `X::template foo<T>()`,
- `this->template foo<T>()`, and
- `obj.template foo<T>()`.

The dependent name does not have to be a function template. It can also be a class template:

```c++
template <typename T>
struct base_parser
{
  template <typename U>
  struct token {};
};
template <typename T>
struct parser : base_parser<T>
{
  void parse()
  {
    // using token_type1 = base_parser<T>::token<int>;        // error
    using token_type1 = base_parser<T>::template token<int>;  // [1]
    using token_type2 = typename base_parser<T>::token<int>;  // [2]
  }
};
```

Notice that the `typename` keyword is not necessary at [1], where the using declaration indicates we are dealing with a type, but is required at [2] because the compiler would otherwise assume it’s a non-type name.

## Template recursion

It is possible to create recursive templates.

```c++
template <unsigned int N>
struct factorial
{
  static constexpr unsigned int
  value = N * factorial<N-1>::value;
};
template <>
struct factorial<0>
{
  static constexpr unsigned int value = 1;
};
static_assert(24 == factorial<4>::value);
int main()
{
  auto result = factorial<4>::value;
}
```

When encountering the instantiation `factorial<4>::value` in the `main` function, the compiler generates all the recursive instantiations from `factorial<4>` to `factorial<0>`.

From these instantiations, the compiler is able to compute the value of the data member `factorial<N>::value`. It should be mentioned again that when optimizations are enabled, this code would not even be generated, but the resulting constant is used directly in the generated assembly code.

```asm
main:
  push  rbp
  mov   rbp, rsp
  mov   DWORD PTR [rbp-4], 24       # result ⟸ 24
  mov   eax, 0
  pop   rbp
  ret
```

The class template `factorial` is basically only a wrapper over the static data member `value`. We can actually avoid it altogether by using a variable template instead.

```c++
template <unsigned int N>
inline constexpr unsigned int factorial = N * factorial<N-1>;
template<>
inline constexpr unsigned int factorial<0> = 1;
int main()
{
  auto result = factorial<4>;
}
```

There is a third approach for computing the factorial at compile time: using function templates.

```c++
template <unsigned int N>
constexpr unsigned int factorial()
{
  return N * factorial<N-1>();
}
template <>
constexpr unsigned int factorial<0>()
{
  return 1;
}
int main()
{
  auto result = factorial<4>();
}
```

> Every version of templates — the class template, the variable template, and the function template — generates the exact same assembly code.

## Function template argument deduction

A function's parameter-type-list is determined as follows:

1. The type of each parameter (including function parameter packs) is determined from its own parameter declaration.
2. After determining the type of each parameter, any parameter of type "array of `T`" or of function type `T` is adjusted to be "pointer to `T`".
3. After producing the list of parameter types, any top-level cv-qualifiers modifying a parameter type are deleted when forming the function type.
4. The resulting list of transformed parameter types and the presence or absence of the ellipsis or a function parameter pack is the function's parameter-type-list.

([Parameter-type-list, Function declaration, C++ reference](https://www.cppreference.com/w/cpp/language/function.html))

Any parameter of type "array of `T`" is adjusted to be "pointer to `T`";

```c++
void process(int* ptr) {}
void process(int arr[]) {}                          // compilation error
// redefinition of void process(int*)
template <typename T> void process(T* ptr) {}
template <typename T> void process(T arr[]) {}      // compilation error
// redefinition of template<class T> void process(T*)
```

Any top-level cv-qualifiers modifying a parameter type are deleted;

```c++
void process(int) {}
void process(int const) {}                          // compilation error
// redefinition of void process(int)
template <typename T> void process(T) {}
template <typename T> void process(T const) {}      // compilation error
// redefinition of template<class T> void process(T)
```

## Class template argument deduction

Before C++17, template argument deduction only worked for functions but not classes.

```c++
template <typename T>
struct wrapper
{
  T data;
};
std::pair<int, double> p{42, 42.0};
std::vector<int> v{1,2,3,4};
wrapper<int> w{42};
```

The C++17 standard has simplified the use of class templates by providing template argument deduction for them. This is possible because the compiler is able to deduce the template arguments from the type of the initializer.

```c++
// since C++17
std::pair p{42, 42.0};        // std::pair<int, double>
std::vector v{1,2,3,4};       // std::vector<int>
// since C++20
wrapper w{42};                // wrapper<int>
```

The way template argument deduction works for class templates is different than for function templates but it relies on the latter. When encountering the name of a class template in a variable declaration or function-style cast, the compiler proceeds to build a set of so-called **deduction guides**.

There are fictional function templates representing constructor signatures of a *fictional class type*. If overload resolution fails on the constructed set of fictional function templates (the return type is not part of the matching process since these functions represent constructors), then the program is ill-formed and an error is generated. Otherwise, the return type of the selected function template specialization becomes the deduced class template specialization.

User-defined deduction guides can be provided in the source code. The syntax is similar to that of functions with a trailing return type but without the `auto` keyword. Deduction guides can be either functions or function templates.

```c++
// if we were to add user-defined deduction guides for the std::pair,
// it must be done in the std namespace
namespace std
{
  template <typename T1, typename T2>
  pair(T1&& v1, T2&& v2) -> pair<T1, T2>;
  template <typename T>
  pair(T&&, char const*) -> pair<T, std::string>;

  pair(char const*, char const*) -> pair<std::string, std::string>;
}
```

Let's look at one more example for user-defined guides, this time for a user-defined class.

```c++
template <typename T>
struct range_t
{
  template <typename Iter>
  range_t(Iter first, Iter last)
  {
    std::copy(first, last, std::back_inserter(data));
  }
  std::vector<T> data;
};
// deduction guide
template <typename Iter> range_t(Iter, Iter)
-> range_t<typename std::iterator_traits<Iter>::value_type>;
// use cases
int arr[] = {1,2,3,4};
range_t r{std::begin(arr), std::end(arr)};
```

Without the deduction guide, compile fails with some error messages (gcc 15);

- class template argument deduction failed
- no matching function for call to 'range_t(int*, int*)'

What the deduction guide is instructing is that when a call to the constructor (of the class template `template <T> struct range_t`) with the two iterator arguments (i.e., of the `Iter` type) is encountered, the value of the template parameter `T` should be deduced to be the value type of the iterator traits (`std::iterator_traits<Iter>::value_type`).

Class template argument deduction is used feature of C++17 with improvements for aggregate types (`wrapper` in the example) in C++20. It helps avoid writing unnecessary explicit template arguments when the compiler is able to deduce it (e.g., `pair v{1,2}`). It also avoid the need for creating factory functions, such as `std::make_pair` or `std::make_tuple`, that were a workaround for benefiting from template argument deduction before it was available for class templates.

## Forwarding references
