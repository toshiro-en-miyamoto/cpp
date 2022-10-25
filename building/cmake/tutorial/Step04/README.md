# Adding Generator Expressions

In this step, we will learn the following topics:

- INTERFACE libraries
- target compiler features
- how to set the C++ Standard with an INTERFACE library

and the following CMake commands:

- `add_library(name INTERFACE)`
- `target_compiler_features(name INTERFACE feature)`

## [Libraries](https://cmake.org/cmake/help/latest/command/add_library.html)

There are five kinds of libraries:

- NORMAL libraries
- OBJECT libraries
- INTERFACE libraries

A NORMAL library is to be built from the source files listed in the `add_library()` command invocation. The actual file name of the library built is constructed based on conventions of the native platform (such as `lib<name>.a` or `<name>.lib`). `STATIC`, `SHARED`, or `MODULE` may be given to specify the type of library to be created.

An OBJECT library compiles source files but does not archive or link their object into a library. Instead other targets created by `add_library()` or `add_executables()` may reference the objects using an expression of the form `$<TARGET_OBJECTS:objlib>` as a source, where `objlib` is the object library name.

An INTERFACE library target does not compile sources and does not produce a library artifact on disk. However, it may have properties set on it and it may be installed and exported. Source files to be compiled may be added later by calls to `target_sources()` with `PRIVATE` or `PUBLIC` keywords or (since version 3.19) listed directly in the `add_library()` call.

`add_library()` also creates [Pseudo targets](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#id38):

- IMPORTED libraries
- ALIAS libraries

An IMPORTED target represents a pre-existing dependency. Usually such targets are defined by an upstream package and should be treated as immutable.

An ALIAS target is a name which may be used interchangeably with a binary target name in read-only contexts. A primary use-case for ALIAS targets is for example or unit test executables accompanying a library, which may be part of the same buildsystem or built separately based on user configuration.

## A use-case for INTERFACE libraries

[A primary use-case for INTERFACE libraries](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#id41) is header-only libraries. Since CMake 3.23, header files may be associated with a library by adding them to a header set using the `target_sources()` command:

```cmake
add_library(Eigen INTERFACE)

target_sources(Eigen INTERFACE
    FILE_SET HEADERS
    BASE_DIRS src
    FILES src/eigen.h src/vector.h src/matrix.h
)

add_executable(exe1 exe1.cpp)
target_link_libraries(exe1 Eigen)
```

When we specify the `FILE_SET` here, the `BASE_DIRS` we define automatically become include directories in the usage requirements for the target `Eigen`. The usage requirements from the target are consumed and used when compiling, but have no effect on linking.

## Exercise 1 - Setting the C++ Standard with Interface Libraries

The two `set()` calls on the variables `CMAKE_CXX_STANDARD` and `CMAKE_CXX_STANDARD_REQUIRED` are no longer required.

```cmake
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

Instead, we need to create an INTERFACE library, `compiler_flags`. And then use `target_compile_features()` to add the compiler feature `cxx_std_11`.

```cmake
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_11)
```

Thus, the top-level `CMakeLists.txt` has been updated as follows,

before:

```cmake
cmake_minimum_required(VERSION 3.10)
project(Tutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(TutorialConfig.h.in TutorialConfig.h)

add_subdirectory(MathFunctions)

add_executable(Tutorial tutorial.cxx)
target_link_libraries(Tutorial PUBLIC MathFunctions)
target_include_directories(Tutorial PUBLIC
    "${PROJECT_BINARY_DIR}"
)
```

after:

```cmake
cmake_minimum_required(VERSION 3.15)
project(Tutorial VERSION 1.0)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_11)

configure_file(TutorialConfig.h.in TutorialConfig.h)

add_subdirectory(MathFunctions)

add_executable(Tutorial tutorial.cxx)
target_link_libraries(Tutorial PUBLIC
    MathFunctions
    compiler_flags
)
target_include_directories(Tutorial PUBLIC
    "${PROJECT_BINARY_DIR}"
)
```

Also the `MathFunctions/CMakeLists.txt` has been updated as follows,

before:

```cmake
add_library(MathFunctions mysqrt.cxx)

target_include_directories(MathFunctions INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(MathFunctions compiler_flags)
```
