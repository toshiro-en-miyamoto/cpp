# Using the `doctest` Conan Package with CMake

[Here](https://conan.io/center/doctest?tab=useit), there is some basic information you can use to integrate doctest with CMake.

- `CMakeDeps`: generates information about where the doctest library and its dependencies are installed together with other information like version, flags, and directory data or configuration. CMake will use this files when you invoke `find_package()` in your `CMakeLists.txt`.
- `CMakeToolchain`: generates a CMake *toolchain file* that you can later invoke with CMake in the command line using `-DCMAKE_TOOLCHAIN_FILE=xxx_toolchain.cmake`.




