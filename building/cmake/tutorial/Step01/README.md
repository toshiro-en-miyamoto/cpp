# A Basic Starting Point

In this step, we will learn the following topics:

- configuring a CMake project
- building an executable
- specifying the C++ standard
- configured header files

and the following `cmake` commands:

- `cmake_minimum_required`
- `project`
- `add_executable`
- `set: CMAKE_CXX_STANDARD`
- `configure_file`
- `target_include_directories`

## Exercise 1 - Building a Basic Project

Create a simple CMake project. Files in the `Step01` directory:

```
tutorial/Step01:
    CMakeLists.txt
    tutorial.cxx
```

```cmake
cmake_minimum_required(VERSION 3.10)
project(Tutorial)
add_executable(Tutorial tutorial.cxx)
```

Configure the project:

```bash
tutorial $ mkdir Step01_build

tutorial $ cd Step01_build
Step01_build $ cmake ../Step01
-- The C compiler identification is GNU 10.2.1
-- The CXX compiler identification is GNU 10.2.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /tutorial/Step01_build

Step01_build $ ls
CMakeCache.txt  CMakeFiles  cmake_install.cmake  Makefile
```

Build the project.

```bash
Step01_build $ cmake --build .
[ 50%] Building CXX object CMakeFiles/Tutorial.dir/tutorial.cxx.o
[100%] Linking CXX executable Tutorial
[100%] Built target Tutorial

Step01_build $ ls
CMakeCache.txt  CMakeFiles  cmake_install.cmake  Makefile  Tutorial
```

Run the executable.

```bash
Step01_build $ ./Tutorial 4294967296
The square root of 4.29497e+09 is 65536

Step01_build $ ./Tutorial 10
The square root of 10 is 3.16228

Step01_build $ ./Tutorial
Usage: ./Tutorial number
```

## Exercise 2 - Specifying the C++ Standard

Add a feature that requires C++11.

```cmake
cmake_minimum_required(VERSION 3.10)
project(Tutorial)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(Tutorial tutorial.cxx)
```

Configure the project. 

```bash
Step01_build $ cmake ../Step01
-- Configuring done
-- Generating done
-- Build files have been written to: /tutorial/Step01_build
```

Build the project.

```bash
Step01_build $ cmake --build .
[ 50%] Building CXX object CMakeFiles/Tutorial.dir/tutorial.cxx.o
[100%] Linking CXX executable Tutorial
[100%] Built target Tutorial
```

Run the executable.

```bash
Step01_build $ ./Tutorial 4294967296
The square root of 4.29497e+09 is 65536

Step01_build $ ./Tutorial 10
The square root of 10 is 3.16228

Step01_build $ ./Tutorial
Usage: ./Tutorial number
```

## Exercise 3 - Configured Header Files

Sometimes it may be useful to have a variable that is defined in your `CMakelists.txt` file also be available in your source code. In this case, we would like to print the project version.

One way to accomplish this is by using a *configured header file*. We create an input file with one or more variables to replace. These variables have special syntax which looks like `@VAR@`.

```
tutorial/Step01:
    CMakeLists.txt
    tutorial.cxx
    TutorialConfig.h.in
```

```c++
// TutorialConfig.h.in
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
```

Then, we use the `configure_file()` command to copy the input file to a given output file and replace these variables with the current value of `VAR` in the `CMakelists.txt` file.

```cmake
cmake_minimum_required(VERSION 3.10)
project(Tutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(TutorialConfig.h.in TutorialConfig.h)
add_executable(Tutorial tutorial.cxx)

target_include_directories(Tutorial PUBLIC
    "${PROJECT_BINARY_DIR}"
)
```

While we could edit the version directly in the source code, using this feature is preferred since it creates a single source of truth and avoids duplication.

Configure the project so as to produce the `TutorialConfig.h` in the build tree.

```bash
Step01_build $ cmake ../Step01
-- Configuring done
-- Generating done
-- Build files have been written to: /tutorial/Step01_build
```

```
tutorial/Step01:
    CMakeLists.txt
    tutorial.cxx
    TutorialConfig.h.in
tutorial/Step01_build:
    TutorialConfig.h
```

The `TutorialConfig.h` looks like this:

```c++
#define Tutorial_VERSION_MAJOR 1
#define Tutorial_VERSION_MINOR 0
```

Now that you have the header file, the `tutorial.cxx` can include the file and reference variables in there:

```c++
#include <cmath>
#include <iostream>
#include <string>
#include "TutorialConfig.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout
            << argv[0] << " Version " 
            << Tutorial_VERSION_MAJOR << "."
            << Tutorial_VERSION_MINOR << std::endl;
        std::cout
            << "Usage: "
            << argv[0] << " number" << std::endl;
        return 1;
    }

    const double inputValue = std::stod(argv[1]);

    const double outputValue = sqrt(inputValue);
    std::cout
        << "The square root of " << inputValue
        << " is " << outputValue << std::endl;
    return 0;
}
```

