# Item 24: Distinguish universal references from rvalue references.

If a function template parameter has type `T&&` for a deduced type `T`, the parameter is either rvalue reference or lvalue reference.

```c++
template<typename T>
void f(T&& param);
```

Because such references are references, they must be initialized. The initializer for such a reference determines whether it represents an rvalue reference or an lvalue reference. If the initializer is an rvalue, the reference can bind to the rvalue. If the initializer is an lvalue, the reference can bind to the lvalue.

```c++
template<typename T>
void f(T&& param);      // param is either lvalue or rvalue ref

Widget w;

f(w);
f(std::move(w));
```

- In the call `f(w)`, lvalue passed to `f`; `param`'s type is `Widget&` (i.e., an lvalue reference).
- In the call `f(std::move(w))`, rvalue passed to `f`; `param`'s type is `Widget&&` (i.e., an rvalue reference).

`const` qualifier is enough to disqualify a reference from being corrupsed:

```c++
template<typename T>
void f(const T&& param);        // param is an lvalue ref
```

If an object is declared using `auto&&`, the object is either rvalue reference or lvalue reference.

```c++
std::string&& str1 = std::string();
static_assert(
    std::is_rvalue_reference_v<decltype(str1)>
);

auto&& str2 = str1;
static_assert(
    std::is_lvalue_reference_v<decltype(str2)>
);

auto&& str3 = std::move(str1);
static_assert(
    std::is_rvalue_reference_v<decltype(str3)>
);
```

The underlying truth is known as *reference collapsing*, a topic to which Item 28 is dedicated.

> [Reference collapsing](https://en.cppreference.com/w/cpp/language/reference): It is permitted to form references to references through type manipulations in templates or typedefs, in which case the reference collapsing rules apply: rvalue reference to rvalue reference collapses to rvalue reference, all other combinations form lvalue reference:
