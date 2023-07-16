# `nlohmann/json`

With this structure:

```base
$ ls -R
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

find_package(nlohmann_json REQUIRED)

add_executable(tutorial1 src/tutorial1.cpp)
target_link_libraries(tutorial1 nlohmann_json::nlohmann_json)
```

And `conanfile.txt`:

```bash
nlohmann $ cat conanfile.txt
[requires]
nlohmann_json/3.11.2

[generators]
CMakeDeps
CMakeToolchain
```

Then install `nlohmann_json`

```bash
nlohmann $ source ~/venv/conan2/bin/activate
(conan2) nlohmann $ conan install . --output-folder=build --build=missing
(conan2) nlohmann $ cd build
(conan2) build $ cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
(conan2) build $ cmake --build .
```
