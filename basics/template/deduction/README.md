# Class Template Argument Deduction (CTAD)

References:
- 15.9.4 MyArray Deduction Guide for Braced Initialization, *C++20 for Programmers*, Paul and Harvey Deitel
- [CTAD since C++17](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction) at CppReference

## User-defined deduction guides

The syntax of a user-defined deduction guide is the syntax of a function declaration with a trailing return type, except that it uses the name of a class template as the function name:

```
explicit-specifier(optional)
template-name ( parameter-declaration-clause ) -> simple-template-id ;
```

such as:

```c++
#include <concepts>

template<typename T, std::size_t SIZE>
struct array
{
  T data[SIZE];
};

template<typename T, std::same_as<T>... Us>
array(T first, Us... rest) -> array<T, 1 + sizeof...(Us)>;

int main()
{
  constexpr array a{1, 2, 3};
  static_assert(std::is_same_v<int[3], decltype(a.data)>);
}
```
