# Building for multiple configurations: Release, Debug

When you call a Conan command setting the `--profile argument`, Conan will take all the information from the profile and apply it to the packages you want to build or install.

```bash
% cat ~/.conan2/profiles/debug
[settings]
arch=x86_64
build_type=Debug
compiler=apple-clang
compiler.cppstd=20
compiler.libcxx=libc++
compiler.version=14
os=Macos
```

Then this `conan install` command will check if we already installed the required libraries (Zlib) in Debug configuration and install them otherwise. It will also set the build configuration in the conan_toolchain.cmake toolchain that the CMakeToolchain generator creates so that when we build the application it’s built in Debug configuration.

```bash
$ conan install . --output-folder=build --build=missing --profile=debug
```

Now build your project as you did in the previous examples and check in the output how it was built in Debug configuration:

```bash
$ cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
$ cmake --build .
$ ./compressor
Uncompressed size is: 233
Compressed size is: 147
ZLIB VERSION: 1.2.13
Debug configuration!
```
