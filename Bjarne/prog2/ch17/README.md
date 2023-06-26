# Vector and Free Store

## Linking template functions

Suppose we have a template function declaration:

```cpp
// ./main/include/ch17/type_size.h
#include <cstddef>
#include <utility>

using size_pair = std::pair<std::size_t, std::size_t>;
template<typename T> size_pair type_size(T val);
```

and its implementation:

```cpp
// ./main/ch17/type_size.cpp
template<typename T>
size_pair type_size(T val)
{
  return std::make_pair(sizeof(T), sizeof(val));
}
```

then compile it as a static library:

```cmake
# ./main/ch17/CMakeLists.txt
add_library(type_size type_size.cpp)
target_include_directories(type_size
  PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)
```

We want to call from an executable:

```cpp
// ./test/ch17/type_size_test.cpp
#include <ch17/type_size.h>
#include <boost/ut.hpp>

int main()
{
  using namespace boost::ut;

  "size of char"_test = [] {
    auto [type_size, value_size] = type_size<char>('a');
    expect(type_size == 1_i);
    expect(value_size == 1_i);
  };
}
```

so need to link with the library:

```cmake
# ./test/ch17/CMakeLists.txt
add_executable(type_size_ut type_size_test.cpp)
target_link_libraries(type_size_ut
  PUBLIC type_size ut cxx_std
)

add_test(NAME type_size_UT COMMAND type_size_ut)
```

but linking fails:

```bash
$ cmake --build build
[ 25%] Building CXX object main/ch17/CMakeFiles/type_size.dir/type_size.cpp.o
[ 50%] Linking CXX static library lib type_size.a
[ 50%] Built target type_size
[ 75%] Building CXX object test/ch17/CMakeFiles/type_size_ut.dir/type_size_test.cpp.o
[100%] Linking CXX executable type_size_ut
/usr/bin/ld: CMakeFiles/type_size_ut.dir/type_size_test.cpp.o: in function `main::{lambda()#1}::operator()() const':
type_size_test.cpp:(.text+0x10): undefined reference to `std::pair<unsigned long, unsigned long> const my::type_size<char>(char)'
```

A function template by itself is not a type, or a function, or any other entity. No code is generated from a source file that contains only template definitions. In order for any code to appear, a template must be instantiated: the template arguments must be determined so that the compiler can generate an actual function (or class, from a class template). So [explicit function template instantiation](https://en.cppreference.com/w/cpp/language/function_template#Function_template_instantiation) comes in.

An explicit instantiation definition forces instantiation of the function they refer to. It may appear in the program anywhere after the template definition, and for a given argument-list, is only allowed to appear once in the program:

```cpp
// ./main/ch17/type_size.cpp
template<typename T>
size_pair type_size(T val)
{
  return std::make_pair(sizeof(T), sizeof(val));
}

template size_pair type_size<char>(char);
```

An explicit instantiation declaration (an *extern template*) prevents implicit instantiations: the code that would otherwise cause an implicit instantiation has to use the explicit instantiation definition provided somewhere else in the program. (since C++ 11)

```cpp
// ./main/include/ch17/type_size.h
#include <cstddef>
#include <utility>

using size_pair = std::pair<std::size_t, std::size_t>;
template<typename T> size_pair type_size(T val);

extern template size_pair type_size<char>(char);
```

Now the build succeeds:

```bash
$ cmake --build build
[ 25%] Building CXX object main/ch17/CMakeFiles/type_size.dir/type_size.cpp.o
[ 50%] Linking CXX static library lib type_size.a
[ 50%] Built target type_size
[ 75%] Building CXX object test/ch17/CMakeFiles/type_size_ut.dir/type_size_test.cpp.o
[100%] Linking CXX executable type_size_ut
[100%] Built target type_size_ut
```

## 17.3.1 The sizeof operator

```cpp
#include <ch17/type_size.h>
#include <boost/ut.hpp>

int main()
{
  using namespace boost::ut;

  "size of char"_test = [] {
    auto [type_size, value_size] = my::type_size<char>('a');
    expect(type_size == 1_i);
    expect(value_size == 1_i);
  };
  "size of int"_test = [] {
    auto [type_size, value_size] = my::type_size<int>(1);
    expect(type_size == 4_i);
    expect(value_size == 4_i);
  };
  "size of long"_test = [] {
    auto [type_size, value_size] = my::type_size<long>(1L);
    expect(type_size == 8_i);
    expect(value_size == 8_i);
  };
  "size of pointer to int"_test = [] {
    int i = 1;
    int* pi = &i;
    auto [type_size, value_size] = my::type_size<int*>(pi);
    expect(type_size == 8_i);
    expect(value_size == 8_i);
  };
  "size of bool"_test = [] {
    auto [type_size, value_size] = my::type_size<bool>(true);
    expect(type_size == 1_i);
    expect(value_size == 1_i);
  };
  "size of double"_test = [] {
    auto [type_size, value_size] = my::type_size<double>(1);
    expect(type_size == 8_i);
    expect(value_size == 8_i);
  };
}
```
