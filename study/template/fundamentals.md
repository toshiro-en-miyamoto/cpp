# Templates

This article primarily references to

- Chapter 2. Template Fundamentals, Template Metaprogramming with C++, Marius Bancila, 2022.
- [Templates, C++ reference](https://www.cppreference.com/w/cpp/language/templates.html)

David Musser and Alexander Stepanov defined *generic programming* in 1989, as follows

> Generic programming centers around the idea of abstracting from concrete, efficient algorithms to obtain generic algorithms that can be combined with different data representations to produce a wide variety of useful software.

Template metaprogramming is the C++ implementation of generic programming.

- Bjarne Stroustrup's first papers describing templates in C++ appeared in 1986.
- Templates became available in the C++ language in 1990.
- Stepanov, Musser, and Lee experimented with the implementation in C++ of various generic concepts in the early 1990s.
- The experiment led to the first implementation of the *Standard Template Library* (STL).
- STL was standardized along with the C++ language in 1998 in what became known as C++98.

Newer versions of the C++ standard, collectively referred to as *modern C++*, have introduced various improvements to template metaprogramming.

- C++11:
  - *Variadic templates*: Templates can have a variable number of template parameters.
  - *Template aliases*: Ability to define synonyms for a template type with the help of `using` declarations.
  - *`extern` templates*: To tell the compiler not to instantiate a template in translation unit.
  - *Type traits*: The new header `<type_traits>` contains standard type traits that identify the category of an object and characteristics of a type.
- C++14
  - *Variable templates*: Support for defining variables or static data members that are templates.
- C++17
  - *Fold expressions*: Reduce the parameter pack of a variadic template over a binary operator.
  - *`typename` in template parameters*: The `typename` keyword can be used instead of `class` in a template parameter.
  - *`auto` for constant template parameter*: The keyword `auto` can be used for constant template parameters.
  - *Class Template Argument Deduction (CTAD)*: The compiler infers the type of template parameters from the way an object is instantiated.
- C++20
  - *Template lambda*: Lambdas can be templates just like regular functions.
  - *String literals as template parameters*: String literals can be used as constant template arguments and a new form of the user-defined literal operator for strings.
  - *Constraints*: Define requirements on template arguments.
  - *Concepts*: Named sets of constrains.

(A brief history of templates, Chapter 1. An Introduction to Templates, Template Metaprogramming with C++, Marius Bancila, 2022)

## Fundamental ideas of templates

A template is a C++ entity that defines one of the following:

- a family of classes (class template), which may be nested classes
- a family of functions (function template), which may be member functions
- an alias to a family of types (alias template)
- a family of variables (variable template)
- a concept (since C++20)

When template arguments are provided (or deduced), they are substituted for the template parameters to obtain a specialization of the template, that is, a specific type or a specific function lvalue.

When a class template specialization is referenced in context that requires a complete object type, or when a function template specialization is referenced in context that requires a function definition to exist, the template is *instantiated* (the code for it is actually compiled), unless the template was already explicitly specialized or explicitly instantiated.

([Templates, C++ reference](https://www.cppreference.com/w/cpp/language/templates.html))

Template definitions that you put in the source code is only a blueprint. The compiler will generate code from it based on its use.

The process of generating code from a template by the compiler is called *template instantiation*. This happens by substituting the template arguments for template parameters used in the definition of the template.

Templates are parameterized by one or more *template parameters*, of three kinds:

- *type template parameters*:
  ```c++
  template <
    typename K, typename V,
    typename Comp = std::less<K>,
    typename Alloc = std::allocator<std::pair<const K, V>>
  > class std::map;
  // four type parameters: K, V, Comp, and Alloc
  ```
- *constant template parameters*:
  ```c++
  template <
    typename T,         // type parameter
    std::size_t N       // constant parameter
  > struct std::array;
  ```
- *template template parameters*:
  ```c++
  template<typename T>
  class my_array {};
 
  template <
    typename K, typename V,
    template<typename> typename C = my_array
  > class Map
  {
    C<K> key;
    C<V> value;
  };
  // template<typename> typename C
  ```

(Understanding template terminology, Chapter 1. An Introduction to Templates, Template Metaprogramming with C++, Marius Bancila, 2022)

> Before C++26, constant template parameters were called *non-type template parameter* in the standard wording. The terminology was changed by P2841R6 (2025). ([Template parameters](https://www.cppreference.com/w/cpp/language/template_parameters.html))

The name of the template parameter can be omitted, which can be useful in forwarding declarations:

```c++
template <typename> struct One_type;
template <typename, typename> struct Two_types;
template <std::size_t> struct One_const;
template <typename, std::size_t> struct Type_const;
template <template<typename> typename> struct Temp1;
template <template<typename, typename> typename> struct Temp2;
template <typename, template<typename> typename> struct Temp3;
```

## Defining function templates

Syntax:

- `template` < *parameter-list* > *function-declaration*

Function templates are defined in a similar way ti regular functions, except that the function declaration is preceded by the keyword `template` followed by a list of template parameters between angle brackets.

```c++
template <typename T>
constexpr T add(const T a, const T b)
{
  return a + b;
}
constexpr auto a = add(42, 21);       // T is deduced as int
constexpr auto b = add<int>(42, 21);  // OK but redundant
constexpr auto c = add<>(42, 21);     // OK but redundant
// int add(int const a, int const b) {...}
#include <type_traits>
static_assert(std::is_same_v<const int, decltype(a)>);
```

When the compiler encounters a call to a function template and is able to match the supplied arguments and their types to function template's parameters, it **generates** an actual function from the template and the arguments used to invoke it.

```c++
constexpr auto a = add(42, 21);       // T is deduced as int
// int add(const int a, const int b) {...}
```

If the type of two parameters is ambiguous, the compiler will not be able to deduce them automatically.

```c++
// constexpr auto d = add(41.0, 21);  // is T double or int?
```

You can explicitly provide a type argument so that the compiler generate another instantiation.

```c++
constexpr auto d = add<double>(41.0, 21);
static_assert(std::is_same_v<const double, decltype(d)>);
```

Or, you can provide an explicit instantiation definition in source code as:

```c++
template double add<double>(const double, const double);
// Explicit instantiation definition
// or, template double add<>(const double, const double);
// or, template double add(const double, const double);
```

> Function templates are only blueprints for creating actual functions and only exist in source code. Unless explicitly called or explicitly instantiated in your source code, the function templates will not be presented in the compiled executable.

## Defining class templates

Syntax:

- `template` < *parameter-list* > *class-declaration*

A class template defies a family of classes.

```c++
template <typename T = int>
struct Wrapper
{
  T value {};
};
#include <type_traits>
constexpr Wrapper w1{4};
static_assert(4 == w1.value);
static_assert(std::is_same_v<int, decltype(w1.value)>);
constexpr Wrapper<double> w2{5};
static_assert(5.0 == w2.value);
static_assert(std::is_same_v<double, decltype(w2.value)>);
```

As long as the class template is not used anywhere in your source code, the compiler will not generate code from it. For that to happen, the class template must be instantiated and all its parameters properly matched to arguments either explicitly, by the user, or implicitly, by the compiler.

## Defining member functions

Non-template classes can have member function templates:

```c++
struct Increment
{
  template <typename T>
  T operator()(T&& value) const
  {
    return ++value;
  }
};
static_assert(2 == Increment{}(1));
```

Template classes can have non-template member functions:

```c++
template <typename T>
struct Less
{
  constexpr bool operator()(const T& lhs, const T& rhs) const
  { return lhs < rhs; }
};
static_assert(Less<double>{}(5, 5.6) == true);
```

Template classes can also have template member functions:

```c++
template <typename T>
struct Wrapper
{
  template <typename U>
  constexpr operator U() const
  {
    return static_cast<U>(value);
  }
  T value{};
};
constexpr Wrapper<int> w;
#include <type_traits>
static_assert(std::is_same_v<short, decltype((short)w)>);
```

In this case, however, the template parameters of the member function template (`typename U`) must differ from the template parameters of the class template (`typename T`); otherwise, the compiler will generate an error.

## Implicit instantiation

Implicit instantiation occurs when the compiler generates definitions based on the use of templates and when no explicit instantiation is present.

- For function templates, implicit instantiation occurs when the user code refers to a function in a context that requires its definition to exist.
- For class templates, implicit instantiation occurs when the user code refers to a template in a context when a complete type is required.

> For the detail about *use of entities*, refer to [odr-used, Definitions and ODR, C++ reference](https://www.cppreference.com/w/cpp/language/definition.html).

Let's consider this code:

```c++
template <typename T>
struct foo
{
  void f() {}
};
int main()
{
  foo<int> x;
}
```

In `main`, we define a variable of the type `foo<int>` but do not use any of its members.

- Because it encounters this use of `foo`, the compiler implicitly defines a specialization of `foo` for the `int` type.
- Because the function `f` is not invoked in our code, it is only declared but not defined.

```c++
template<>
struct foo<int>
{
  inline void f();      // only declared but not defined
};
```

Should we add a call `f` in `main`, the specialization would change as follows:

```c++
template<>
struct foo<int>
{
  inline void f() { }   // defined (meaning also declared)
};
```

When a class template contains static members, those members are not implicitly instantiated when the compiler implicitly instantiates the class template (`[1]` and `[3]`) but only when the compiler needs their definition (`[2]` and `[4]`).

```c++
template <typename T>
struct foo
{
  inline static T data{};   // [A]
};

#include <cassert>
int main()
{
  foo<int> a;               // [1]
  assert(a.data == 0);      // [2]

  foo<double> b;            // [3]
  b.data = 2;               // [4]
  assert(a.data == 0);      // [5]
  assert(b.data == 2);

  foo<double> c;
  assert(c.data == 2);      // [6]
}
```

`[5]` shows that every specialization (`foo<int>` and `foo<double>`) of the template `foo` has its own copy of static members (`foo<int>::data` and `foo<double>::data`). And `[6]` shows that `b` and `c` share the static member `foo<double>.data`.

## Explicit instantiation

Explicit instantiation definition helps

- distribute libraries,
- reduce build times, and
- reduce executable sizes.

If you are building a library that you want to distribute as a `.lib` file and that library uses templates, the template definitions that are not instantiated are not put into the library. But that leads to increased build times of your user code every time you use the library.

By forcing instantiations of templates in the library, those definitions are put into the object files and the .lib file you are distributing. As a result, your user code only needs to be linked to those available functions in the library file.

A problem is that, if the same header that contains a template is included in multiple translation units (`.cpp` files) and the same template instantiation is used (let's say `Wrapper<int>` from our previous examples), then identical copies of these instantiations are put in each translation unit. This leads to increased object sizes.

The problem can be solved with the help of *explicit instantiation declarations*.

| Syntax: explicit instantiation definition                                     |
|-------------------------------------------------------------------------------|
| `template class/struct/union` *template-name* < *arg-list* >;                 |
| `template` *return-type* *func-name* < *arg-list* > ( *param-list* );         |
| `template` *return-type* *func-name* ( *param-list* );                        |

| Syntax: explicit instantiation declaration                                    |
|-------------------------------------------------------------------------------|
| `extern template class/struct/union` *template-name* < *arg-list* >;          |
| `extern template` *return-type* *func-name* < *arg-list* > ( *param-list* );  |
| `extern template` *return-type* *func-name* ( *param-list* );                 |

An explicit instantiation definition forces instantiation of the class, struct, or union they refer to.

```c++
template <typename T>
struct Y                      // template definition
{
  void mf() {}                // non-template member function
};

template struct Y<char>;      // explicit instantiation
template void Y<int>::mf();   // explicit instantiation
```

A trailing template-argument can be left unspecified in an explicit instantiation of a function template specialization or of a member function template specialization if it can be deduced from the function parameter:

```c++
template<typename T>
void f(T s) { ... }
 
template void f<long>(long);  // instantiates f<long>(long)
template void f<>(char);      // f<char>(char), arg deduced
template void f(int);         // f<int>(int), arg deduced
```

An explicit instantiation declaration (an `extern` template) skips implicit instantiation step: the code that would otherwise cause an implicit instantiation instead uses the explicit instantiation definition provided elsewhere (resulting in link errors if no such instantiation exists). This can be used to reduce compilation times by explicitly declaring a template instantiation in all but one of the source files using it, and explicitly defining it in the remaining file.

When you do explicit template declarations, keep in mind that a class member function that is defined within the body of the class is always considered `inline` and therefore it will always be instantiated. Therefore, you can only use the `extern` keyword for member functions that are defined outside of the class body.

## Template specialization

A *template specialization* is the definition created from a template instantiation. The template that is being specialized is called the *primary template*. You can provide an *explicit specialized* definition for a given set of template arguments, therefore overwriting the implicit code the compiler would generate instead.

(Understanding template specialization, [Bancila 2022])

Specializations may also be provided explicitly:

- *full specializations* are allowed for class, variable and function templates.
- *partial specializations* are only allowed for class templates and variable templates.

([Templates, C++ reference](https://www.cppreference.com/w/cpp/language/templates.html))

### Explicit (full) specialization

[Explicit (full) template specialization](https://en.cppreference.com/w/cpp/language/template_specialization.html) allows customizing the template code for a given set of template arguments.

```c++
template <typename T>
struct is_floating_point
{
  constexpr static bool value = false;
};
```

`is_floating_point` is the primary template. It contains a `constexpr static` Boolean data member called `value` that is initialized with the `false` value.

Then, we have three full specializations of this primary template, for the `float`, `double`, and `long double` types. These new definitions change the way value is being initialized using `true` instead of `false`.

```c++
template <>
struct is_floating_point<float>
{
  constexpr static bool value = true;
};
template <>
struct is_floating_point<double>
{
  constexpr static bool value = true;
};
template <>
struct is_floating_point<long double>
{
  constexpr static bool value = true;
};
```

As a result, we can use this template to write code as follows:

```c++
static_assert(is_floating_point<int>::value == false);
static_assert(is_floating_point<float>::value);
static_assert(is_floating_point<double>::value);
static_assert(is_floating_point<long double>::value);
#include <string>
static_assert(is_floating_point<std::string>::value == false);
```

Template instantiation and specialization are distinct:

```c++
// instantiation
template
struct is_floating_point<int>;

// specialization
template <>
struct is_floating_point<float>
{
  constexpr static bool value = true;
};
```

The following code exemplifies an incomplete specialization; the code should have defined the member `value`.

```c++
template <>                       // having <>
struct is_floating_point<int>;    // but no definition

// compile error
// incomplete type 'is_floating_point<int>'
// used in nested name specifier
static_assert(is_floating_point<int>::value == false);
```

The following code exemplifies the syntax error of specialization.

```c++
// compile error:
// an explicit instantiation cannot have a definition;
// use `template <>` to declare a specialization.
template                          // no <>
struct is_floating_point<float>   // having definition 
{
  constexpr static bool value = true;
};
```

### Partial specialization

Partial specialization occurs when you specialize a primary template but only specify some of the template arguments. This means a partial specialization has both a template parameter list (which follows the `template` keyword) and a template argument list (which follows the template name).

```c++
template <typename T, std::size_t S>
struct collection
{
   void operator()()
   { std::cout << "primary template\n"; }
};
template <typename T>
struct collection<T, 10>
{
   void operator()()
   { std::cout << "partial specialization <T, 10>\n"; }
};
template <std::size_t S>
struct collection<int, S>
{ 
   void operator()()
   { std::cout << "partial specialization <int, S>\n"; }
};
template <typename T, std::size_t S>
struct collection<T*, S>
{ 
   void operator()()
   { std::cout << "partial specialization <T*, S>\n"; }
};
```

To understand how partial specialization is useful, let's take a look at a real-world example.

In this example, we want to create a function that formats the content of an array in a nice way and outputs it to a stream. The content of a formatted array should look like `[1,2,3,4,5]`. However, for arrays of `char` elements, the elements should not be separated by a comma but instead displayed as a string within square brackets, such as [demo].

```c++
// the primary template
template <typename T, size_t S>
std::ostream& pretty_print(
  std::ostream& os, 
  std::array<T, S> const& arr
) {
   os << '[';
   if (S > 0)
   {
      size_t i = 0;
      for (; i < S - 1; ++i)
         os << arr[i] << ',';
      os << arr[S-1];
   }
   os << ']';
   return os;
}
// partial specialization <char, S>
template <size_t S>
std::ostream& pretty_print(
  std::ostream& os, 
  std::array<char, S> const& arr
) {
   os << '[';
   for (auto const& e : arr)
      os << e;
   os << ']';
   return os;
}
```

With partial specialization `<char, S>`, the second `pretty_print` is a function template with a single template parameter `S`, which is a non-type template parameter indicating the size of the array. The type template parameter is explicitly specified as `char`.

### Members of partial specializations

The template parameter list and the template argument list of a member of a partial specialization must match the parameter list and the argument list of the partial specialization.

Just like with members of primary templates, they only need to be defined if used in the program.

Members of partial specializations are not related to the members of the primary template.

Explicit (full) specialization of a member of a partial specialization is declared the same way as an explicit specialization of the primary template.

```c++
// primary template
template<class T, int I>
struct A
{
  void f();               // member declaration
};
template<class T, int I>
void A<T, I>::f() {}      // member definition
 
// partial specialization; int I = 2
template<class T>
struct A<T, 2>
{
  void f();
  void g();
  void h();
};
template<class T>
void A<T, 2>::g() {}      // member definition
 
// explicit (full) specialization
// of a member of partial specialization
template<>
void A<char, 2>::h() {}
 
int main()
{
  A<char, 0> a0;
  A<char, 2> a2;
  a0.f();         // OK, primary template
  a2.g();         // OK, partial specialization
  a2.h();         // OK, fully-specialized definition
  a2.f();         // error: no definition of f() in
                  // the partial specialization A<T,2>
                  //  (the primary template is not used)
}
```

## Defining variable templates

Variable templates were introduced in C++14 and allow us to define variables that are templates either at namespace scope, in which case they represent a family of global variables, or at class scope, in which case they represent static data members.

```c++
template <typename T>
struct is_floating_point
{
  constexpr static bool value = false;
};
template <>
struct is_floating_point<float>
{
  constexpr static bool value = true;
};
template <>
struct is_floating_point<double>
{
  constexpr static bool value = true;
};
template <>
struct is_floating_point<long double>
{
  constexpr static bool value = true;
};
// this is a variable template
template <typename T>
constexpr bool is_floating_point_v = is_floating_point<T>::value;
```

And we can use it as:

```c++
static_assert(is_floating_point_v<int> == false);
static_assert(is_floating_point_v<float>);
static_assert(is_floating_point_v<double>);
static_assert(is_floating_point_v<long double>);
#include <string>
static_assert(is_floating_point_v<std::string> == false);
```

## Defining alias templates

*Type alias* is a name that refers to a previously defined type. It does not introduce a new type and it cannot change the meaning of an existing type name. There is no difference between a type alias declaration and `typedef` declaration.

```c++
// type alias declaration
// the name 'flags' now denotes a type
using flags = std::ios::base::fmtflags;
```

*Alias template* is a name that refers to a family of types.

```c++
// type alias used to hide a template parameter
template <typename T>
using my::string = std::basic_string<T, std::char_traits<T>>;

my::string<char> s;
```

An alias template is a template which, when specialized, is equivalent to the result of substituting the template arguments of the alias template for the template parameters in the `type-id`.

```c++
template<class T>
struct Alloc {};
 
template<class T>
using Vec = vector<T, Alloc<T>>;
// type-id is vector<T, Alloc<T>>
 
Vec<int> v;
// Vec<int> is the same as vector<int, Alloc<int>>
```

Type aliases can introduce a member typedef name.

```c++
template <typename T>
struct Container { using value_type = T; };

template<typename C, typename T = C::value_type>
constexpr T info(const C& c)
{
  return T{};
}

#include <type_traits>
const Container<int> c;
static_assert(std::is_same_v<int, decltype(info(c))>);
```

([Type alias, alias templates, C++ reference](https://www.cppreference.com/w/cpp/language/type_alias.html))

Alias template declarations have an important advantage over `typedef`s as they allow us to create aliases for templates. An *alias template* is a name that refers not to a type but a family of types. Remember, a template is not a class, function, or variable but a blueprint that allows the creation of a family of types, functions, or variables.

```c++
struct delivery_address_t {};
struct invoice_address_t {};

template <typename T>
using customer_address_t = std::map<int, std::vector<T>>;

using customer_delivery_address_t 
    = customer_address_t<delivery_address_t>;
using customer_invoice_address_t  
    = customer_address_t<invoice_address_t>;
```

Since `std::vector<T>` is not a type, but a family of types, `customer_addresses_t<T>` defines a family of types. Then two type aliases are introduced from the family: `delivery_address_t` and `invoice_address_t`.

Alias templates can neither be fully nor partially specialized.

```c++
// alias template for std::vector<T>
template <typename T, std::size_t S>
using list_t = std::vector<T>;
// but it cannot be specialized - compile error
template <typename T>
using list_t<T, 1> = T;
```

However, there are ways to overcome this limitation. A solution is to

1. create a class template
2. with a type alias member and
3. specialize the class. Then
4. create an alias template that refers to the type alias member.

```c++
template <typename T, std::size_t S>
struct list                                   // 1
{
  using type = std::vector<T>;                // 2
};
template <typename T>
struct list<T, 1>                             // 3
{
  using type = T;
};
template <typename T, size_t S>
using list_t = typename list<T, S>::type;     // 4

static_assert(std::is_same_v<list_t<int, 1>, int>);
static_assert(std::is_same_v<list_t<int, 2>, std::vector<int>>);
```

## Lambda expressions and generic programming

Lambda expressions, which were introduced in C++11, have received several updates in later versions of the standard. There are notably two:

- *Generic lambdas*, introduced in C++14, allow us to use the auto specifier instead of explicitly specifying types. This transforms the generated function object into one with a template function-call operator.
- *Template lambdas*, introduced in C++20, allow us to use the template syntax to explicitly specify the shape of the templatized function-call operator.

```c++
// regular lambda, C++11
auto lambda1 = [](int a) {return a + a; };
// generic lambda, C++14
auto lambda2 = [](auto a) {return a + a; };
// template lambda, C++20
auto lambda3 = []<typename T>(T a) { return a + a; };
```

`lambda2` is a generic lambda. The compiler proceeds to instantiate specializations of it for all the types of the arguments it's invoked with.

Not surprisingly, the compiler will generate the same code for the third lambda expression, `lambda3`, which is a template lambda, only available in C++20. The question that arises from this is how are generic lambdas and lambda templates different?

```c++
#include <array>
#include <numeric>

auto lambda4
= []<typename T, std::size_t N>(std::array<T, N> const& a)
{
  return 
  std::accumulate(a.begin(), a.end(), static_cast<T>(0));
};

constexpr auto a = std::array{1, 2, 3, 4};
static_assert(10 == lambda4(a));
```

`lambda4` has a single argument, a `std::array`. However, the type of the elements of the array and the size of the array are specified as template parameters of the lambda template.

An interesting benefit of generic lambdas over regular lambdas concerns recursive lambdas.

Lambdas do not have names; they are anonymous, therefore, you cannot recursively call them directly. Instead, you have to define a `std::function object`, assign the lambda expression to it, and also capture it by reference in the capture list.

```c++
#include <functional>
#include <cassert>
void test() {
  std::function<int(int const)> factorial;
  factorial = [&factorial](int const n) -> int {
    return n < 2 ? 1 : n * factorial(n - 1);
  };
  assert(120 == factorial(5));
}
```

This can be simplified with the use of generic lambdas. They don't require a `std::function` and its capture.

```c++
constexpr auto factorial = [](auto f, int const n) -> int {
  return (n < 2) ? 1 : n * f(f, n - 1);
};
static_assert(120 == factorial(factorial, 5));
```

(END)