# Types

Objects, references, functions including function template specializations, and expressions have a property called [type](https://en.cppreference.com/w/cpp/language/type), which both

- restricts the operations that are permitted for those entities and
- provides semantic meaning to the otherwise generic sequences of bits.

## Type classification

### The C++ type system

The C++ type system consists of the following types:

- fundamental types (`std::is_fundamental`)
  - `void` (`std::is_void`);
  - `std::nullptr_t` (`std::is_null_pointer`); (*since C++11*)
  - arithmetic types (`std::is_arithmetic`):
    - integral types, incl. cv-qualified versions (`std::is_integral`):
      - `bool`;
      - character types:
        - `char`, `signed char`, `unsigned char`;
        - `char8_t`; (*since C++20)
        - `char16_t`, `char32_t`; (*since C++11)
        - `wchar_t`;
      - integer types:
        - `signed car`, `short`, `int`, `long`, `long long`;
        - `unsigned char`, `unsigned short`, `unsigned`, `unsigned long`, `unsigned long long`;
    - floating point types, incl. cv-qualified versions (`std::is_floating_point`):
      - `float`, `double`, `long double`;
- compound types (`std::is_compound`):
  - reference types (`std::is_reference`):
    - lvalue reference types (`std::is_lvalue_reference`):
      - lvalue reference to object types;
      - lvalue reference to function types;
    - rvalue reference types (`std::is_rvalue_reference`): (*since C++11)
      - rvalue reference to object types;
      - rvalue reference to function types;
  - pointer types (`std::is_pointer`):
    - pointer to object types;
    - pointer to function types;
  - pointer to member type (`std::is_member_pointer`):
    - pointer to data member types (`std::is_member_object_pointer`);
    - pointer to member function types (`std::is_member_function_pointer`);
  - array types (`std::is_array`);
  - function types (`std::is_function`);
  - enumeration types (`std::is_enum`):
    - unscoped enumeration types;
    - scoped enumeration types (`std::is_scoped_enum`); (*since C++11*)
  - class types
    - non-union types (`std::is_class`)
    - union types (`std::is_union`)

For every non-cv-qualified type other than reference and function, the type system supports three additional cv-qualified versions of that type (`const`, `volatile`, and `const volatile`). 

Types are grouped in various categories:

- *object types*: types that are not function types, reference types, or `void`(`std::is_object`);
- *scalar types*: object types that are not array types or class types (`std::is_scalar`);
- *trivial types*: (`std::is_trivial`)
- *POD types*: (`std::is_pod`)
- *literal types*: (`std::is_literal`)

| object  | scalar  | type                    |
|:-------:|:-------:|-------------------------|
|         |         | `void`                  |
| &check; | &check; | `std::nullptr_t`        |
| &check; | &check; | arithmetic types        |
|         |         | reference types         |
| &check; | &check; | pointer types           |
| &check; | &check; | pointer to member type  |
| &check; |         | array types             |
|         |         | function types          |
| &check; | &check; | enumeration types       |
| &check; |         | class types             |

### Constant and volatile type qualifiers

[cv type qualifiers](https://en.cppreference.com/w/cpp/language/cv) appear in any type specifier to specify constness or volatility of the object being declared or of the type being named.

- A *const object* is an object whose type is const-qualified.
- A non-mutable sub-object of a const object is also a const object.

Such object cannot be modified: attempt to do so directly is a compile-time error, and attempt to do so indirectly (e.g., by modifying the const object through a reference or pointer to non-const type) results in undefined behavior.

- **constant expression**: An expression that can be evaluated at compile time. ([cppreference](https://en.cppreference.com/w/cpp/language/constant_expression)) For example, `constexpr double pi = 3.1415;`. (&sect;3.3.1)
- **`const` variable**: A variable that is not a constant expression (its value is not known at compile time) but do not change value after initialization. For example, `const int c3 = n;`, where `n` is a variable. (&sect;3.3.1)

### `mutable` specifier

`mutable` specifier permits modification of the class member even if the containing object is declared const. `mutable` specifier may appear in the declaration of

- a non-static class members
- of non-reference non-const type:

```c++
class X
{
  mutable const int* p;   // OK
  mutable int* const q;   // ill-formed
  mutable int&       r;   // ill-formed
};
```

### Constness in pointer declaration

A [pointer declaration](https://en.cppreference.com/w/cpp/language/pointer) has the form:

```
* [attr] [cv] declarator
```

Const/volatile qualifier (`cv` in the form) apply to the pointer that is being declared &mdash; not to the pointed-to type, whose qualifications are part of declaration specifier sequence.

- If `cv` appears before `*` in the pointer declaration, it is part of the declaration specifier sequence and applies to the pointed-to object.
- If `cv` appears after `*` in the pointer declaration, it is part of the declarator and applies to the pointer that's being declared. 

| syntax            | meaning                             |
|-------------------|-------------------------------------|
| `const T*`        | pointer to constant object          |
| `T const*`        | pointer to constant object          |
| `T* const`        | constant pointer to object          |
| `const T* const`  | constant pointer to constant object |
| `T const* const`  | constant pointer to constant object |

```c**
int i;                      // i is a con-const int
const int ci = 10;          // ci is a const int

const int *pc = &ci;        // pc is a non-const pointer to const int
const int *const cpc = pc;  // cpc is a const pointer to const int

int *p;                     // p is a non-const pointer to non-const int
int *const cp = &i;         // cp is a const pointer to non-const int
 
i = ci;    // okay: value of const int copied into non-const int
ci++;      // error: const int cannot be changed

pc++;      // okay: non-const pointer (to const int) can be changed
*pc = 2;   // error: pointed-to const int cannot be changed
p = pc;    // error: pointer to non-const int cannot point to const int

cpc++;     // error: const pointer (to const int) cannot be changed

*cp = ci;  // okay: non-const int (pointed-to by const pointer) can be changed
cp = &ci;  // error: const pointer (to non-const int) cannot be changed
```

### Lvalue reference declarations

[Reference declaration](https://en.cppreference.com/w/cpp/language/reference) declares a named variable as a reference, that is, an alias to an already-existing object or function.

A lvalue reference declaration has the form:

```
& [attr] declarator
```

References are not objects; they do not necessarily occupy storage, although the compiler may allocate storage if it is necessary to implement the desired semantics (e.g. a non-static data member of reference type usually increases the size of the class by the amount necessary to store a memory address).

Because references are not objects, there are no arrays of references, no pointers to references, and no references to references: 

```c++
int& a[3];    // error: there are no arrays of references
int&* p;      // error: there are no pointers to references
```

Lvalue references can be used to alias an existing object (optionally with different cv-qualification):

```c++
int main()
{
  std::string str = "Ex";
  std::string& r1 = str;
  const std::string& r2 = str;

  r1 += "ample";    // modifies 'str'
//r2 += "!";        // compile-time error: cannot modify through const ref
  std::cout << r2;  // prints "Example"
}
```

lvalue references to const can extend the lifetimes of temporary objects:

```c++
int main()
{
  std::string s = "Test";
//std::string& r = s + s;         // compile-time error: s + s is temporal
  const std::string& r = s + s;   // const ref can extend the lifetime
  std::cout << r << '\n';
}
```

Lvalue references can also be used to implement *pass-by-reference* semantics in function call:

```c++
void double_string(std::string& s)
{
  s += s;     // 's' is the same object as 'main()'s 'str'
}
int main()
{
  std::string str = "Test";
  double_string(str);
  std::cout << str;   // prints "TestTest"
}
```

When a function's return type is lvalue reference, the function call expression becomes an lvalue expression:

```c++
char& char_number(std::string& s, std::size_t n)
{
  return s.at(n);   // 'string.at()' returns a reference to char
}
```

### Rvalue reference declarations

[Reference declaration](https://en.cppreference.com/w/cpp/language/reference) declares a named variable as a reference, that is, an alias to an already-existing object or function.

A rvalue reference declaration (*since C++11*) has the form:

```
&& [attr] declarator
```

Rvalue references can be used to extend the lifetimes of temporary objects (note, lvalue references to const can extend the lifetimes of temporary objects too, but they are not modifiable through them):

```c++
int main()
{
  std::string s1 = "Test";
//std::string&& r1 = s1;           // error: can't bind to lvalue
 
  const std::string& r2 = s1 + s1; // okay: lvalue ref to const extends lifetime
//r2 += "Test";                    // error: can't modify through ref to const
 
  std::string&& r3 = s1 + s1;      // okay: rvalue ref extends lifetime
  r3 += "Test";                    // okay: can modify through ref to non-const
  std::cout << r3 << '\n';
}
```

When a function has both rvalue reference and lvalue reference overloads, the rvalue reference overload binds to rvalues (including both prvalues and xvalues), while the lvalue reference overload binds to lvalues.

This allows move constructors, move assignment operators, and other move-aware functions (e.g. `std::vector::push_back()`) to be automatically selected when suitable.

```c++
void f(int& x);
void f(const int& x);
void f(int&& x);
 
int main()
{
    int i = 1;
    const int ci = 2;
 
    f(i);  // calls f(int&)
    f(ci); // calls f(const int&)
    f(3);  // calls f(int&&)
           // would call f(const int&) if f(int&&) overload wasn't provided
    f(std::move(i)); // calls f(int&&)
 
    // rvalue reference variables are lvalues when used in expressions
    int&& x = 1;
    f(x);            // calls f(int& x)
    f(std::move(x)); // calls f(int&& x)
}
```

> Note: Rvalue reference variables are lvalues when used in expressions. In the expression `f(x)`, `x` is an expression and is a lvalue.

Because rvalue references can bind to xvalues, they can refer to non-temporary objects:

```c++
int i2 = 42;
int&& rri = std::move(i2); // binds directly to i2
```

This makes it possible to move out of an object in scope that is no longer needed:

```c++
std::vector<int> v{1, 2, 3, 4, 5};
std::vector<int> v2(std::move(v)); // binds an rvalue reference to v
assert(v.empty());
```

### `std::move`

```
template< class T >
constexpr std::remove_reference_t<T>&& move( T&& t ) noexcept;
```

[`std::move`](https://en.cppreference.com/w/cpp/utility/move) (*since C++11*) is used to indicate that an object `t` may be "moved from", i.e. allowing the efficient transfer of resources from `t` to another object.

> Move semantics is nothing else than an additional overload that is allowed and expected to steal data from a source object. [*Back to Basics: C++ Move Semantics*. CppCon 2022]

In particular, `std::move` produces an xvalue expression that identifies its argument `t`. It is exactly equivalent to a `static_cast` to an rvalue reference type.

```c++
void fun(std::vector<int>&& rr);

void use1()
{
  std::vector v{2, 3, 4};
  fun(static_cast<std::vector<int>&&>(v));
}

void use2()
{
  std::vector v{2, 3, 4};
  fun(std::move(v));
}
```

The functions that accept rvalue reference parameters (including move constructors, move assignment operators, and regular member functions such as `std::vector::push_back`) are selected, by overload resolution, when called with rvalue arguments (either prvalues such as a temporary object or xvalues such as the one produced by `std::move`). If the argument identifies a resource-owning object, these overloads have the option, but aren't required, to move any resources held by the argument.

Names of rvalue reference variables are lvalues and have to be converted to xvalues to be bound to the function overloads that accept rvalue reference parameters, which is why move constructors and move assignment operators typically use `std::move`:

```c++
// the expression "arg.member" is lvalue
A(A&& arg) : member(std::move(arg.member))
{}
 
A& operator=(A&& other)
{
    member = std::move(other.member);
    return *this;
}
```

### Forwarding references

### Value categories


