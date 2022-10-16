# Linking with CMake

## The basic of linking

## Linking types

| kind   | Unix  | Windows
|--------|-------|---------
| static | `.a`  | `.lib`
| shared | `.so` | `.dll`

We can create these libraries with a simple `add_library()` command (which is consumed with the `target_link_libraries()` command).

```cmake
cmake_minimum_required(VERSION 3.20.0)
project(Libraries CXX)

add_library(my_static STATIC function_a.cpp function_b.cpp)
add_library(my_shared SHARED function_a.cpp function_b.cpp)
add_library(my_module MODULE function_a.cpp function_b.cpp)
```

When building libraries (static, shared, or shared modules), you'll often encounter the name *linking* for this process.

```bash
$ cmake --build build/
[ 11%] Building CXX object CMakeFiles/my_static.dir/function_a.cpp.o
[ 22%] Building CXX object CMakeFiles/my_static.dir/function_b.cpp.o
[ 33%] Linking CXX static library libmy_static.a
[ 33%] Built target my_static
[ 44%] Building CXX object CMakeFiles/my_shared.dir/function_a.cpp.o
[ 55%] Building CXX object CMakeFiles/my_shared.dir/function_b.cpp.o
[ 66%] Linking CXX shared library libmy_shared.so
[ 66%] Built target my_shared
[ 77%] Building CXX object CMakeFiles/my_module.dir/function_a.cpp.o
[ 88%] Building CXX object CMakeFiles/my_module.dir/function_b.cpp.o
[100%] Linking CXX shared module libmy_module.so
[100%] Built target my_module
```

A shared module isn't loaded automatically with the start of the program (like regular shared libraries). This only happens when a program explicitly requests it by making a system call.

## The One Definition Rule

## Unresolved symbols

## Separating `main()` for testing

Consider extracting all logic from your program's `main()` to an external function, `start_program()`, like so:

```c++
// main.cpp
extern int start_program(int, const char**);
int main(int argc, const char** argv) {
  return start_program(argc, argv);
}
```

We can then create a library containing the original source from `main()` wrapped in a new function – `start_program()`.

```c++
// program.cpp
#include <iostream>
int start_program(int argc, const char** argv) {
  if (argc <= 1) {
    std::cout << "Not enough arguments" << std::endl;
    return 1;
  }
  return 0;
}
```

We can now prepare a project that builds this application and links together those two translation units:

```cmake
cmake_minimum_required(VERSION 3.20.0)
project(Testing CXX)

add_library(program program.cpp)

add_executable(main main.cpp)
target_link_libraries(main program)

add_executable(test test.cpp)
target_link_libraries(test program)
```

The important thing is that we have now defined `main()` twice:

- In `main.cpp` for production use
- In `test.cpp` for test purposes

```c++
// test.cpp
#include <iostream>
extern int start_program(int, const char**);
using namespace std;
int main() {
  auto exit_code = start_program(0, nullptr);
  if (exit_code == 0)
    cout << "Non-zero exit code expected" << endl;

  const char* arguments[2] = {"hello", "world"};
  exit_code = start_program(2, arguments);
  if (exit_code != 0)
    cout << "Zero exit code expected" << endl;
}
```
