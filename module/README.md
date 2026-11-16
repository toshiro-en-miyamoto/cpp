# How to compile

## Google Gemini

Prompt:
> macos llvm clang c++20 compile with std module

### Overview

To compile C++ code using `import std;` on macOS, you must install upstream Homebrew LLVM (`brew install llvm`), as Apple's default Xcode Clang does not ship with standard library module sources or manifests. Additionally, while C++20 introduced the module system, the `std` module itself is formally a C++23 feature, though LLVM allows you to precompile `std.cppm` and use it in `-std=c++20` mode.

Because macOS defaults to the system `libc++` runtime, compiling with `import std;` requires pointing Homebrew's clang++ to Homebrew's bundled `libc++` headers and libraries.

### Command-Line Compilation

First, set a variable for your Homebrew LLVM prefix (usually `/opt/homebrew/opt/llvm` on Apple Silicon or `/usr/local/opt/llvm` on Intel Macs):

```bash
LLVM_PREFIX=$(brew --prefix llvm)
```

#### Step A: Precompile the `std` module (`std.pcm`)

You only need to do this once per compiler version or flag configuration. Pass `-Wno-reserved-module-identifier` because `std` is a reserved module name

```bash
~ % $LLVM_PREFIX/bin/clang++ -std=c++20 -stdlib=libc++ \
      -isystem "$LLVM_PREFIX/include/c++/v1" \
      -Wno-reserved-module-identifier \
      --precompile -o std.pcm \
      "$LLVM_PREFIX/share/libc++/v1/std.cppm"
```

#### Step B: Compile and link your program

Assuming you have a `main.cpp` containing `import std;`, compile the `std.pcm` into an object file (or pass `std.pcm` directly to Clang) and link against Homebrew's `libc++`:

```bash
~ % $LLVM_PREFIX/bin/clang++ -std=c++20 -stdlib=libc++ \
      -isystem "$LLVM_PREFIX/include/c++/v1" \
      -L"$LLVM_PREFIX/lib/c++" -Wl,-rpath,"$LLVM_PREFIX/lib/c++" \
      -fmodule-file=std=std.pcm \
      std.pcm main.cpp -o main
```

### CMake Setup (Recommended for Projects)

For multi-file projects, CMake (3.30+) with the Ninja generator handles module dependency scanning automatically. However, developers on [Reddit r/cpp_questions](https://www.reddit.com/r/cpp_questions/comments/1tm9i4a/using_c20_modules_and_module_std_with_cmake_on/) note that CMake fails to locate Homebrew's `libc++.modules.json` automatically because Homebrew Clang queries the macOS system SDK by default.

```
# CMakeLists.txt
cmake_minimum_required(VERSION 3.30)

# Must be set before project()
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")

project(ModuleExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_MODULE_STD 1)

add_executable(main main.cpp)
```

> Note: The UUID for `CMAKE_EXPERIMENTAL_CXX_IMPORT_STD` changes across CMake versions; check your installed CMake's `Help/dev/experimental.rst` file if CMake warns about an outdated UUID.

Pass Homebrew's compiler, `libc++.modules.json` path, and linker flags when configuring CMake:

```bash
~ % LLVM_PREFIX=$(brew --prefix llvm)

~ % cmake -G Ninja -B build \
      -DCMAKE_CXX_COMPILER="$LLVM_PREFIX/bin/clang++" \
      -DCMAKE_CXX_STDLIB_MODULES_JSON="$LLVM_PREFIX/lib/c++/libc++.modules.json" \
      -DCMAKE_CXX_FLAGS="-isystem $LLVM_PREFIX/include/c++/v1" \
      -DCMAKE_EXE_LINKER_FLAGS="-L$LLVM_PREFIX/lib/c++ -Wl,-rpath,$LLVM_PREFIX/lib/c++"

~ % cmake --build build
```
