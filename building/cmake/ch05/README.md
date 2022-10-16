# Compiling C++ Sources with CMake

## Compilation

The compiler has to execute the following stages to create an object file:

- Preprocessing
- Linguistic analysis
- Assembly
- Optimization
- Code emission

CMake offers multiple commands to affect each stage:

- [`target_compile_features()`](https://cmake.org/cmake/help/latest/command/target_compile_features.html): Add expected compiler features to a target.
- [`target_sources()`](https://cmake.org/cmake/help/latest/command/target_sources.html): Add sources to an already defined target.
- [`target_include_directories()`](https://cmake.org/cmake/help/latest/command/target_include_directories.html): Set up the preprocessor *include paths*.
- [`target_compile_definitions()`](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html): Set up preprocessor definitions.
- [`target_compile_options()`](https://cmake.org/cmake/help/latest/command/target_compile_options.html): Compiler-specific options for the command line.
- [`target_precompile_headers()`](https://cmake.org/cmake/help/latest/command/target_precompile_headers.html): Add a list of header files to precompile.

All of the preceding commands accept similar arguments:

```cmake
target_xxx(<target name> <scope> <value>)

# scope: INTERFACE | PUBLIC | PRIVATE
```

This means that they support *property propagation*.

### [Compile Features](https://cmake.org/cmake/help/latest/manual/cmake-compile-features.7.html):

Project source code may depend on, or be conditional on, the availability of certain features of the compiler.

While features are typically specified in programming language standards, CMake provides a primary user interface based on granular handling of the features, not the language standard that introduced the feature.

- The `CMAKE_CXX_KNOWN_FEATURES` global property contains all the features known to CMake, regardless of compiler support for the feature.
- The `CMAKE_CXX_COMPILE_FEATURES` variable contains all features CMake knows are known to the compiler, regardless of language standard or compile flags needed to use them.

Compile feature requirements may be specified with the `target_compile_features()` command. For example, if a target must be compiled with compiler support for the `cxx_constexpr` feature:

```cmake
add_library(mylib mylib.cpp)
target_compile_features(mylib PRIVATE cxx_constexpr)
```

This will ensure that the in-use C++ compiler is capable of the feature, and will add any necessary flags such as `-std=gnu++11` to the compile lines of C++ files in the `mylib` target.

In the above example, `mylib` requires `cxx_constexpr` when it is built itself, but consumers of `mylib` are not required to use a compiler which supports `cxx_constexpr`. If the interface of `mylib` does require the `cxx_constexpr` feature (or any other known feature), that may be specified with the `PUBLIC` or `INTERFACE` signatures of `target_compile_features()`:

```cmake
add_library(mylib mylib.cpp)
target_compile_features(mylib PUBLIC cxx_constexpr)

# cxx_constexpr is a usage-requirement of mylib.
# Dependents such as myexe will be compiled
# with -std=gnu++11 for cxx_constexpr.

add_executable(myexe main.cpp)
target_link_libraries(myexe mylib)
```

In projects that use a large number of commonly available features from a particular language standard (e.g. C++ 11) one may specify a meta-feature (e.g. `cxx_std_11`) that requires use of a compiler mode that is at minimum aware of that standard, but could be greater. This is simpler than specifying all the features individually, but does not guarantee the existence of any particular feature. Diagnosis of use of unsupported features will be delayed until compile time.

For example, if C++ 11 features are used extensively in a project's header files, then clients must use a compiler mode that is no less than C++ 11. This can be requested with the code:

```cmake
target_compile_features(mylib PUBLIC cxx_std_11)
```

In this example, CMake will ensure the compiler is invoked in a mode of at-least C++ 11 (or C++ 14, C++ 17, ...), adding flags such as `-std=gnu++11` if necessary. This applies to sources within mylib as well as any dependents (that may include headers from `mylib`).

[CMake C++ Known Features](https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html):

The features listed in the global property `CMAKE_CXX_KNOWN_FEATURES` may be known to be available to the C++ compiler. If the feature is available with the C++ compiler, it will be listed in the `CMAKE_CXX_COMPILE_FEATURES` variable.

The following meta features indicate general support for the associated language standard. It reflects the language support claimed by the compiler, but it does not necessarily imply complete conformance to that standard.

- `cxx_std_98`
- `cxx_std_11`
- `cxx_std_14`
- `cxx_std_17`
- `cxx_std_20` (since 3.12)
- `cxx_std_23` (since 3.20)

### The `target_source()` command

We can use the `target_sources()` command to append files to a previously created target:

```cmake
cmake_minimum_required(VERSION 3.20.0)
project(Sources CXX)

add_executable(main main.cpp)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(main PRIVATE gui_linux.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(main PRIVATE gui_windows.cpp)
endif()
```

## Preprocessor

The most basic feature of the preprocessor is the ability to include header files with the `#include` directive. It comes in two forms:

- `#include <path-spec>`: Angle-bracket form
- `#include "path-spec"`: Quoted form

Typically, the angle-bracket form will check standard include directories, including the directories where standard C++ library and standard C library headers are stored in the system.

The quoted form will start searching for the included file in *the directory of the current file* and then check directories for the angle-bracket form.

```cmake
target_include_directories(<target> [SYSTEM] [AFTER|BEFORE]
    <INTERFACE|PUBLIC|PRIVATE> [items1...]
    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

The `SYSTEM` keyword informs the compiler that the provided directories are meant as standard system directories (to be used with the angle-bracket form).

## The optimizer

Most compilers offer four basic levels of optimization, from 0 to 3. We specify them with the `-O<level>` option.

- `-O0` means no optimization and, usually, it's the default level for compilers.
- `-O2` is considered a full optimization, one that generates highly optimized code but at the cost of the slowest compilation time.
- `-O3`, which is full optimization, like `-O2`, but with a more aggressive approach to subprogram inlining and loop vectorization.

There's an in-between `-O1` level, which (depending on your needs) can be a good compromise – it enables a reasonable amount of optimization mechanisms without slowing the compilation too much.

CMake provides some default flags for compilers, which are stored in system-wide (not target-specific) variables for used language (`CXX` for C++) and build configuration (`DEBUG` or `RELEASE`):

- `CMAKE_CXX_FLAGS_DEBUG` equals `-g`.
- `CMAKE_CXX_FLAGS_RELEASE` equals `-O3 -DNDEBUG`.

Variables such as `CMAKE_<LANG>_FLAGS_<CONFIG>` are global – they apply to all targets. It is recommended to configure your targets through properties and commands such as t`arget_compile_options()` rather than relying on global variables.


## Compilation process
