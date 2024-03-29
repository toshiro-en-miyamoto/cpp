# Boost-ext μ(micro)/Unit Testing Framework

[UT / μt](https://github.com/boost-ext/ut) is a C++ single header/single module, macro-free μ(micro)/Unit Testing Framework.

# CMake Integration

## Directory structure

We have the following CMake project to check if CMake and UT work together. The top-level directory is represented as `./`.

```
./:
    CMakeLists.txt
    .gitignore
    main/
    test/
    .vscode/

./main/:
    CMakeLists.txt
    include/
    zzz/

./main/include/:
    hello.h

./main/zzz/:
    CMakeLists.txt
    hello.cpp

./test/:
    CMakeLists.txt
    boost/
    zzz/

./test/boost/:
    ut.hpp

./test/zzz/:
    CMakeLists.txt
    hello_test.cpp

./.vscode:
    c_cpp_properties.json
```

## Top-level directory

```
./:
    CMakeLists.txt
    .gitignore
    main/
    test/
    .vscode/
```

The top-level `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.23)
project(microTestTutorial CXX)

add_library(cxx_std INTERFACE)
target_compile_features(cxx_std INTERFACE cxx_std_20)

add_subdirectory(main)
add_subdirectory(test)
```

With CMake version 3.23 or above, `target_sources()` allows to add a [file set](https://cmake.org/cmake/help/latest/command/target_sources.html#file-sets), which will be used in `./test/CMakeLists.txt`.

UT requires C++20. The `add_library(INTERFACE)` and `target_compile_features()` commands tells CMake to compile all C++ programs in the project with the C++20 Standard.

## `main` directories

```
./main/:
    CMakeLists.txt
    include/
    zzz/

./main/include/:
    hello.h

./main/zzz/:
    CMakeLists.txt
    hello.cpp
```

`main/CMakeLists.txt` adds the `zzz` directory. It is named so simply because the code in there is to check if UT and CMake work together.

```cmake
add_subdirectory(zzz)
```

`main/zzz/hello.cpp` implements the code to be tested with UT.

```cpp
#include "hello.h"

namespace my {

std::string greeting(const std::string& who)
{
    const std::string greeting = "Hello " + who;
    return greeting;
}

} // end of namespace
```

`include/hello.h` exports the interface of `hello.cpp`. We assume that all header files that export interface are placed in the `./main/include/` directory.

```cpp
#pragma once
#include <string>

namespace my {

std::string greeting(const std::string& who);

} // end of namespace
```

With the `h` and `cpp` files, `main/zzz/CMakeLists.txt` looks like:

```cmake
add_library(hello hello.cpp)
target_include_directories(hello
    PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)
```

## `boost/ut.hpp` - the UT single header

```
./test/boost/:
    ut.hpp
```

Although UT is available in the form of C++20 Module, we use the single header of UT for easy integration. Download the [latest header](https://github.com/boost-ext/ut/blob/master/include/boost/ut.hpp) and place it in the `./test/boost` directory.

## `test` directories

```
./test/:
    CMakeLists.txt
    boost/
    zzz/

./test/zzz/:
    CMakeLists.txt
    hello_test.cpp
```

`./test/CMakeLists.txt`:

```cmake
add_library(ut INTERFACE)
target_sources(ut INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(zzz)
```

[A primary use case for INTERFACE library](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries)  is header-only libraries. Since CMake 3.23, header files may be associated with a library by adding them to a header set using the `target_sources()` command.

`test/zzz/CMakeLists.txt` builds the executable `hello_ut` as follows:

```cmake
add_executable(hello_ut hello_test.cpp)
target_link_libraries(hello_ut
    PUBLIC hello ut cxx_std
)
```

With the INTERFACE library `ut` to be linked with the `hello_ut` target, `test/zzz/hello_test.cpp` can include `<boost/ut.hpp>`:

```c++
#include "hello.h"
#include <boost/ut.hpp>

int main()
{
    using namespace boost::ut;

    "Hello"_test = [] {
        auto actual = my::greeting("world");
        expect("Hello world" == actual);
    };
}
```

## Configure, Build and Run

Although we have not added `hello_ut` as CMake tests, we can run the executable manually.

At the top-level directory, run `cmake` to configure the project with
- the `<top-level>` directory as the source directory root and
- the `<top-level>/build` as the build directory:

```bash
<top-level> $ cmake -B build
-- The CXX compiler identification is GNU 10.2.1
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /<top-level>/build
```

Then run CMake to build:

```bash
<top-level> $ cmake --build build
[ 25%] Building CXX object bin/zzz/CMakeFiles/hello.dir/hello.cpp.o
[ 50%] Linking CXX static library libhello.a
[ 50%] Built target hello
[ 75%] Building CXX object test/zzz/CMakeFiles/hello_ut.dir/hello_test.cpp.o
[100%] Linking CXX executable hello_ut
[100%] Built target hello_ut
```

Now that `libhello.a` and `hello_ut` have been built, you can run the test code with UT:

```bash
<top-level> $ build/test/zzz/hello_ut
All tests passed (1 asserts in 1 tests)
```

## Add tests to CTest

Add [`enable_testing()`](https://cmake.org/cmake/help/latest/command/enable_testing.html) command in the top-level `CMakeLists.txt`, which enables testing for this directory and below.

```cmake
cmake_minimum_required(VERSION 3.23)
project(microTestTutorial CXX)
enable_testing()

add_library(cxx_std INTERFACE)
target_compile_features(cxx_std INTERFACE cxx_std_20)

add_subdirectory(main)
add_subdirectory(test)
```

This command should be in the source directory root (`<top-level>`) because `ctest` expects to find a test file in the build directory root (`<top-level>/build`).

Then `test/zzz/CMakeLists.txt` file can add executables as tests:

```cmake
add_executable(hello_ut hello_test.cpp)
target_link_libraries(hello_ut
    PUBLIC hello ut cxx_std
)

add_test(NAME Hello_UT COMMAND hello_ut)
```

Let's run the tests with CTest:

```bash
<top-level> $ cmake -B build
-- The CXX compiler identification is GNU 10.2.1
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: <top-level>/build

<top-level> $ cmake --build build
[ 25%] Building CXX object bin/zzz/CMakeFiles/hello.dir/hello.cpp.o
[ 50%] Linking CXX static library libhello.a
[ 50%] Built target hello
[ 75%] Building CXX object test/zzz/CMakeFiles/hello_ut.dir/hello_test.cpp.o
[100%] Linking CXX executable hello_ut
[100%] Built target hello_ut

<top-level> $ cd build

build $ ctest
Test project <top-level>/build
    Start 1: Hello_UT
1/1 Test #1: Hello_UT .........................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.02 sec
```

You have to move to `build` directory to run `ctest` because again, `ctest` expects to find a test file in the build directory root (`<top-level>/build`).

CTest provides the [Build and Test mode](https://cmake.org/cmake/help/latest/manual/ctest.1.html#build-and-test-mode), which is to configure (i.e. run cmake on), build, and/or execute a test:

```
ctest --build-and-test <path-to-source> <path-to-build>
    --build-generator <generator>
    [<options>...]
    [--build-options <opts>...]
    [--test-command <command> [<args>...]]
```

The arguments to this command line are the source and binary directories. The `--build-generator` option must be provided to use `--build-and-test`. If `--test-command` is specified then that will be run after the build is complete.

```bash
<top-level> $ ctest --build-and-test ./ ./build \
    --build-generator "Unix Makefiles" \
    --test-command ctest \
    --output-on-failure

Internal cmake changing into directory: <top-level>/build
======== CMake output     ======
The CXX compiler identification is GNU 10.2.1
Detecting CXX compiler ABI info
Detecting CXX compiler ABI info - done
Check for working CXX compiler: /usr/bin/c++ - skipped
Detecting CXX compile features
Detecting CXX compile features - done
Configuring done
Generating done
Build files have been written to: <top-level>/build
======== End CMake output ======
Change Dir: <top-level>/build

Run Clean Command:/usr/bin/gmake -f Makefile clean

Run Build Command(s):/usr/bin/gmake -f Makefile && [ 25%] Building CXX object bin/zzz/CMakeFiles/hello.dir/hello.cpp.o
[ 50%] Linking CXX static library libhello.a
[ 50%] Built target hello
[ 75%] Building CXX object test/zzz/CMakeFiles/hello_ut.dir/hello_test.cpp.o
[100%] Linking CXX executable hello_ut
[100%] Built target hello_ut

Running test command: "/usr/local/cmake-3.23.4/bin/ctest" "--output-on-failure"
Test project <top-level>/build
    Start 1: Hello_UT
1/1 Test #1: Hello_UT .........................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```

## Integrating Git and VS Code

### `.gitignore`

It is a basic `.gitignore` template created by VS Code CMake Tools extension:

```
build/
build_debug/
build_release/

Makefile
CMakeFiles/

# Generated and user files
**/CMakeCache.txt
**/CMakeUserPresets.json
**/CTestTestfile.cmake
**/CPackConfig.cmake
**/cmake_install.cmake
**/install_manifest.txt
**/compile_commands.json
```

Unlike `.gitignore` for C/C++ projects, our `.gitignore` does not include binary files produced by compiler or linker such as libraries (`.o` or `a`) and executables. CMake can be configured to place such binrary files in a so-called *build* directory. Our top-level `CMakeLists.txt` specifies `.build/` as the build directory, which is included at the beginning of our `.gitignore`.

### `~/.vscode/c_cpp_properties.json`

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/main/include",
                "${workspaceFolder}/test"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "gnu17",
            "cppStandard": "c++20",
            "intelliSenseMode": "linux-gcc-arm64"
        }
    ],
    "version": 4
}
```

In the `includePath`, we are assuming that

- all header files that export interface are placed in `${workspaceFolder}/main/include` directory, and
- `boost/ut.hpp` is located at `${workspaceFolder}/test` directory.

UT requires C++20, so does the `cppStandard`.

### `~/.vscode/settings.json`

Once the project is configured with VS Code CMake Tools extension, the following file is created:

```json
{
    "cmake.sourceDirectory": "${workspaceFolder}/."
}
```

Then you can work with CMake via the Status bar:

![Status bar](statusbar.png)

- build the project
- debug an executable
- run executables
- run CTest tests

# UT Tutorial

## Step 0: Get it

Get the latest header from [here](https://github.com/boost-ext/ut/blob/master/include/boost/ut.hpp), and place it in `test/include` directory of a project, such as:

```
./test:
./test/include:
./test/include/boost:
    ut.hpp
```

Note that the directory `.` in this document represents the root direcory of the `UT-Tutorial` project.

### Integrating VS Code and Git

Before writing your test code, `c_cpp_properties.json` and `tasks.json` in `.vscode` directory will help you write your test code.

```
./.vscode:
    c_cpp_properties.json
    tasks.json
./test:
./test/include:
./test/include/boost:
    ut.hpp
```

The `includePath` value in the `c_cpp_properties.json` file let C++ IntelliSense know where to search for the `ut.hpp`:

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/test/include"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "gnu17",
            "cppStandard": "c++20",
            "intelliSenseMode": "linux-gcc-arm64"
        }
    ],
    "version": 4
}
```

Note that as UT Framework requires C++ 20, the `cppStandard` value must be `c++20` for IntelliSense to work with UT.

The `tasks.json`

```json
{
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: g++ build active file",
            "command": "/usr/bin/g++",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "-std=c++20",
                "-I${workspaceFolder}/test/include",
                "${file}",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}.out"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "Task generated by Debugger."
        }
    ],
    "version": "2.0.0"
}
```

The `tasks.json` is helpful when you build and run your test code with VS Code. Notice that the two values in the `args` array:

- `-std=c++20`
- `-I${workspaceFolder}/test/include`

At this point, `.gitignore` in the project root should be:

```
##### VisualStudioCode
.vscode/*
!.vscode/settings.json
!.vscode/tasks.json
!.vscode/launch.json
!.vscode/extensions.json
*.code-workspace

# My preference
!.vscode/c_cpp_properties.json

# Local History for Visual Studio Code
.history/

##### C++
# Prerequisites
*.d

# Compiled Object files
*.slo
*.lo
*.o
*.obj

# Precompiled Headers
*.gch
*.pch

# Compiled Dynamic libraries
*.so
*.dylib
*.dll

# Fortran module files
*.mod
*.smod

# Compiled Static libraries
*.lai
*.la
*.a
*.lib

# Executables
*.exe
*.out
*.app

# C/C++ binary extension file
*.bin
```

## Step 1: Expect it

Let's write our first test code.

```
./.vscode:
    c_cpp_properties.json
    tasks.json
./test:
./test/include:
./test/include/boost:
    ut.hpp
./test/tutorial:
    step01.cpp
```

The assersions in the `step01.cpp` are:

```cpp
#include <boost/ut.hpp>

int main()
{
    using namespace boost::ut;

    expect(1_i == 2);       // UDL syntax
    expect(1 == 2_i);       // UDL syntax
    expect(that % 1 == 2);  // Matcher syntax
    expect(eq(1, 2));       // eq/neq/gt/ge/lt/le

    expect(42l == 42_l and 1 == 2_i); // compound expression
}
```

Then run the test code with VS Code `Run C/C++ File` task:

```
  step01.cpp:7:FAILED [1 == 2]
  step01.cpp:8:FAILED [1 == 2]
  step01.cpp:9:FAILED [1 == 2]
  step01.cpp:10:FAILED [1 == 2]
  step01.cpp:12:FAILED [(42 == 42 and 1 == 2)]
================================================
tests:   0 | 0 failed
asserts: 5 | 0 passed | 5 failed
```

Notice that the build process runned by the above task creates the executable `step01.out`:

```
./.vscode:
    c_cpp_properties.json
    tasks.json
./test:
./test/include:
./test/include/boost:
    ut.hpp
./test/tutorial:
    step01.cpp
    step01.out
```

This is because the `args` value of the above-mentioned `tasks.json` tells the C++ Compiler to do so:

- `"-o",`
- `"${fileDirname}/${fileBasenameNoExtension}.out"`

### Integrating CMake

Let's integrate CMake and UT.

```
.:
    CMakeLists.txt
./.vscode:
    c_cpp_properties.json
    tasks.json
./test:
    CMakeLists.txt
./test/include:
./test/include/boost:
    ut.hpp
./test/tutorial:
    CMakeLists.txt
    step01.cpp
```

The project `CMakeLists.txt`:

```cmake
# ./CMakeLists.txt
cmake_minimum_required(VERSION 3.23)
project(microTestTutorial CXX)
add_subdirectory(test)
```

The Test `CMakeLists.txt`:

```cmake
enable_testing()
add_subdirectory(tutorial)
```

The Test/Tutorial `CMakeLists.txt`:

```cmake
add_executable(step01 step01.cpp)
target_compile_features(    step01 PRIVATE cxx_std_20)
target_include_directories( step01 PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
)
add_test(Name Step-01 Command step01)
```

