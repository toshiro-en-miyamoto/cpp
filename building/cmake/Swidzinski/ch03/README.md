# Setting Up Your First CMake Project

## Basic directives and commands

A minimum `CMakeLists.txt` from Chapter 1:

```cmake
cmake_minimum_required(VERSION 3.20)
project(Hello)
add_executable(Hello hello.cpp)
```

### `project()`

```
project(<PROJECT-NAME> [<language-name>...])
```

CMake enables C and C++ by default, so you may want to explicitly specify only `CXX` for your C++ projects.

```cmake
project(Hello CXX)
```

## Partitioning

```
03-add_subdirectory/
    CMakeLists.txt
    main.cpp
    cars/
        CMakeLists.txt
        car.cpp
        car.h
```

Here, we have two `CMakeLists.txt` files. The top-level file will use the nested directory, `cars`:

```cmake
# 03-add_subdirectory/CMakeLists.txt
cmake_minimum_required(VERSION 3.20.0)
project(Rental CXX)
add_executable(Rental main.cpp)

add_subdirectory(cars)
target_link_libraries(Rental PRIVATE cars)
```

The `add_subdirectory(cars)` adds a source directory to our build.

```cmake
# 03-add_subdirectory/cars/CMakeLists.txt
add_library(cars OBJECT
    car.cpp
#   car_maintenance.cpp
)
target_include_directories(cars PUBLIC .)
```

The `add_library()` command produces a globally visible target, `cars`.

The `target_include_directories(cars PUBLIC .)` adds the `cars` directory to its public include directories. This allows `main.cpp` to include the `cars.h` file without providing a relative path:

```
#include "car.h"
```

## Project structures

Among the many available project structure templates online,

```
/
    cmake/
        script/
        module/
        include/
    src/
        app1/
        app2/
        lib1/
        lib2/
    doc/
    extern/
    test/
```

The `cmake` directory includes macros and functions, find_modules, and one-off scripts. The `extern` contains the external projects we are building from source.

The main listfile shouldn't declare any build steps on its own, but instead, it should use the `add_subdirectory()` command to execute all of the listfiles in the nested directories.

The following list represents the actions executed by each `/CMakeLists.txt`.

- `/CMakeLists.txt` (the top level listfile)
  - CMake policies
  - project settings
  - global variables
  - global includes
  - global dependencies
  - add subdirectories
  - `src/CMakeLists.txt`
    - add subdirectories
    - `src/app1/CMakeLists.txt` add executable
    - `src/app2/CMakeLists.txt` add executable
    - `src/lib1/CMakeLists.txt` add library
    - `src/lib2/CMakeLists.txt` add library
  - `doc/CMakeLists.txt`
    - add documentation target
  - `external/CMakeLists.txt`
    - download external project
    - clone git repository
  - `test/CMakeLists.txt`
    - configure CTest
    - add test targets

Remember that the preceding steps are happening in the exact order in which we wrote the commands in our listfiles.

## Toolchains

We have `CMakeLists.txt` and `main.cpp` in the directory `08-test_run/`.

```cmake
cmake_minimum_required(VERSION 3.20.0)
project(TestRun CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

try_run(run_result compile_result
    ${CMAKE_BINARY_DIR}/test_output ${CMAKE_SOURCE_DIR}/main.cpp
    RUN_OUTPUT_VARIABLE output)

message("run_result: ${run_result}")
message("compile_result: ${compile_result}")
message("output:\n\n${output}")
```

```
#include <iostream>

int main()
{
    std::cout << "Quick check if things work." << std::endl;
}
```

Then, run `cmake -B build` in the directory:

```bash
$ cmake -B build
-- The CXX compiler identification is GNU 10.2.1
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
run_result: 0
compile_result: TRUE
output:

Quick check if things work.

-- Configuring done
-- Generating done
-- Build files have been written to: /home/toshiro/repo/build/cmake/ch03/
```
