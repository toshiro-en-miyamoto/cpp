# Concepts

```c++
#include <string>

template<typename T>
T multiply1(T first, T second)
{
  return first * second;
}

int main()
{
  multiply1(5, 7);
  multiply1(2.3, 2.0);
  multiply1(std::string{"a"}, std::string("b"));
}
```

Although IntelliSense can't detect that `multiply1("a", "b")` doesn't satisfy the implicit requirements for the type parameter `T`, `g++` compiler emits the following error messages.

```
$ g++ --version
g++ (Debian 10.2.1-6) 10.2.1 20210110

$ g++ multiply.cpp -std=c++20
multiply.cpp: In instantiation of ‘T multiply1(T, T) [with T = std::__cxx11::basic_string<char>]’:
multiply.cpp:15:46:   required from here
multiply.cpp:8:16: error: no match for ‘operator*’ (operand types are ‘std::__cxx11::basic_string<char>’ and ‘std::__cxx11::basic_string<char>’)
    8 |   return first * second;
```

By explicitly stating the requirements with C++20 concepts, IntelliSense now can detect the preceding error.

```c++
#include <concepts>
#include <string>

template<typename T>
  requires std::integral<T> || std::floating_point<T>
T multiply1(T first, T second)
{
  return first * second;
}

int main()
{
  multiply1(5, 7);
  multiply1(2.3, 2.0);
  multiply1(std::string{"a"}, std::string("b"));
}
```

Now `g++` shows a meaningful message.

```
multiply.cpp:15:46: error: use of function ‘T multiply1(T, T)
[with T = std::__cxx11::basic_string<char>]’ with
unsatisfied constraints
   15 |   multiply1(std::string{"a"}, std::string("b"));

multiply.cpp:6:3: note: declared here
    6 | T multiply1(T first, T second)
multiply.cpp:6:3: note: constraints not satisfied

multiply.cpp:5:29: note: no operand of the disjunction is satisfied
    5 |   requires std::integral<T> || std::floating_point<T>
```

When you have a single concept-constrained type parameter, the C++ Core Guidelines recommend using the concept name in place of `typename` in the `template` header.

```c++
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<Numeric T>
T multiply2(T first, T second)
{
  return first * second;
}
```

The following code reimplements `multiply` as an **abbreviated function template**. In this case, we used *constrained* `auto` for each parameter, using our `Numeric` concept to restrict the types we can pass as arguments. We also used `auto` as the return type, so the compiler will infer it from the type of the expression.

```c++
auto multiply3(Numeric auto first, Numeric auto second)
{
  return first * second;
}
```

The preceding abbreviated function template is actually equivalent to a template with two type parameters:

```c++
template<Numeric T1, Numeric T2>
auto multiply4(T1 first, T2 second)
{
  return first * second;
}
```

The key difference between `multiply2` and `multiply3` is that the compiler treats each `auto` as a separate template type parameter. Thus, `first` and `second` can have different data types which passes an `int` and a `double`.

```c++
  multiply3(2.3, 2);
  multiply4(2.3, 2);
```

When overloaded functions perform syntactically identical operations on different types, they can be expressed more compactly and conveniently using **function templates**. You can then write function calls with different argument types and let the compiler instantiate the template appropriately for each function call. The instantiations all have the same function name, so the compiler uses overload resolution to invoke the proper one.
