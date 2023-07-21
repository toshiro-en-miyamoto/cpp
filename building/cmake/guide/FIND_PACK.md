# Using Pre-built Packages With `find_package()`

The primary methods of [bringing dependencies](https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html) into the build are the `find_package()` command and the `FetchContent` module.

> Dependencies do not necessarily have to be pre-built in order to use them with CMake. They can be built from sources as part of the main project. The `FetchContent` module provides functionality to download content (typically sources, but can be anything) and add it to the main project if the dependency also uses CMake.

A package needed by the project may already be built and available at some location on the user's system. That package might

- have been built by CMake, or
- have used a different build system entirely, or
- even just be a collection of files that didn't need to be built at all.

CMake provides the `find_package()` command for these scenarios.

In most cases, projects should generally use the *Basic Signature*.

- this will involve just the package name,
- maybe a version constraint, and
- the `REQUIRED` keyword if the dependency is not optional.
- A set of package components may also be specified.

```cmake
find_package(Catch2)
find_package(GTest REQUIRED)
find_package(Boost 1.79 COMPONENTS date_time)
```

## Config-file packages

The preferred way for a third party to provide executables, libraries, headers, and other files for use with CMake is to provide *config files*. These are text files shipped with the package, which define CMake targets, variables, commands, and so on.

The config file must be named either

- `<PackageName>Config.cmake` or
- `<LowercasePackageName>-config.cmake`

This file is the entry point to the package for CMake.

A separate optional file named

- `<PackageName>ConfigVersion.cmake` or
- `<LowercasePackageName>-config-version.cmake`

may also exist in the same directory. This file is used by CMake to determine whether the version of the package satisfies any version constraint included in the call to `find_package()`.

> The `nlohmann_json/3.11.2` Conan package provides with `config.cmake` and `config-version.cmake` files:

```bash
$ ls -l nlohmann_json-config*.cmake
-rw-r--r-- 1 ~ ~ 1558 ~ ~ nlohmann_json-config.cmake
-rw-r--r-- 1 ~ ~  577 ~ ~ nlohmann_json-config-version.cmake
```

There may be additional files providing CMake commands or *Imported Targets* for you to use. CMake does not enforce any naming convention for these files.

> The `nlohmann_json` package provides with `nlohmann_jsonTargets.cmake`:

```bash
$ ls -l nlohmann_json*.cmake
-rw-r--r-- 1 ~ ~ 1558 ~ ~ nlohmann_json-config.cmake
-rw-r--r-- 1 ~ ~  577 ~ ~ nlohmann_json-config-version.cmake
-rw-r--r-- 1 ~ ~ 2042 ~ ~ nlohmann_json-release-armv8-data.cmake
-rw-r--r-- 1 ~ ~ 4070 ~ ~ nlohmann_json-Target-release.cmake
-rw-r--r-- 1 ~ ~  973 ~ ~ nlohmann_jsonTargets.cmake
```

They are related to the primary `config.cmake` file by use of the CMake `include()` command. The `config.cmake` file would typically include these for you, so they won't usually require any additional step other than the call to `find_package()`.

```bash
$ sed -n '/include.*nlohmann/p' nlohmann_json-config.cmake 
include(${CMAKE_CURRENT_LIST_DIR}/nlohmann_jsonTargets.cmake)
```

> `nlohmann_jsonTargets.cmake` includes the debug and release variables and library finders:

```
# Load the debug and release variables
get_filename_component(_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
file(GLOB DATA_FILES "${_DIR}/nlohmann_json-*-data.cmake")

foreach(f ${DATA_FILES})
    include(${f})

# Load the debug and release library finders
get_filename_component(_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
file(GLOB CONFIG_FILES "${_DIR}/nlohmann_json-Target-*.cmake")

foreach(f ${CONFIG_FILES})
  include(${f})
```
