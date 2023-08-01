# Building for multiple configurations: Release, Debug

Let’s build our project for Debug configuration:

```bash
(conan2) debug $ conan install . \
  --output-folder=build --build=missing --settings=build_type=Debug
```

This is the equivalent of having debug profile and running these command using the `--profile` argument. When you call a Conan command setting the `--profile`, Conan will take all the information from the profile and apply it to the packages you want to build or install.

```bash
% cat ~/.conan2/profiles/debug.clang16
[settings]
arch=armv8
build_type=Debug
compiler=clang
compiler.cppstd=20
compiler.libcxx=libc++
compiler.version=16
os=Linux
[runenv]
{% set clang_home = '/usr/local/clang-16.0.0' %}
{% set clang      = clang_home + '/bin/clang' %}
{% set clang_lib  = clang_home + '/lib/aarch64-linux-gnu' %}
LD_LIBRARY_PATH={{ clang_lib }}
[conf]
tools.build:compiler_executables={'c': '{{ clang }}', 'cpp': '{{ clang + '++' }}'}
```

Then this `conan install` command will check if we already installed the required libraries (Zlib) in Debug configuration and install them otherwise. It will also set the build configuration in the conan_toolchain.cmake toolchain that the CMakeToolchain generator creates so that when we build the application it’s built in Debug configuration.

```bash
debug $ source ~/venv/conan2/bin/activate

(conan2) debug $ conan install . \
  --output-folder=build --build=missing --profile=debug.clang16
........
Install finished successfully
```

Now build your project as you did in the previous examples and check in the output how it was built in Debug configuration:

```bash
(conan2) debug $ cd build
(conan2) build $ cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Debug

(conan2) build $ cmake --build .
[ 50%] Building CXX object CMakeFiles/compressor.dir/src/main.cpp.o
[100%] Linking CXX executable compressor
[100%] Built target compressor

(conan2) debug $ deactivate

build $ source conanrun.sh
build $ ./compressor
Uncompressed size is: 233
Compressed size is: 147
ZLIB VERSION: 1.2.13
Debug configuration!

build $ source deactivate_conanrun.sh
Restoring environment
```
