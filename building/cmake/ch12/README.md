# A Professional Project

## The root directory

The `/CMakeLists.txt` file:

```cmake
cmake_minimum_required(VERSION 3.20.0)
project(Calc VERSION 1.0.0 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(src bin)
add_subdirectory(test)
```

The `bin` value in `add_subdirectory(src bin)` states that we'd like the binary output of `src` subdirectories to be placed in `<build_tree>/bin`. Otherwise, binary files would end up in `<build_tree>/src`. (§8)


## The `src` directory

The `/src/CMakeLists.txt` file:

```cmake
add_subdirectory(calc)
```

### The `src/calc` directory

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

### The `add_library OBJECT` command

```
add_library(calc_obj OBJECT calc.cpp)

add_library(<name> OBJECT [<source>...])
```

The [`add_library`](https://cmake.org/cmake/help/latest/command/add_library.html#id3) command creates an *Object Library*. An object library compiles source files but does not archive or link their object files into a library. Instead other targets created by `add_library()` or `add_executable()` may reference the objects using an expression of the form `$<TARGET_OBJECTS:objlib>` as a source, where `objlib` is the object library name.

### The `target_include_directories` command

```
target_include_directories(calc_obj INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_include_directories(<target> [SYSTEM] [AFTER|BEFORE]
    <INTERFACE|PUBLIC|PRIVATE> [items1...]
    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

The [`target_include_directories`](https://cmake.org/cmake/help/latest/command/target_include_directories.html) command specifies include directories to use when compiling a given target. The named `<target>` must have been created by a command such as `add_executable()` or `add_library()`.

The [`$<BUILD_INTERFACE:...>`](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#id29) generator expression produces the content of `...` when the target is used by another target in the same buildsystem.

The [`${CMAKE_CURRENT_SOURCE_DIR}`](https://cmake.org/cmake/help/latest/variable/CMAKE_CURRENT_SOURCE_DIR.html) CMake variable is the full path to the source directory that is currently being processed by `cmake`. (Also see [`CMAKE_BINARY_DIR`](https://cmake.org/cmake/help/latest/variable/CMAKE_BINARY_DIR.html), [`CMAKE_SOURCE_DIR`](https://cmake.org/cmake/help/latest/variable/CMAKE_SOURCE_DIR.html), and [`CMAKE_CURRENT_BINARY_DIR`](https://cmake.org/cmake/help/latest/variable/CMAKE_CURRENT_BINARY_DIR.html) variables.)

### The `add_library (Normal Library)` command

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

## The `test` directory

```cmake
enable_testing()
add_subdirectory(calc)
```

The [`enable_testing`](https://cmake.org/cmake/help/latest/command/enable_testing.html) command enables testing for current directory and below. This command should be in the source directory root because ctest expects to find a test file in the build directory root.

