# `cxxopts`

[This](https://github.com/jarro2783/cxxopts) is a lightweight C++ option parser library, supporting the standard GNU style syntax for options.

Options can be given as:

```bash
--long
--long=argument
--long argument
-a
-ab
-abc argument
```

where `c` takes an argument, but `a` and `b` do not.

Additionally, anything after `--` will be parsed as a positional argument.

## Conan package

This is a simple CMake project layout using this library.

`conanfile.txt`:

```
[requires]
cxxopts/2.2.0
doctest/2.4.11

[generators]
CMakeDeps
CMakeToolchain
```

`CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.26)
project(nlohmann_json_tutorial CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(cxxopts REQUIRED)
find_package(doctest REQUIRED)

add_executable(example src/main.cpp)
target_link_libraries(example cxxopts::cxxopts)
target_link_libraries(example doctest::doctest)
```

`main.cpp`:

```c++
#include "cxxopts.hpp"
```

Install the Conan package and generate CMake files:

```bash
cxxopts $ ~/venv/conan2/bin/activate

(conan2) cxxopts $ conan install . \
  --output-folder=build --build=missing

(conan2) cxxopts $ cd build
(conan2) build $ cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release
```

In the Advanced section of the *C/C++: Edit Configurations (UI)*, you can supply the path to your `compile_commands.json` and the extension will use the compilation information listed in that file to configure IntelliSense.

In the Advanced section of the *C/C++: Edit Configurations (UI)*, you can supply the path to your `compile_commands.json` and the extension will use the compilation information listed in that file to configure IntelliSense.

```json
"compileCommands": "build/compile_commands.json"
```

Build the project and run the executable:

```bash
(conan2) build $ cmake --build .

(conan2) build $ source conanrun.sh
(conan2) build $ ./example
```

## Basics

Creates a `cxxopts::Options` instance.

```c++
cxxopts::Options options("example", "Example app with CxxOpts");
```

Then use `add_options`:

```c++
options.add_options()
  ("d,debug", "Enable debugging") // a bool parameter
  ("i,integer", "Int param", cxxopts::value<int>())
  ("f,file", "File name", cxxopts::value<std::string>())
  (
    "v,verbose", "Verbose output",
    cxxopts::value<bool>()->default_value("false")
  )
  ;
```

Options are declared with a long and an optional short option. A description must be provided. The third argument is the value, if omitted it is `boolean`. Any type can be given as long as it can be parsed, with `operator>>`.

To parse the command line do:

```c++
auto result = options.parse(argc, argv);
```

To retrieve an option `opt`, get the number of times it appeared:

```c++
result.count("opt");
```

and get its value.

```c++
result["opt"].as<type>()
```

If `opt` doesn't exist, or isn't of the right type, then an exception will be thrown.

> Note that the result of `options.parse` should only be used as long as the `Options` object that created it is in scope.

## Example code

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <cxxopts.hpp>

TEST_CASE("basics") {
  cxxopts::Options options("example", "Example app with CxxOpts");
  options.add_options()
    ("d,debug", "Enable debugging") // a bool parameter
    ("i,integer", "Int param", cxxopts::value<int>())
    ("f,file", "File name", cxxopts::value<std::string>())
    (
      "v,verbose", "Verbose output",
      cxxopts::value<bool>()->default_value("false")
    );

  SUBCASE("") {
  }
}
```
