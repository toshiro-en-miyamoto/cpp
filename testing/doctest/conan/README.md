# `doctest` with Conan

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

Now, you can run this Conan command to locally install (and build if necessary) this recipe and its dependencies (if any):

```bash
$ conan install conanfile.txt --build=missing
```

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

