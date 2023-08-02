# Conan

[Conan](https://conan.io/) is a dependency and package manager for C and C++ languages.

- is free and open-source, works in all platforms ( Windows, Linux, OSX, FreeBSD, Solaris, etc.)
- can be used to develop for all targets including embedded, mobile (iOS, Android), and bare metal
- integrates with all build systems like CMake, Visual Studio (MSBuild), Makefiles, SCons, etc., including proprietary ones

Conan is specifically designed and optimized for accelerating the development and Continuous Integration of C and C++ projects. With full binary management, it can create and reuse

- any number of different binaries (for different configurations like architectures, compiler versions, etc.)
- for any number of different versions of a package, using exactly the same process in all platforms.

## Installing Conan 2.0

The preferred and strongly recommended way to [install Conan](https://docs.conan.io/2/installation.html) is from PyPI, the Python Package Index, using the `pip` command.

Using Python virtual environment is strongly recommended. If not, it is possible that conan dependencies will conflict with previously existing dependencies, especially if you are using Python for other purposes.

```bash
$ python -m venv ~/venv/conan2
```

Running the [`venv` module](https://docs.python.org/3/library/venv.html) creates the target directory (`~/venv/conan2`) and places a `pyvenv.cfg` file in it with a home key pointing to the Python installation from which the command was run.

A virtual environment may be *activated* using a script in its binary directory. This will prepend that directory to your `PATH`, so that running `python` will invoke the environment’s Python interpreter and you can run installed scripts without having to use their full path.

```bash
$ source ~/venv/conan2/bin/activate
(conan2) ~ $ which pip
~/venv/conan2/bin/pip
```

Now you can install Conan in the virtual environment:

```bash
(conan2) ~ $ pip install conan
(conan2) build $ which conan
~/venv/conan2/bin/conan

(conan2) ~ $ deactivate
~ $
```

## Conan Profiles

[*Conan profiles*](https://docs.conan.io/2/reference/config_files/profiles.html) allow users to define a configuration set for things like the compiler, build configuration, architecture, shared or static libraries, etc. Conan, by default, will not try to detect a profile automatically, so we need to create one. 

Profiles can be created with the detect option in `conan profile` command, and edited later.

```bash
(conan2) ~ $ conan profile detect
Found gcc 10
gcc>=5, using the major as version
gcc C++ standard library: libstdc++11
Detected profile:
[settings]
arch=armv8
build_type=Release
compiler=gcc
compiler.cppstd=gnu14
compiler.libcxx=libstdc++11
compiler.version=10
os=Linux
Saving detected profile to ~/.conan2/profiles/default
```

For more details, [here](./fundamentals/README.md)

> [Best practices](https://docs.conan.io/2/reference/commands/profile.html): It is not recommended to use `conan profile detect` in production. To guarantee reproducibility, it is recommended to define your own profiles, store them in a git repo or in a zip in a server, and distribute it to your team and CI machines with `conan config install`, together with other configuration like custom settings, custom remotes definition, etc.

## Integrating Clang

Apple Clang is the default C/C++ compiler for Mac OS X, but Apple does not support [statically linked binaries](https://developer.apple.com/library/archive/qa/qa1118/_index.html) on Mac OS X, where dynamic-linking is the only option.

On GCC-defaulted systems such as Debian, dynamically linked executables build with Clang require to know at runtime where `libc++.so` is located. A simple way to do so is set the `LD_LIBRARY_PATH` variable environment.

> While [`LD_LIBRARY_PATH`](https://tecadmin.net/understanding-the-ld_library_path-environment-variable/) can be handy, an attacker can set LD_LIBRARY_PATH to a directory containing malicious versions of common libraries, tricking the program into using them.

In order to integrate Clang on such systems, edit Conan profiles to set Clang attributes in the profile files:

```bash
~ $ cat ~/.conan2/profiles/default
[settings]
arch=armv8
build_type=Release
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

The environment variables specified in the `[runenv]` section of your Conan profiles go into `build/conanrun*.sh`. The shell sets the environment variables.

```bash
~ $ source ~/venv/conan2/bin/activate
(conan2) ~ $ cd path/to/simple

(conan2) simple $ conan install . \
  --output-folder=build --build=missing
......
Install finished successfully

(conan2) simple $ ls build/*.sh | grep conanrun
build/conanrunenv-release-armv8.sh
build/conanrun.sh
build/deactivate_conanrun.sh
```

Upon successful build, run the shells:

```bash
(conan2) simple $ cd build
(conan2) build $ cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release
(conan2) build $ cmake --build .

(conan2) simple $ deactivate

build $ source conanrun.sh
build $ env | grep LD_
LD_LIBRARY_PATH=/usr/local/clang-16.0.0/lib/aarch64-linux-gnu

build $ ./compressor
Uncompressed size is: 233
Compressed size is: 147
ZLIB VERSION: 1.2.13
```

The shell not only sets the environment variables but creates `build/deactivate_conanrun.sh`, which allows you to restore the environment.

```bash
build $ ls *.sh | grep conanrun
conanrunenv-release-armv8.sh
conanrun.sh
deactivate_conanrunenv-release-armv8.sh
deactivate_conanrun.sh

build $ source deactivate_conanrun.sh
Restoring environment

build $ env | grep LD_
build $ 
```

## Integrating VS Code

Setting `(CMAKE_EXPORT_COMPILE_COMMANDS ON)` tells CMake to
generate [`compile_commands.json`](https://cmake.org/cmake/help/latest/variable/CMAKE_EXPORT_COMPILE_COMMANDS.html) in the `build` directory. The json file containing the exact compiler calls for all translation units of the project in machine-readable form.

```cmake
cmake_minimum_required(VERSION 3.26)
project(tutorial CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(doctest REQUIRED)

add_executable(tutorial1 src/tutorial1.cpp)
target_link_libraries(tutorial1 doctest::doctest)
```

In the Advanced section of the *C/C++: Edit Configurations (UI)*, you can supply the path to your `compile_commands.json` and the extension will use the compilation information listed in that file to configure IntelliSense. Once setting the pathname of the compile commands file, the `c_cpp_properties.json` file in the `.vscode` directory of the VS Code workplace include the following option:

```json
"compileCommands": "${workspaceFolder}/build/compile_commands.json"
```

With the setting, IntelliSense can find header files provided by the Conan packages.

```c++
#include <doctest/doctest.h>
```
