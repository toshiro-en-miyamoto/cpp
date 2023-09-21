# Template integrating Conan, CMake, and `doctest`

`conanfile.txt`:

```
[requires]
doctest/2.4.11

[generators]
CMakeDeps
CMakeToolchain

[layout]
cmake_layout
```

## Conan profiles

Two Conan profiles; `release` and `debug`:

```
(conan2) cd ~/.conan2/profiles/
(conan2) profiles % cat release
[settings]
arch=x86_64
build_type=Release
compiler=apple-clang
compiler.cppstd=20
compiler.libcxx=libc++
compiler.version=14
os=Macos

(conan2) profiles % diff release debug
3c3
< build_type=Release
---
> build_type=Debug
```

## Installing dependencies

Install (and build if necessary; `--build=missing`) dependencies in the Release and Debug configurations:

```bash
(conan2) proj % conan install . --build=missing --profile=release
(conan2) proj % conan install . --build=missing --profile=debug
```

## Specifying `doctest` in `CMakeList.txt`

A simple use case using the CMake file name and the global target:

```
# ...
find_package(doctest REQUIRED)
# ...
target_link_libraries(YOUR_TARGET doctest::doctest)
```

Then you can include the header file as follows:

```c++
#include <doctest/doctest.h>
```

## Building the project using `CMakePresets`

Build the project (using a single-configuration generator):

```bash
(conan2) proj % cmake --preset conan-release
(conan2) proj % cmake --build --preset conan-release

(conan2) proj % cmake --preset conan-debug  
(conan2) proj % cmake --build --preset conan-debug

(conan2) cmake % build/Debug/test/components/zzz/zzz_ut
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases: 1 | 1 passed | 0 failed | 0 skipped
[doctest] assertions: 5 | 5 passed | 0 failed |
[doctest] Status: SUCCESS!
```
