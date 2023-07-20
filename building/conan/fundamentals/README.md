# Conan fundamentals

- Recipes
  - `conanfile.py`
  - `conanfile.txt`
- Configuration
  - profiles
  - `settings.yml`
  - `global.conf`
  - built-in conf
  - `remotes.json`
  - `source_credentials.json`
- Recipe tools
  - `conan.tools.build`
  - `conan.tools.cmake`
  - `conan.tools.apple`
  - `conan.tools.microsoft`
  - `conan.tools.android`
  - `conan.tools.scm`

## Recipes

[A **package** is defined by a `conanfile.py`.](https://docs.conan.io/2/introduction.html) This is a file that defines the package’s dependencies, sources, how to build the binaries from sources, etc. One package `conanfile.py` recipe can generate any arbitrary number of binaries, one for each different platform and **configuration**: operating system, architecture, compiler, build type, etc.

[The `conanfile.txt` file](https://docs.conan.io/2/reference/conanfile_txt.html) is a simplified version of `conanfile.py`, aimed at simple consumption of dependencies, but it cannot be used to create a package. Also, it is not necessary to have a `conanfile.txt` for consuming dependencies, a `conanfile.py` is perfectly suited for simple consumption of dependencies.

[The easiest way to install the Zlib library](https://docs.conan.io/2/tutorial/consuming_packages/build_simple_cmake_project.html) and find it from our project with Conan is using a `conanfile.txt` file.

```
[requires]
zlib/1.2.11

[generators]
CMakeDeps
CMakeToolchain
```

- `[requires]` section is where we declare the libraries we want to use in the project.
- `[generators]` section tells Conan to generate the files that the compilers or build systems will use to find the dependencies and build the project.

A recipe `conanfile.py` equivalent of the preceding `conanfile.txt` could like [this](https://docs.conan.io/2/tutorial/consuming_packages/the_flexibility_of_conanfile_py.html):

```python
from conan import ConanFile


class CompressorRecipe(ConanFile):
  settings = "os", "compiler", "build_type", "arch"
  generators = "CMakeToolchain", "CMakeDeps"

  def requirements(self):
    self.requires("zlib/1.2.11")
```

To create the Conan recipe we declared a new class that inherits from the `ConanFile` class.

- `settings` attribute defines the project-wide variables, like the compiler, its version, or the OS itself that may change when we build our project.
- `generators` attribute specifies which Conan generators will be run when we call the `conan install` command.

## Configuration files

### Profiles

[Conan Profiles](https://docs.conan.io/2/reference/config_files/profiles.html) allow users to define a configuration set for things like the compiler, build configuration, architecture, shared or static libraries, etc. Conan, by default, will not try to detect a profile automatically, so we need to create one. 

Profiles can be created with the `detect` option in `conan profile` command, and edited later.

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

Conan will always set the default C++ standard as the one that the detected compiler version uses by default. If you want to use a different C++ standard, you can edit the default profile file directly. As of writing, the default `cppstd` is `gnu14` for Linux distributions and `gnu17` for Mac OS.

```bash
(conan2) ~ $ cd ~/.conan2/profiles
(conan2) profiles $ mv default default.org
(conan2) profiles $ sed s/gnu14/20/ < default.org > default
(conan2) profiles $ diff default.org default
5c5
< compiler.cppstd=gnu14
---
> compiler.cppstd=20

(conan2) profiles $ cat default
[settings]
arch=armv8
build_type=Release
compiler=gcc
compiler.cppstd=20
compiler.libcxx=libstdc++11
compiler.version=10
os=Linux
```

You can store different profiles and use them to build for different settings. For example, to use a `build_type=Debug` to all the packages you build with that profile.

```bash
(conan2) profiles % sed s/Release/Debug/ < default > debug
(conan2) profiles % cat debug
[settings]
arch=x86_64
build_type=Debug
compiler=apple-clang
compiler.cppstd=20
compiler.libcxx=libc++
compiler.version=14
os=Macos
```

There are the available sections in profiles:

- `[settings]`: List of settings available from `settings.yml`
  ```
  [settings]
  arch=armv8
  build_type=Release
  os=Linux
  ```
- `[options]`: List of options available from your recipe and its dependencies
  ```
  [options]
  MyLib:shared=True
  ```
- `[buildenv]`: List of environment variables that will be injected to the environment every time the ConanFile `run(cmd, env="conanbuild")`` method is invoked.
- `[runenv]`: List of environment variables that will be injected to the environment every time the ConanFile run(cmd, env="conanrun") method is invoked.
- `[conf]`: List of user/tools configurations
  ```
  [conf]
  tools.build:verbosity=verbose
  ```
  - `core.xxx` configuration can only be defined in `global.conf` file, but not in profiles.
  - `tools.yyy` and `user.zzz` can be defined in `global.conf` and they will affect both the “build” and the “host” context. Values defined in profiles will have priority over globally defined ones in `global.conf`.

### Settings and options

[Difference between settings and options](https://docs.conan.io/2/tutorial/consuming_packages/different_configurations.html)

- **settings** are typically a project-wide configuration defined by the client machine. Things like the operating system, compiler or build configuration that will be common to several Conan packages and would not make sense to define one default value for only one of them. For example, it doesn’t make sense for a Conan package to declare “Visual Studio” as a default compiler because that is something defined by the end consumer, and unlikely to make sense if they are working in Linux.
- **options** are intended for package-specific configuration that can be set to a default value in the recipe. For example, one package can define that its default linkage is static, and this is the linkage that should be used if consumers don’t specify otherwise.

### `settings.yml`

*Settings files* define the possible values of the `settings` attribute to ensure matching naming and spelling as well as defining a common settings model among users and the OSS community.

- `~/.conan2/setting.yml`
- `~/.conan2/settings_user.yml`

The `settings_user.yml` file will contain only the new fields-values that you want to use in your recipes, so the final result will be a composition of both files, the `settings.yml` and the `settings_user.yml`.

### `global.conf`

[`global.conf`](https://docs.conan.io/2/reference/config_files/global_conf.html), located in `~/.conan2`, is aimed to save some core/tools/user configuration variables that will be used by Conan.

- `core.*`: aimed to configure values of Conan core behavior (download retries, package ID modes, etc.). Only definable in `global.conf` file.
- `tools.*`: aimed to configure values of Conan tools (toolchains, build helpers, etc.) used in your recipes. Definable in both `global.conf` and profiles.
- `user.*`: aimed to define personal user configurations. They can define whatever user wants. Definable in both `global.conf` and profiles.

### Host and build contexts

A Conan tutorial, [the flexibility of using `conanfile.py`](https://docs.conan.io/2/tutorial/consuming_packages/the_flexibility_of_conanfile_py.html), demonstrates how to use a conanfile.py or conanfile.txt to build an application that compresses strings using the Zlib Conan packages. The tutorial also explained that you can set information like the operating system, compiler or build configuration in a file called the Conan profile.

For all those examples, we used the same platform for building and running the application. But, what if you want to build the application on your machine running Ubuntu Linux and then run it on another platform like a Raspberry Pi?

Conan can model that case using two different profiles, one for the machine that builds the application (Ubuntu Linux) and another for the machine that runs the application (Raspberry Pi).

Even if you specify only one `--profile` argument when invoking Conan, Conan will internally use two profiles. One for the machine that builds the binaries (called the `build` profile) and another for the machine that runs those binaries (called the `host` profile).

```bash
(conan2) ~ $ conan install . --build=missing --profile=raspberry
```

is equivalent to:

```bash
(conan2) ~ $ conan install . --build=missing --profile:host=raspberry --profile:build=default
```

`profile:host` is the profile that defines the platform where the built binaries will run. For our string compressor application this profile would be the one applied for the Zlib library that will run in a Raspberry Pi.
