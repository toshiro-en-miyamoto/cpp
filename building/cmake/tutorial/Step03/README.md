# Usage Requirements

In this step, we will learn the following topics:

- Usage requirements

and the following `cmake` commands:

- `target_compile_definitions()`
- `target_compile_options()`
- `target_include_directories()`
- `target_link_directories()`
- `target_link_options()`
- `target_precompile_headers()`
- `target_sources()`

## Exercise 1 - Adding Usage Requirements for a Library

In this exercise, we will refactor our code from `Adding a Library` to use the modern CMake approach. We will let our library define its own usage requirements so they are passed transitively to other targets as necessary. In this case, `MathFunctions` will specify any needed include directories itself. Then, the consuming target `Tutorial` simply needs to link to `MathFunctions` and not worry about any additional include directories.

We want to state that anybody linking to `MathFunctions` needs to include the current source directory, while `MathFunctions` itself doesn't. This can be expressed with an `INTERFACE` usage requirement. Remember `INTERFACE` means things that consumers require but the producer doesn't.

```cmake
# MathFunctions/CMakeLists.txt
add_library(MathFunctions mysqrt.cxx)
target_include_directories(MathFunctions
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
```

Now that we've specified usage requirements for `MathFunctions` we can safely remove our uses of the `MathFunctions` directory from the top-level `CMakeLists.txt`:

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
    "${PROJECT_SOURCE_DIR}/MathFunctions"
)
```

after:

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


