# First Steps with CMake

## How does it work?

You can think of CMake as the orchestrator of your building process:

- the configuration state
- the generation state
- the building stage

### The configuration state

This stage is about

- reading project details stored in a directory, called the *source tree*, and
- preparing an output directory or *build tree* for the generation stage.

CMake starts by creating an empty build tree and collecting all of the details about the environment it is working in.

```
./CMakeLists.txt
./hello.cpp
```

Next, the `CMakeLists.txt` project configuration file is parsed and executed.

```
cmake_minimum_required(VERSION 3.20.0)
project(Hello)
add_executable(Hello hello.cpp)
```

During this process, CMake stores collected information in the build tree such as system details, project configurations, logs, and temp files, which are used for the next step. Specifically, a `CMakeCache.txt` file is created to store more stable variables.

### The generation state

After reading the project configuration, CMake will generate a *buildsystem* - configuration files for other tools (e.g., Makefiles, IDE project files).

> The generation stage is executed automatically after the configuration stage.

```bash
$ cmake -B build/
```

### The building stage

```bash
$ cmake --build build/

$ ./build/Hello
Hello World!
```

## Matering the command line

CMake is a family of tools:

- `cmake`: the main executable that configures, generates, and builds projects.
- `ctest`: the test driver program used to run and report test results.
- `cpack`: the packaging program used to generate installers and source packages.

### CMake

The syntax of the generation mode:

```
cmake [<options>] -S <path-to-source> -B <path-to-build>
```

For example, the following command build in the `./build` directory, and use a source from the current directory (`-S` is optional):

```bash
$ cmake -B build/
```

The syntax of the build mode:

```
cmake --build <dir> [<options>] [-- <build-tool-options>]
cmake --build <dir> --config <cfg>
```

Specify `Debug`, `Release`, `MinSizeRel`, or `RelWithDebInfo` as the configuration generator. Otherwise, CMake will use `Debug` as the default.

## Navigating the project files

### The source tree

This is the directory where your project will live. It is also called the *project root*. It contains all of the C++ sources and CMake project files.

- It is required that you provide a `CMakeLists.txt` configuration file in its top directory.
- It should be managed with a VCS such as `git`.

Avoid hardcoding any absolute paths to the source tree in your CMake code.

### The build tree

Also known as *build root* or *binary tree*, CMake uses this directory to store ... anything that your native build tool will create.

- Your binary files will be created here, such as executables and libraries, along with object files and archives used for final linking.
- Don't add this directory to your VCS. If you decide to put it inside the source tree, make sure to add it to the VCS ignore file.

### Listfiles

Files that contain the CMake language are called *listfiles*. They can be included one in another, by calling `include()` and `find_package()`, or indirectly with `add_subdirectory()`:

- CMake doesn't enforce consistent naming for these files, but usually, they have a `.cmake` extension.
- A very important naming exception is a file called `CMakeLists.txt`, which is the file to be executed in the configuration stage. It is required at the top of the source tree.

As CMake walks the source tree and includes different listfiles, the following variables are set:

- `CMAKE_CURRENT_LIST_DIR`,
- `CMAKE_CURRENT_LIST_FILE`,
- `CMAKE_PARENT_LIST_FILE`, and
- `CMAKE_CURRENT_LIST_LINE`

### CMakeLists.txt

CMake projects are configured with `CMakeLists.txt` listfiles. It should contains at least two commands:

- `cmake_minimum_required(VERSION <x.xx>)`
- `project(<name> <OPTIONS>)`: used to name the project and to specify the options to configure it

As your software grows, you might want to partition it into smaller units that can be configured and reasoned about separately. CMake supports this through the notion of subdirectories and their own `CMakeLists.txt` files.

Your project structure might look similar to the following example:

```
CMakeLists.txt
api/CMakeLists.txt
api/api.h
api/api.cpp
```

The top level `CMakeLists.txt` lools like:

```
cmake_minimum_required(VERSION 3.20.0)
project(Hello)
message("Top level CMakeLists.txt")
add_subdirectory(api)
```

The main aspects of the project are covered in the top-level file:

- managing the dependencies,
- stating the requirements, and
- detecting the environment.

In this file, we also have an `add_subdirectory(api)` command to include another `CMakeListst.txt` file from the `api` directory.

### CMakeCache.txt

Cache variables will be generated from `listfiles` and stored in `CMakeCache.txt` when the configure stage is run for the first time. This file resides in the root of the build tree.

```
# EXTERNAL cache entries

CMAKE_CXX_COMPILER:FILEPATH=/usr/bin/c++
CMAKE_CXX_FLAGS:STRING=
CMAKE_CXX_FLAGS_DEBUG:STRING=-g
CMAKE_CXX_FLAGS_MINSIZEREL:STRING=-Os -DNDEBUG
CMAKE_CXX_FLAGS_RELEASE:STRING=-O3 -DNDEBUG
CMAKE_CXX_FLAGS_RELWITHDEBINFO:STRING=-O2 -g -DNDEBUG

CMAKE_INSTALL_PREFIX:PATH=/usr/local

CMAKE_LINKER:FILEPATH=/usr/bin/ld
CMAKE_MODULE_LINKER_FLAGS:STRING=
CMAKE_MODULE_LINKER_FLAGS_DEBUG:STRING=
CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL:STRING=
CMAKE_MODULE_LINKER_FLAGS_RELEASE:STRING=
CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO:STRING=

CMAKE_PROJECT_NAME:STATIC=Hello
Hello_BINARY_DIR:STATIC=/home/toshiro/repo/build/cmake/ch01/01-hello/build
Hello_SOURCE_DIR:STATIC=/home/toshiro/repo/build/cmake/ch01/01-hello

# INTERNAL cache entries
```

The variable `CMAKE_CXX_FLAGS:STRING` specifies flags used by the CXX compiler during all build types.

Cache entries in the `EXTERNAL` section are meant for users to modify, while the `INTERNAL` section is managed by CMake. Note that it's not recommended that you change them manually.

- You can manage this file manually, by calling `cmake` (options for caching).
- You can reset the project to its default configuration by deleting this file; it will be regenerated from the listfiles.

### Options for caching

CMake queries the system for all kinds of information during the configuration stage. This information is cached in `CMakeCache.txt` in the build tree directory.

To prepopulate cached information:

```bash
$ cmake -C <initial-cache-script> <path-to-source>
```

We can provide a path to the CMake script, which (only) contains a list of set() commands to specify variables that will be used to initialize an empty build tree.

To initialize and modify existing cache variables:

```bash
$ cmake -D <var>[:<type>]=<value> <path-to-source>
```

The `:<type>` section is optional (it is used by GUIs); you can use `BOOL`, `FILEPATH`, `PATH`, `STRING`, or `INTERNAL`. If you omit the type, it will be set to the type of an already existing variable; otherwise, it will be set to `UNINITIALIZED`.

We can list cache variables with the `-LA` option:

```bash
$ cmake -LA <path-to-source>
```

The removal of one or more variables can be done with the following option:

```bash
$ cmake -U <globbing_expr> <path-to-source>
```

Here, the globbing expression supports the `*` wildcard and any `?` character symbols. Be careful when using these, as you might break things.

### Options for presets

A `CMakePresets.json` specifies some default options. These values override the system defaults and the environment. However, at the same time, they can be overridden with any arguments that are explicitly passed on the command line (`-D`).

### CMakePresets.json and CMakeUserPresets.json

The configuration of the projects can become a relatively busy task when we need to be specific about things such as cache variables, chosen generators, the path of the build tree, and more.

Presets are stored in the same JSON format in two files:

- `CMakePresets.json`: meant for project authors to provide official presets.
- `CMakeUserPresets.json`: dedicated to users who want to customize the project configuration to their liking.

### [CMake >> Documentation >> cmake-presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html).

One problem that CMake users often face is sharing settings with other people for common ways to configure a project. This may be done to support CI builds, or for users who frequently use the same build. CMake supports two main files, `CMakePresets.json` and `CMakeUserPresets.json`, that allow users to specify common configure options and share them with others. CMake also supports files included with the include field.

`CMakePresets.json` and `CMakeUserPresets.json` live in the project's root directory. They both have exactly the same format, and both are optional (though at least one must be present if `--preset` is specified). `CMakePresets.json` is meant to specify project-wide build details, while `CMakeUserPresets.json` is meant for developers to specify their own local build details.

`CMakePresets.json` may be checked into a version control system, and `CMakeUserPresets.json` should NOT be checked in. For example, if a project is using Git, `CMakePresets.json` may be tracked, and `CMakeUserPresets.json` should be added to the `.gitignore`.

### Ignoring files in Git

The `.gitignore` file looks like this:

```
build/
build_debug/
build_release/

# Generated and user files
**/CMakeCache.txt
**/CMakeUserPresets.json
**/CTestTestfile.cmake
**/CPackConfig.cmake
**/cmake_install.cmake
**/install_manifest.txt
**/compile_commands.json
```

## Discovering scripts and modules

### Scripts

To configure project building, CMake offers a platform-agnostic programming language. This comes with many useful commands. You can use this tool to write scripts that come with your project or are completely independent.

Think of it as a consistent way to do cross-platform work: instead of using bash scripts on Linux and batch or PowerShell scripts on Windows, you can have one version. Sure, you could bring in external tools such as Python, Perl, or Ruby scripts, but this is yet another dependency and will increase the complexity of your C/C++ projects.

We can execute scripts using the `-P` option:

```bash
$ cmake -P script.cmake
```

A script can be as complex as you like or an empty file. However, it is recommended that you call the `cmake_minimum_required()` command at the beginning of the script.

```
cmake_minimum_required(VERSION 3.20.0)
message("Hello world")
file(WRITE Hello.txt "I am writing to a file")
```

When running scripts, CMake won't execute any of the usual stages (such as configuration or generation), and it won't use the cache. Since there is no concept of a source/build tree in scripts, variables that usually hold references to these paths will contain the current working directory instead: `CMAKE_BINARY_DIR`, `CMAKE_SOURCE_DIR`, `CMAKE_CURRENT_BINARY_DIR`, and `CMAKE_CURRENT_SOURCE_DIR`.

### Utility modules

CMake projects can use external modules to enhance their functionality. Modules are written in the CMake language and contain macro definitions, variables, and commands that perform all kinds of functions.

A full list of bundled modules can be found at https://cmake.org/cmake/help/latest/manual/cmake-modules.7.html.
