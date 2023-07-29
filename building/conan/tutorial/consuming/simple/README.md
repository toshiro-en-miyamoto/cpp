# Build a simple CMake project using Conan

We start from a very simple C language project with this structure:

- `CMakeLists.txt`
- `conanfile.txt`
- `src`
  - `main.cpp`

The `main.cpp` (modified for C++20) looks like

```c++
#include <zlib.h>

#include <vector>
#include <cstddef>
#include <string>
#include <ranges>
#include <iterator>

namespace my
{
  std::vector<std::byte> compress(std::u8string&& in)
  {
    const auto BUF_SIZE = in.size();
    std::vector<std::byte> out(BUF_SIZE);

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = BUF_SIZE;
    defstream.next_in = reinterpret_cast<Bytef*>(in.data());
    defstream.avail_out = BUF_SIZE;
    defstream.next_out = reinterpret_cast<Bytef*>(out.data());

    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    const auto OUT_SIZE
    = std::ranges::distance(out.begin(), std::ranges::find(out, std::byte{0x00}));
    out.resize(OUT_SIZE);

    return out;
  }
}

#include <iostream>

int main()
{
  static std::u8string in {
    u8"Conan is a MIT-licensed, Open Source package "
    "manager for C and C++ development for C and C++ "
    "development, allowing development teams to easily "
    "and efficiently manage their packages and "
    "dependencies across platforms and build systems."
  };
  std::cout
  << "Uncompressed size is: " << in.size() << std::endl;

  auto out = my::compress(std::move(in));
  
  std::cout
  << "Compressed size is: " << out.size() << std::endl;
  std::cout
  << "ZLIB VERSION: " << zlibVersion() << std::endl;
}
```

Also, the contents of `CMakeLists.txt` are:

```
cmake_minimum_required(VERSION 3.26)
project(compressor)

find_package(ZLIB REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ZLIB::ZLIB)
```

The easiest way to install the Zlib library and find it from our project with Conan is using a `conanfile.txt` file.

```
[requires]
zlib/1.2.13

[generators]
CMakeDeps
CMakeToolchain
```

We will use Conan to install Zlib and generate the files that CMake needs to find this library and build our project. We will generate those files in the folder `build`.

> Note: Before running the `conan install` command, specify the `LD_LIBRARY_PATH` environment variable in the `[runenv]` section of your Conan profile.

```bash
simple $ source ~/venv/conan2/bin/activate 
(conan2) simple $ conan install . \
  --output-folder=build --build=missing
```

There are a couple of things that happened:

- Conan installed the Zlib library from the remote server. This server stores both the Conan recipes and the binaries.
- Generators in the `conanfile.txt` generated several files under the `build` folder.
  - `CMakeDeps` generates files so that CMake finds the Zlib library we have just downloaded.
  - `CMakeToolchain` generates a toolchain file for CMake so that we can transparently build our project with CMake using the same settings that we detected for our default profile.

Now we are ready to build:

```bash
(conan2) simple $ cd build
(conan2) build $ cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release
(conan2) build $ cmake --build .

(conan2) simple $ deactivate

build $ source conanrun.sh
build $ ./compressor
Uncompressed size is: 233
Compressed size is: 147
ZLIB VERSION: 1.2.13

build $ source deactivate_conanrun.sh
Restoring environment
```
