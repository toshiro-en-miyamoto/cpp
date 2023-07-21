# CMake Toolchains

[CMake uses a toolchain](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html) of utilities to compile, link libraries and create archives, and other tasks to drive the build.

- The toolchain utilities available are determined by the languages enabled.
- In normal builds, CMake automatically determines the toolchain for host builds based on system introspection and defaults.
- In cross-compiling scenarios, a toolchain file may be specified with information about compiler and utility paths.

> New in version 3.19: One may use `cmake-presets(7)` to specify toolchain files.

## Languages

Languages are enabled by the `project()` command. Language specific built-in variables, such as `CMAKE_CXX_COMPILER` are set by invoking the `project()` command.

If no project command is in the top-level CMakeLists file, one will be implicitly generated. By default the enabled languages are `C` and `CXX`:

```cmake
project(CXX_Only_Project CXX)
```

## Variables and Properties

Several variables relate to the language components of a toolchain which are enabled:

- `CMAKE_<LANG>_COMPILER`: The full path to the compiler used for `<LANG>`
- `CMAKE_<LANG>_COMPILER_ID`: The compiler identifier used by CMake
- `CMAKE_<LANG>_COMPILER_VERSION`: The version of the compiler.
- `CMAKE_<LANG>_FLAGS`: flags that will be added to the compile command when compiling

CMake needs a way to determine which compiler to use to invoke the linker. This is determined by the `LANGUAGE` property of source files of the `target`, and in the case of static libraries, the `LANGUAGE` of the dependent libraries. The choice CMake makes may be overridden with the `LINKER_LANGUAGE` target property.

## Cross Compiling

If `cmake` is invoked with the command line parameter

- `--toolchain path/to/file` or
- `-DCMAKE_TOOLCHAIN_FILE=path/to/file`

the file will be loaded early to set values for the compilers. The `CMAKE_CROSSCOMPILING` variable is set to true when CMake is cross-compiling.

A typical cross-compiling toolchain for Linux has content such as:

```cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_SYSROOT /home/devel/rasp-pi-rootfs)
set(CMAKE_STAGING_PREFIX /home/devel/stage)

set(tools /home/devel/gcc-4.7-linaro-rpi-gnueabihf)
set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
```


