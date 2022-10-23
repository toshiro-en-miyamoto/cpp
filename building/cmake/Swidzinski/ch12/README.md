# A Professional Project

## The root directory

```bash
$ find . -maxdepth 1 -type f
./README.md
./CMakeLists.txt
```

The `/CMakeLists.txt` file:

```cmake
cmake_minimum_required(VERSION 3.20.0)
project(Calc VERSION 1.0.0 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(src bin)
add_subdirectory(test)
```

The `bin` value in `add_subdirectory(src bin)` states that we'd like the binary output of `src` subdirectories to be placed in `<build_tree>/bin`. Otherwise, binary files would end up in `<build_tree>/src`. (§8)


## `src` directory

```bash
$ find src -maxdepth 1 -type f
src/CMakeLists.txt
```

The `/src/CMakeLists.txt` file:

```cmake
add_subdirectory(calc)
```

### `src/calc` directory

```bash
$ find src/calc -type f
src/calc/CMakeLists.txt
src/calc/calc.cpp
src/calc/include/calc/calc.h
```

The `src/calc/CMakeLists.txt` file:

```cmake
add_library(calc_obj OBJECT calc.cpp)
target_include_directories(calc_obj INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

add_library(calc_static STATIC)
target_link_libraries(calc_static calc_obj)
```

### `add_library OBJECT` command

```
add_library(<name> OBJECT [<source>...])
```

The [`add_library`](https://cmake.org/cmake/help/latest/command/add_library.html#id3) command creates an *Object Library*. An object library compiles source files but does not archive or link their object files into a library. Instead other targets created by `add_library()` or `add_executable()` may reference the objects using an expression of the form `$<TARGET_OBJECTS:objlib>` as a source, where `objlib` is the object library name.

In our project,

```cmake
add_library(calc_obj OBJECT calc.cpp)
```

### `target_include_directories` command

```
target_include_directories(<target> [SYSTEM] [AFTER|BEFORE]
    <INTERFACE|PUBLIC|PRIVATE> [items1...]
    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

The [`target_include_directories`](https://cmake.org/cmake/help/latest/command/target_include_directories.html) command specifies include directories to use when compiling a given target. The named `<target>` must have been created by a command such as `add_executable()` or `add_library()`.

The [`$<BUILD_INTERFACE:...>`](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#id29) generator expression produces the content of `...` when the target is used by another target in the same buildsystem.

The [`${CMAKE_CURRENT_SOURCE_DIR}`](https://cmake.org/cmake/help/latest/variable/CMAKE_CURRENT_SOURCE_DIR.html) CMake variable is the full path to the source directory that is currently being processed by `cmake`. (Also see [`CMAKE_BINARY_DIR`](https://cmake.org/cmake/help/latest/variable/CMAKE_BINARY_DIR.html), [`CMAKE_SOURCE_DIR`](https://cmake.org/cmake/help/latest/variable/CMAKE_SOURCE_DIR.html), and [`CMAKE_CURRENT_BINARY_DIR`](https://cmake.org/cmake/help/latest/variable/CMAKE_CURRENT_BINARY_DIR.html) variables.)

In our project,

```cmake
target_include_directories(calc_obj INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
```

### `add_library (Normal Library)` command

```
add_library(calc_static STATIC)
target_link_libraries(calc_static calc_obj)

add_library(<name> [STATIC | SHARED | MODULE]
    [EXCLUDE_FROM_ALL]
    [<source>...])
```

The [`add_library`](https://cmake.org/cmake/help/latest/command/add_library.html) command for a normal library adds a library target called `<name>` to be built from the source files listed in the command invocation. The `<name>` corresponds to the logical target name and must be globally unique within a project. The actual file name of the library built is constructed based on conventions of the native platform (such as `lib<name>.a` or `<name>.lib`).

By default the library file will be created in the build tree directory corresponding to the source tree directory in which the command was invoked.

The [`target_link_libraries`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html) command specify libraries or flags to use when linking a given target and/or its dependents.

- The named `<target>` must have been created by a command such as `add_executable()` or `add_library()`. 
- When `<item>` is a library target name, the generated link line will have the full path to the linkable library file associated with the target. The buildsystem will have a dependency to re-link `<target>` if the library file changes.
- The named target must be created by `add_library()` within the project

## `test` directory

```bash
$ find test -maxdepth 1 -type f
test/CMakeLists.txt
```

The `/test/CMakeLists.txt` file:

```cmake
enable_testing()
add_subdirectory(calc)
```

The [`enable_testing`](https://cmake.org/cmake/help/latest/command/enable_testing.html) command enables testing for current directory and below. This command should be in the source directory root because ctest expects to find a test file in the build directory root.

### `test/calc` directory

```bash
$ find test/calc -type f
test/calc/CMakeLists.txt
test/calc/calc_test.cpp
```

The `/test/calc/CMakeLists.txt` file:

```cmake
add_executable(calc_test calc_test.cpp)
target_link_libraries(calc_test PRIVATE calc_static)
```

### Integrating GoogleTest with CTest

The Fetch commands fetches GoogleTest:

The `/test/CMakeLists.txt` file:

```cmake
# /test/CMakeLists.txt
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)
option(INSTALL_GMOCK "Install GMock" OFF)
option(INSTALL_GTEST "Install GTest" OFF)
FetchContent_MakeAvailable(googletest)

add_subdirectory(calc)
```

You can add GoogleTest tests as follows:

```cmake
# /test/calc/CMakeLists.txt
add_executable(calc_test calc_test.cpp)
target_link_libraries(calc_test PRIVATE calc_static)

target_link_libraries(${target} PRIVATE gtest_main gmock)
gtest_discover_tests(${target})
```

The `gtest_discover_tests` function provided by the [GoogleTest](https://cmake.org/cmake/help/latest/module/GoogleTest.html) module,

- is intended to replace use of `add_test()` to register tests, and will create a separate CTest test for each Google Test test case.
- automatically adds tests with CTest by querying the compiled test executable for available tests

For example, with the following test code, `SumAddsTwoInts` and `MultiplyMultipliesTwoInts` tests will be added with CTest.

```c++
// test/calc/calc_test.cpp
#include "calc/calc.h"
#include <gtest/gtest.h>

TEST(CalcTest, SumAddsTwoInts) {
  EXPECT_EQ(4, Calc::Sum(2, 2));
}

TEST(CalcTest, MultiplyMultipliesTwoInts) {
  EXPECT_EQ(12, Calc::Multiply(3, 4));
}
```

### `add_test` command

The [`add_test`](https://cmake.org/cmake/help/latest/command/add_test.html) CMake command adds a test to the project to be run by `ctest`.

```
add_test(NAME <name> COMMAND <command> [<arg>...]
    [CONFIGURATIONS <config>...]
    [WORKING_DIRECTORY <dir>]
    [COMMAND_EXPAND_LISTS])
```

The `COMMAND` argument specifies the test command-line. If `<command>` specifies an executable target (created by `add_executable()`) it will automatically be replaced by the location of the executable created at build time.

For a reminder, `add_test` commands are used in the chapter 8 as follows:

```cmake
# chapter08/02-structured/test/CMakeLists.txt
add_executable(unit_tests
    unit_tests.cpp
    calc_test.cpp
    run_test.cpp)
target_link_libraries(unit_tests PRIVATE sut)

add_test(NAME SumAddsTwoInts COMMAND unit_tests 1)
add_test(NAME MultiplyMultipliesTwoInts COMMAND unit_tests 2)
add_test(NAME RunOutputsCorrectEquations COMMAND unit_tests 3)
```

### Integrating Boost-ext UT with CTest

[Boost-ext UT](https://github.com/boost-ext/ut) is a C++ single header/single module, macro-free μ(micro)/Unit Testing Framework. The project provides a CMake config and target. Just load `ut` with `find_package` to import the `Boost::ut` target. Linking against this target will add the necessary include directory for the single header file.

```cmake
find_package(ut REQUIRED)
add_library(my_test my_test.cpp)
target_link_libraries(my_test PRIVATE Boost::ut)
```

# CMake Using Dependencies Guide

The [Using Dependencies Guide](https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html) provides a high-level introduction to this general topic. It provides a broader overview of where the `find_package()` command fits into the bigger picture, including its relationship to the `FetchContent` module.

## Introduction

The primary methods of bringing dependencies into the build are the `find_package()` command and the `FetchContent` module.

## Using Pre-built Packages With `find_package()`

A package needed by the project may already be built and available at some location on the user's system. That package might have also been built by CMake, or it could have used a different build system entirely. It might even just be a collection of files that didn't need to be built at all. CMake provides the `find_package()` command for these scenarios.

The `find_package()` command supports two main methods for carrying out the search:

- Config mode: With this method, the command looks for files that are typically provided by the package itself. This is the more reliable method of the two, since the package details should always be in sync with the package.
- Module mode: Not all packages are CMake-aware. Many don't provide the files needed to support config mode. For such cases, a Find module file can be provided separately, either by the project or by CMake.

### `find_package` command

The [`find_package`](https://cmake.org/cmake/help/latest/command/find_package.html) command finds a package (usually provided by something external to the project), and load its package-specific details.

The command has a few modes by which it searches for packages:
- Module mode: searches for `Find<PackageName>.cmake`
- Config mode: searches for so-called `config-file`s
- FetchContent

FetchContent redirection mode: A call to `find_package()` can be redirected internally to a package provided by the FetchContent module. To the caller, the behavior will appear similar to Config mode, except that the search logic is by-passed and the component information is not used.

The *basic signature* is supported by both Module and Config modes. The `MODULE` keyword implies that only Module mode can be used to find the package, with no fallback to Config mode.

```
find_package(<PackageName> [version] [EXACT] [QUIET] [MODULE]
    [REQUIRED] [[COMPONENTS] [components...]]
    [OPTIONAL_COMPONENTS components...]
    [REGISTRY_VIEW  (64|32|64_32|32_64|HOST|TARGET|BOTH)]
    [GLOBAL]
    [NO_POLICY_SCOPE]
    [BYPASS_PROVIDER])
```

The tables below show the directories searched.

```
Unix:
<prefix>/(lib/<arch>|lib*|share)/cmake/<name>*/
<prefix>/(lib/<arch>|lib*|share)/<name>*/
<prefix>/(lib/<arch>|lib*|share)/<name>*/(cmake|CMake)/
<prefix>/<name>*/(lib/<arch>|lib*|share)/cmake/<name>*/
<prefix>/<name>*/(lib/<arch>|lib*|share)/<name>*/
<prefix>/<name>*/(lib/<arch>|lib*|share)/<name>*/(cmake|CMake)/
```

```
macOS:
<prefix>/<name>.framework/Resources/
<prefix>/<name>.framework/Resources/CMake/
<prefix>/<name>.framework/Versions/*/Resources/
<prefix>/<name>.framework/Versions/*/Resources/CMake/
<prefix>/<name>.app/Contents/Resources/
<prefix>/<name>.app/Contents/Resources/CMake/
```

For example, with CMake 3.23 on Debian Bullseye:

```
/usr/local/cmake-3.23.4/share/cmake-3.23/Modules/:
CPack.cmake
CTest.cmake
FetchContent.cmake
FindBoost.cmake
FindGTest.cmake
FindProtobuf.cmake
```

### Config-file packages

The preferred way for a third party to provide executables, libraries, headers, and other files for use with CMake is to provide config files. These are text files shipped with the package, which define CMake targets, variables, commands, and so on. The config file is an ordinary *CMake script*, which is read in by the `find_package()` command.

The config file is the entry point to the package for CMake, named:
- `<PackageName>Config.cmake`, or
- `<LowercasePackageName>-config.cmake`

A separate optional file may also exist in the same directory, named:
- `<PackageName>ConfigVersion.cmake`, or
- `<LowercasePackageName>-config-version.cmake`

If the location of the package is in a directory known to CMake, the `find_package()` call should succeed. The directories known to CMake are platform-specific. For example, packages installed on Linux with a standard system package manager will be found in the `/usr` prefix automatically.

## Downloading And Building From Source With `FetchContent`

Dependencies do not necessarily have to be pre-built in order to use them with CMake. They can be built from sources as part of the main project. The `FetchContent` module provides functionality to download content (typically sources, but can be anything) and add it to the main project if the dependency also uses CMake. The dependency's sources will be built along with the rest of the project, just as though the sources were part of the project's own sources.

## Imported Targets

### Buildsystem

Some target types do not represent outputs of the buildsystem, but only inputs such as external dependencies, aliases or other non-build artifacts. *Pseudo targets* are not represented in the generated buildsystem.

An [IMPORTED target](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#imported-targets) represents a pre-existing dependency. Usually such targets are defined by an upstream package and should be treated as immutable. After declaring an IMPORTED target one can adjust its target properties by using the customary commands, just like with any other regular target, such as:

- `target_compile_definitions()`
- `target_include_directories()`
- `target_compile_options()`
- `target_link_libraries()`

IMPORTED targets may have the same usage requirement properties populated as binary targets, such as

- `INTERFACE_INCLUDE_DIRECTORIES`
- `INTERFACE_COMPILE_DEFINITIONS`
- `INTERFACE_COMPILE_OPTIONS`
- `INTERFACE_LINK_LIBRARIES`
- `INTERFACE_POSITION_INDEPENDENT_CODE`

### Dependencies

Both config files and Find module files can define [Imported Targets](https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html#id8). These will typically have names of the form `SomePrefix::ThingName`.

Such targets typically carry usage requirements and apply things like header search paths, compiler definitions, etc. automatically to other targets that link to them (e.g. using `target_link_libraries()`).

A complete example which finds a third party package and uses a library from it might look like the following:

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyExeProject VERSION 1.0.0)

# Make project-provided Find modules available
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(SomePackage REQUIRED)
add_executable(MyExe main.cpp)
target_link_libraries(MyExe PRIVATE SomePrefix::LibName)
```

Note that the above call to `find_package()` could be resolved by a config file or a Find module. It uses only the basic arguments supported by the Basic Signature. A `FindSomePackage.cmake` file in the `${CMAKE_CURRENT_SOURCE_DIR}/cmake` directory would allow the `find_package()` command to succeed using module mode, for example. If no such module file is present, the system would be searched for a config file.
