# Adding a Library

In this step, we will learn the following topics:

- organizing source files in subdirectories
- adding a library
- give an executable access to header files of the library
- linking the library and executable targets

and the following `cmake` commands:

- `add_subdirectory()`
- `add_library()`
- `target_include_directories()`
- `target_link_libraries()`

## Exercise 1 - Creating a Library

To add a library in CMake, use the `add_library()` command and specify which source files should make up the library.

Rather than placing all of the source files in one directory, we can organize our project with one or more subdirectories. In this case, we will create a subdirectory specifically for our library. Here, we can add a new `CMakeLists.txt` file and one or more source files. In the top level `CMakeLists.txt` file, we will use the `add_subdirectory()` command to add the subdirectory to the build.

Once the library is created, it is connected to our executable target with `target_include_directories()` and `target_link_libraries()`.


We will put the library into a subdirectory called `MathFunctions`. This directory already contains a header file, `MathFunctions.h`, and a source file `mysqrt.cxx`.

```
tutorial/Step02/MathFunctions:
    MathFunctions.h
    CMakeLists.txt
    mysqrt.cxx
tutorial/Step02:
    TutorialConfig.h.in
    CMakeLists.txt
    tutorial.cxx
```

In the `CMakeLists.txt` file in the `MathFunctions` directory, we create a library target called `MathFunctions` with `add_library()`. The source file for the library is passed as an argument to `add_library()`.

```cmake
add_library(MathFunctions mysqrt.cxx)
```

To make use of the new library,
- we will add an `add_subdirectory()` call in the top-level `CMakeLists.txt` file so that the library will get built.
- the new library target is linked to the executable target using `target_link_libraries()`.
- we need to specify the library's header file location. Modify `target_include_directories()` to add the `MathFunctions` subdirectory as an include directory so that the `MathFunctions.h` header file can be found.

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
    "${PROJECT_SOURCE_DIR}/MathFunctions"
)
```
