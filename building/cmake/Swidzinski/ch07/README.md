# Managing Dependencies with CMake

External dependencies can play an important part in the process of building and controlling the quality of your code – whether it is in the form of special compilers such as Google Protocol Buffers or testing frameworks such as Google Test.

## Find installed packages

The `find_package()` can often do it for you if the package in question provides an appropriate `config-file` that allows CMake to determine variables necessary to support the package.

If you plan to use some popular library that doesn't provide it, don't worry just yet. Chances are that CMake authors have bundled the file with CMake itself (these are called `find-modules`, to differentiate from `config-files`).

If that's not the case, we still have some options:

- Provide our own `find-modules` for a specific package and bundle it with our project.
- Write a `config-file` and ask package maintainers to ship the package with it.

Both `find-modules` and `config-files` can be used in CMake projects with a single `find_package()` command. CMake looks for matching `find-modules`, and if it can't find any, it will turn to `config-files`.

The search will start from the path stored in the `CMAKE_MODULE_PATH` variable, which

- is empty by default
- can be configured by a project when it wants to add and use external `find-modules`

Next, CMake will scan the list of built-in `find-modules` available in the installed version of CMake.

If no applicable module is found, it's time to search for corresponding package `config-files`. CMake has a long list of paths appropriate for a host operating system, which can be scanned for filenames matching the following pattern:

- `<CamelCasePackageName>Config.cmake`
- `<kebab-case-package-name>-config.cmake`

### [Protocol Buffers](https://developers.google.com/protocol-buffers/docs/overview)

Protocol buffers provide a language-neutral, platform-neutral, extensible mechanism for serializing structured data in a forward-compatible and backward-compatible way. It’s like JSON, except it's smaller and faster, and it generates native language bindings.

Protocol buffers are a combination of the definition language (created in `.proto` files), the code that the proto compiler generates to interface with data, language-specific runtime libraries, and the serialization format for data that is written to a file (or sent across a network connection).

```cmake
# chapter07/01-find-package-variables/CMakeLists.txt
cmake_minimum_required(VERSION 3.20.0)
project(FindPackageProtobufVariables CXX)

find_package(Protobuf REQUIRED)
protobuf_generate_cpp(GENERATED_SRC GENERATED_HEADER
    message.proto)

add_executable(main main.cpp
    ${GENERATED_SRC} ${GENERATED_HEADER})
target_link_libraries(main PRIVATE ${Protobuf_LIBRARIES})
target_include_directories(main PRIVATE
    ${Protobuf_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR})
```

`find_package(Protobuf REQUIRED)` asks CMake to run the bundled `FindProtobuf.cmake` find-module and set up the Protobuf library for us. That find-module will scan commonly used paths and (because we provided the `REQUIRED` keyword) terminate if a library is not found. It will also specify useful variables and functions (such as the one on the next line).

`protobuf_generate_cpp` is a custom function defined in the Protobuf find-module. Under the hood, it calls `add_custom_command()`, which invokes the `protoc` compiler with appropriate arguments. We use this function by providing two variables that will be filled with paths to the generated source (`GENERATED_SRC`) and header (`GENERATED_HEADER`) files, and a list of files to compile (`message.proto`).

In most cases, you can expect some variables to be set when you call `find_package()`:

- `<PKG_NAME>_FOUND`
- `<PKG_NAME>_INCLUDE_DIRS` or `<PKG_NAME>_INCLUDES`
- `<PKG_NAME>_LIBRARIES` or `<PKG_NAME>_LIBS`
- `<PKG_NAME>_DEFINITIONS`
- `IMPORTED` targets

If a package supports so-called "modern CMake" (built around targets), it will provide those `IMPORTED` targets instead (or alongside) of these variables, which allows for cleaner, simpler code. It is recommended to prioritize targets over variables.

```cmake
# chapter07/02-find-package-targets/CMakeLists.txt
cmake_minimum_required(VERSION 3.20.0)
project(FindPackageProtobufTargets CXX)

find_package(Protobuf REQUIRED)
protobuf_generate_cpp(GENERATED_SRC GENERATED_HEADER
    message.proto)

add_executable(main main.cpp
    ${GENERATED_SRC} ${GENERATED_HEADER})
target_link_libraries(main PRIVATE protobuf::libprotobuf)
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})
```

The `protobuf::libprotobuf` imported target implicitly specifies *include directories* and, thanks to transitive dependencies, they are shared with our `main` target.

## Discovery legacy packages

## Your own find-modules

## Git repositories

## ExternalProject and FetchContent modules

Nowadays, it is recommended to go with the `FetchContent` module to import external projects.

```cmake
# chapter07/09-fetch-content/CMakeLists.txt
cmake_minimum_required(VERSION 3.20.0)
project(ExternalProjectGit CXX)

add_executable(welcome main.cpp)
configure_file(config.yaml config.yaml COPYONLY)

include(FetchContent)
FetchContent_Declare(ext-yaml-cpp
    GIT_REPOSITORY    https://github.com/jbeder/yaml-cpp.git
    GIT_TAG           yaml-cpp-0.7.0
)
FetchContent_MakeAvailable(ext-yaml-cpp)
target_link_libraries(welcome PRIVATE yaml-cpp)

include(CMakePrintHelpers)
cmake_print_properties(TARGETS yaml-cpp
    PROPERTIES TYPE SOURCE_DIR)
```

CMake effectively performs the configuration stage of the fetched project and retrieves any targets defined there in the current scope.

```bash
$ cmake -B build
: : : :
Properties for TARGET yaml-cpp:
   yaml-cpp.TYPE = "STATIC_LIBRARY"
   yaml-cpp.SOURCE_DIR = "/home/.../09-fetch-content/build/_deps/ext-yaml-cpp-src"
: : : :
```

We can explicitly access the targets created by the `yaml-cpp` library.

```c++
// chapter07/09-fetch-content/main.cpp
#include <string>
#include <iostream>
#include "yaml-cpp/yaml.h"

using namespace std;
int main() {
  string name = "Guest";

  YAML::Node config = YAML::LoadFile("config.yaml");
  if (config["name"])
    name = config["name"].as<string>();

  cout << "Welcome " << name << endl;
  return 0;
}
```

```yaml
# chapter07/09-fetch-content/config.yaml
name: Rafal
```


```bash
$ build/welcome
Welcome Rafal
```
