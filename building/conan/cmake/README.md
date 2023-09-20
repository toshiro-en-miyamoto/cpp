# `doctest` with Conan

In [this example](https://docs.conan.io/2/examples/tools/cmake/cmake_toolchain/build_project_cmake_presets.html) we are going to see how to use `CMakeToolchain`, predefined layouts like `cmake_layout` and the `CMakePresets` CMake feature.

As of September 15 2023, Conan Center shares `doctest/2.4.11`. So `conanfile.txt` should like like:

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

Commands in the following sections assume two Conan profiles; `release` and `debug`:

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

## Installing `doctest` in a CMake project

In the folder you have provided `conanfile.txt`, you can run this Conan command to locally install (and build if necessary; `--build=missing`) this recipe and its dependencies (if any) in the Release configuration:

```bash
(conan2) proj % conan install . --build=missing --profile=release

(conan2) proj % ls -R build 
Release

build/Release:
generators

build/Release/generators:
CMakePresets.json                 deactivate_conanbuild.sh
cmakedeps_macros.cmake            deactivate_conanrun.sh
conan_toolchain.cmake             doctest-Target-release.cmake
conanbuild.sh                     doctest-config-version.cmake
conanbuildenv-release-x86_64.sh   doctest-config.cmake
conanrun.sh                       doctest-release-x86_64-data.cmake
conanrunenv-release-x86_64.sh     doctestTargets.cmake
```

With the [`cmake_layout` option](https://docs.conan.io/2/reference/tools/cmake/cmake_layout.html) in `conanfile.txt`, `conan install` command generates the structure of a typical CMake project: if the cmake generator is single-configuration, depending on the build type

- build folder:
  - `build/Debug` or `build/Release`
- generator folder
  - `build/Debug/generators` or `build/Release/generators`

> Note: there are two types of build configuration: a multi-config generator (like Visual Studio or Xcode), or a single-config generator (like Unix Makefiles).

You can install `doctest` in the Debug configuration with `debug` profile:

```bash
(conan2) proj % conan install . --build=missing --profile=debug

(conan2) proj % ls -R build                                    
Debug	Release

build/Debug:
generators

build/Debug/generators:
CMakePresets.json                 deactivate_conanbuild.sh                     
cmakedeps_macros.cmake            deactivate_conanrun.sh     
conan_toolchain.cmake             doctest-Target-debug.cmake        
conanbuild.sh                     doctest-config-version.cmake          
conanbuildenv-debug-x86_64.sh     doctest-config.cmake
conanrun.sh                       doctest-debug-x86_64-data.cmake
conanrunenv-debug-x86_64.sh       doctestTargets.cmake

build/Release:
generators

build/Release/generators:
CMakePresets.json                 deactivate_conanbuild.sh
cmakedeps_macros.cmake            deactivate_conanrun.sh
conan_toolchain.cmake             doctest-Target-release.cmake
conanbuild.sh                     doctest-config-version.cmake
conanbuildenv-release-x86_64.sh   doctest-config.cmake
conanrun.sh                       doctest-release-x86_64-data.cmake
conanrunenv-release-x86_64.sh     doctestTargets.cmake
```

## Specifying `doctest` in `CMakeList.txt`

These are the main declared targets:

- CMake package name(s): `doctest`
- CMake target name(s): `doctest::doctest`
- pkg-config file name(s): `doctest.pc`

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

So far, note that:
- A `CMakeUserPresets.json` file is generated in the same folder of your `CMakeLists.txt` file, so you can use the `--preset` argument.
- The `CMakeUserPresets.json` is including the `CMakePresets.json` files located at the corresponding generators folder.
- The `CMakePresets.json` contains information about the `conan_toolchain.cmake` location and even the `binaryDir` set with the output directory.

As you are using a single-configuration generator:

```bash
(conan2) proj % cmake --preset conan-release
(conan2) proj % cmake --build --preset conan-release

(conan2) proj % build/Release/test/components/zzz/zzz_ut 
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases: 1 | 1 passed | 0 failed | 0 skipped
[doctest] assertions: 5 | 5 passed | 0 failed |
[doctest] Status: SUCCESS!

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
