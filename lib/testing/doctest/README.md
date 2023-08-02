# Using the `doctest` Conan Package with CMake

[This](https://conan.io/center/recipes/doctest?version=2.4.11) is a simple CMake project layout using this library:

```bash
doctest $ ls
CMakeLists.txt   conanfile.txt

doctest $ ls src
main.cpp
```

`conanfile.txt`:

```
[requires]
doctest/2.4.11

[generators]
CMakeDeps
CMakeToolchain
```

`CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.26)
project(test CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(doctest REQUIRED)

add_executable(example src/main.cpp)
target_link_libraries(example doctest::doctest)
```

`main.cpp`:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

int factorial1(int number) {
  return number <= 1
    ? number
    : factorial1(number - 1) * number;
}

TEST_CASE("testing the factorial function") {
  CHECK(factorial1(0) == 1);
  CHECK(factorial1(1) == 1);
  CHECK(factorial1(2) == 2);
  CHECK(factorial1(3) == 6);
  CHECK(factorial1(10) == 3628800);
}

int factorial2(int number) {
  return number > 1
    ? factorial2(number - 1) * number
    : 1;
}

TEST_CASE("testing the factorial function") {
  CHECK(factorial2(0) == 1);
  CHECK(factorial2(1) == 1);
  CHECK(factorial2(2) == 2);
  CHECK(factorial2(3) == 6);
  CHECK(factorial2(10) == 3628800);
}
```
Install `doctest` and generate the files that CMake needs to find this library:

```bash
doctest $ ~/venv/conan2/bin/activate

(conan2) doctest $ conan install . \
  --output-folder=build --build=missing

(conan2) doctest $ cd build
(conan2) build $ cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release
```

In the Advanced section of the *C/C++: Edit Configurations (UI)*, you can supply the path to your `compile_commands.json` and the extension will use the compilation information listed in that file to configure IntelliSense.

```json
"compileCommands": "${workspaceFolder}/build/compile_commands.json"
```

Build this project:

```bash
(conan2) build $ cmake --build .

(conan2) build $ deactivate

build $ source conanrun.sh
build $ ./example
```
