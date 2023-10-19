# `catch2` on Debian with Conan

In [this example](https://docs.conan.io/2/examples/tools/cmake/cmake_toolchain/build_project_cmake_presets.html) we are going to see how to use `CMakeToolchain`, predefined layouts like `cmake_layout` and the `CMakePresets` CMake feature.

As of November 19 2023, Conan Center shares `catch2/3.4.0`. So `conanfile.txt` should like like:

```
[requires]
catch2/3.4.0

[generators]
CMakeDeps
CMakeToolchain

[layout]
cmake_layout
```

## Conan profiles

Commands in the following sections assume two Conan profiles; `gcc-release` and `gcc-debug`:

```
~ $ cd ~/.conan2/profiles/
profiles $ cat gcc-release
[settings]
arch=armv8
build_type=Release
compiler=gcc
compiler.cppstd=20
compiler.libcxx=libstdc++11
compiler.version=12
os=Linux

profiles $ diff gcc-release gcc-debug
3c3
< build_type=Release
---
> build_type=Debug
```

## Installing `catch2` in a CMake project

In the folder you have provided `conanfile.txt`, you can run this Conan command to locally install (and build if necessary; `--build=missing`) this recipe and its dependencies (if any) in the Debug configuration:

```bash
(conan2) debian $ conan install . --build=missing --profile=gcc-debug
```

With the [`cmake_layout` option](https://docs.conan.io/2/reference/tools/cmake/cmake_layout.html) in `conanfile.txt`, `conan install` command generates the structure of a typical CMake project: if the cmake generator is single-configuration, depending on the build type

- build folder:
  - `build/Debug`, or
  - `build/Release`
- generator folder
  - `build/Debug/generators`, or
  - `build/Release/generators`

> Note: there are two types of build configuration: a multi-config generator (like Visual Studio or Xcode), or a single-config generator (like Unix Makefiles).

## CMake integration

[Catch2's CMake build](https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md) exports two targets,

- `Catch2::Catch2`, and
- `Catch2::Catch2WithMain`.

If you do not need custom main function, you should be using `Catch2::Catch2WithMain` (and only). Linking against it will add the proper include paths and link your target together with 2 static libraries that implement Catch2 and its main respectively.

If you need custom main, you should link only against `Catch2::Catch2`.

This means that if Catch2 has been installed on the system, it should be enough to do:

```
find_package(Catch2 3 REQUIRED)

# These tests can use the Catch2-provided main
add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
```

Catch2 also contains two CMake scripts that help users with automatically registering their `TEST_CASE`s with CTest. They can be found in the `extras` folder, and are

- `Catch.cmake`
- `CatchSharedTests.cmake`

If Catch2 has been installed in system, both of these can be used after doing `find_package(Catch2 REQUIRED)`. Otherwise you need to add them to your CMake module path.

`Catch.cmake` provides function `catch_discover_tests` to get tests from a target. This function works by running the resulting executable with `--list-test-names-only` flag, and then parsing the output to find all existing tests.

```
find_package(Catch2 3 REQUIRED)

add_executable(unit_tests
  factorial_test.cpp
)
target_link_libraries(unit_tests PRIVATE
  sut
  Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(unit_tests)
```

## Building the project using `CMakePresets`

So far, note that:
- A `CMakeUserPresets.json` file is generated in the same folder of your `CMakeLists.txt` file, so you can use the `--preset` argument.
- The `CMakeUserPresets.json` is including the `CMakePresets.json` files located at the corresponding generators folder.
- The `CMakePresets.json` contains information about the `conan_toolchain.cmake` location and even the `binaryDir` set with the output directory.

As you are using a single-configuration generator:

```bash
(conan2) debian $ cmake --preset conan-debug  
(conan2) debian $ cmake --build --preset conan-debug

(conan2) debian $ build/Debug/test/unit_tests
```
