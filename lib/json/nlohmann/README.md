# `nlohmann/json`

With this structure:

```base
nlohmann $ ls -R
.:
CMakeLists.txt
conanfile.txt
src

./src:
tutorial1.cpp
```

`CMakeLists.txt` looks like:

```bash
nlohmann $ cat CMakeLists.txt
cmake_minimum_required(VERSION 3.26)
project(nlohmann_json_tutorial CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(nlohmann_json REQUIRED)

add_executable(tutorial1 src/tutorial1.cpp)
target_link_libraries(tutorial1 nlohmann_json::nlohmann_json)
```

And `conanfile.txt`:

```bash
nlohmann $ source ~/venv/conan2/bin/activate
(conan2) nlohmann $ cat conanfile.txt
[requires]
nlohmann_json/3.11.2

[generators]
CMakeDeps
CMakeToolchain
```

Install `nlohmann_json` and generate the files that CMake needs to find this library:

```bash
(conan2) nlohmann $ conan install . \
  --output-folder=build --build=missing

(conan2) nlohmann $ cd build
(conan2) build $ cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release
```

Among the files CMake generates in the `build` directory is [`compile_commands.json`](https://cmake.org/cmake/help/latest/variable/CMAKE_EXPORT_COMPILE_COMMANDS.html). The json file file containing the exact compiler calls for all translation units of the project in machine-readable form.

In the Advanced section of the *C/C++: Edit Configurations (UI)*, you can supply the path to your `compile_commands.json` and the extension will use the compilation information listed in that file to configure IntelliSense. Once setting the pathname of the compile commands file, the `c_cpp_properties.json` file in the `.vscode` directory of the VS Code workplace include the following option:

```json
"compileCommands": "${workspaceFolder}/build/compile_commands.json"
```

With the setting, IntelliSense can find `nlohmann/json.hpp`:

```c++
#include <nlohmann/json.hpp>
```

You can build and run the executable;

```
(conan2) build $ cmake --build .
(conan2) build $ source conanrun.sh
(conan2) build $ ./tutorial1
All tests passed (3 asserts in 2 tests)
```
