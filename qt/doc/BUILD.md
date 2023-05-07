# Installing Qt by building the Qt sources

- [Building Qt Sources](https://doc.qt.io/qt-6/build-sources.html)

## [Qt Configure Options](https://doc.qt.io/qt-6/configure-options.html)

To build Qt from source, use `configure` to configure Qt for a particular platform with a particular set of Qt features or modules.

Since Qt 6, `configure` is a wrapper around cmake. CMake can also be invoked directly.

Note that CMake caches configure options and information about the environment in a file called `CMakeCache.txt`. Delete this file when you want to start over without cached information.

### Source, Build, and Install Directories

It's recommended to keep these directories separate by shadow-building and using the `-prefix` option.

- The *source* directory contains the source code that is obtained from the source package or git repository.
- The *build* directory is where the build-related files, such as build system files, object files, and other intermediate files are stored.
- The *install* directory is where the binaries and libraries are installed, for use either by the system or by the application.

To shadow-build, run `configure` from a separate directory:

```bash
mkdir ~/qt6/build
cd ~/qt6/build
~/qt6/source/configure -prefix /usr/local/qt6
```

Configuring with the `-prefix` option means that the Qt binaries and libraries are *installed* into another directory, which is `/usr/local/qt6` in this case.

### Build Configurations

Debug and Release Builds:
- `-release` tells the compiler to optimize the code, and not provide additional debug symbols alongside Qt and its tools.
- `-debug` enables the generation of debug symbols that let you inspect the code and state of the Qt libraries in a debugger.

Static and Shared Builds:
- Qt Modules can be built as separate libraries that an executable links to and loads at start time (for Qt libraries), or runtime (for Qt plugins). This is called a shared build and is the **default** configuration on most platforms. (`-dynamic`)
- You can also build Qt such that an executable binary will include all Qt modules it links to and all Qt plugins it needs. This is called a static build. (`-static`)

### CMake Generators

Qt's `configure` always uses the Ninja generator and build tool if a ninja executable is available. Use of other generators might work, but is not officially supported.

A [CMake Generator](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html) is responsible for writing the input files for a native build system. Exactly one of the CMake Generators must be selected for a build tree to determine what native build system is to be used.

CMake Generators are platform-specific so each may be available only on certain platforms. The `cmake` command-line tool `--help` output lists available generators on the current platform.

With CMake 3.26.3 on Debian Bullseye, the following generators are available (* marks default) among others:

| generator             | generates
|-----------------------|--------------
| Unix Makefiles (*)    | standard UNIX makefiles
| Ninja                 | `build.ninja` files
| Ninja Multi-Config    | `build-<Config>.ninja` files

## [Qt for X11 Requirements](https://doc.qt.io/qt-6/linux-requirements.html)

On Linux, the xcb QPA (Qt Platform Abstraction) platform plugin is used. It provides the basic functionality needed by Qt GUI and Qt Widgets to run against X11. Its library dependencies are described the following table. To build Qt from its source code, you will also need to install the development packages for these libraries for your system.

| Library             | min   | now    | * | Notes
|---------------------|-------|--------|---|-------
| libfontconfig1      | 2.6   | 2.13.1 | Y | Font configuration
| libfreetype6        | 2.3.0 | 2.10.4 | Y | Font engine
| libice6             | 6.3.5 | 1.0.10 | Y | Inter-Client Exchange
| libglib2.0-0        | 2.8.3 | 2.66.8 | Y | Common event loop
| libpthread          | 2.3.5 |        | - | Multithreading
| libboost-thread1.74.0 |     | 1.74.0 | Y | Portable C++ multithread
| libsm6              | 6.0.4 | 1.2.3  | Y | X Session Management
| libx11-6            | 6.2.1 | 7.2.1  | Y | X11 client-side lib
| libx11-xcb1         | 1.3.2 | 1.7.2  | Y | Xlib/XCB interface
| libxcb1             | 1.11  | 1.14   | Y | X C Binding library
| libxcb-cursor0      | 0.1.1 | 0.1.1  | - | XCB for cursor
| libwayland-cursor0  |       | 1.18.0 | Y | Wayland compositor Cursor
| libxcursor1         |       | 1.2.0  | Y | X cursor management
| libxcb-glx0         |       | 1.14   | Y | XCB for glx
| libxcb-icccm4       | 0.3.9 | 0.4.1  | Y | XCB for ICCCM
| libxcb-image0       | 0.3.9 | 0.4.0  | Y | XCB for XImage
| libxcb-keysyms1     | 0.3.9 | 0.4.0  | Y | XCB for keycode conv
| libxcb-randr0       | 1.11  | 1.14   | Y | XCB for Resize/Rotate
| libxcb-render0      | 1.11  | 1.14   | Y | XCB for Render
| libxcb-render-util0 | 0.3.9 | 0.3.9  | Y | XCB for Render
| libxcb-shape0       | 1.11  | 1.14   | Y | XCB for Shape
| libxcb-shm0         | 1.11  | 1.14   | Y | XCB for Shared Memory
| libxcb-sync1        | 1.11  | 1.14   | Y | XCB for Sync
| libxcb-util1        | 0.3.9 | 0.4.0  | Y | XCB for atom/aux/event
| libxcb-xfixes0      | 1.11  | 1.14   | Y | XCB for Fixes
| libxcb-xinerama0    |       | 1.14   | Y | XCB for Xinerama
| libxcb-xkb1         | 1.11  | 1.14   | Y | XCB for XKeyboard
| libxext6            | 6.4.3 | 1.3.3  | Y | X Extensions
| libxfixes3          |       | 5.0.3  | Y | X11 misc fixes
| libxi6              |       | 1.7.10 | Y | X11 Input extension
| libxkbcommon0       | 0.5.0 | 1.0.3  | Y | Keymap handling
| libxkbcommon-x11-0  | 0.5.0 | 1.0.3  | Y | Keymap handling
| libxrender1         | 0.9.0 | 0.9.10 | Y | X Rendering Extension

Development packages for these libraries contain header files that are used when building Qt from its source code. On Debian-based GNU/Linux systems, for example, we recommend that you install the following development packages:

| Library                 | min   | now    | * | Notes
|-------------------------|-------|--------|---|-------
| libfontconfig1-dev      |       | 2.13.1 | - |
| libfreetype6-dev        |       | 2.10.4 | - |
| libx11-dev              |       | 1.7.2  | - |
| libx11-xcb-dev          |       | 1.7.2  | - |
| libxcb1-dev             |       | 1.14   | - |
| libxcb-cursor-dev       |       | 0.1.1  | - |
| libxcb-glx0-dev         |       | 1.14   | - |
| libxcb-icccm4-dev       |       | 0.4.1  | - |
| libxcb-image0-dev       |       | 0.4.0  | - |
| libxcb-keysyms1-dev     |       | 0.4.0  | - |
| libxcb-randr0-dev       |       | 1.14   | - |
| libxcb-render-util0-dev |       | 0.3.9  | - |
| libxcb-shape0-dev       |       | 1.14   | - |
| libxcb-shm0-dev         |       | 1.14   | - |
| libxcb-sync-dev         |       | 1.14   | - |
| libxcb-util-dev         |       | 0.4.0  | - |
| libxcb-xfixes0-dev      |       | 1.14   | - |
| libxcb-xinerama0-dev    |       | 1.14   | - |
| libxcb-xkb-dev          |       | 1.14   | - |
| libxext-dev             |       | 1.3.3  | - |
| libxfixes-dev           |       | 5.0.3  | - |
| libxi-dev               |       | 1.7.10 | - |
| libxkbcommon-dev        |       | 1.0.3  | - |
| libxkbcommon-x11-dev    |       | 1.0.3  | - |
| libxrender-dev          |       | 0.9.10 | - |

### [Installing Clang for QDoc](https://doc.qt.io/qt-6/qdoc-guide-clang.html)

QDoc uses Clang when parsing C++ files as well as function signatures in `\fn` commands. If you're going to build QDoc from source, you must install Clang 8.0 or later.

Linux distributions often provide a package called `libclang-dev`. Qt's build system instructs CMake to look for `ClangConfig.cmake`, so make sure you install the package that provides this file if you want to build QDoc. Running QDoc requires only `libclang`.

| Library                 | min   | now    | * | Notes
|-------------------------|-------|--------|---|-------
| libclang1-13            |       | 13.0.1 | Y | Clang 13
| libclang-13-dev         |       | 13.0.1 | - |

## [Qt for Linux/X11](https://doc.qt.io/qt-6/linux-building.html)

[How to Build Qt 6.4.0 from Source on Ubuntu Linux](https://www.ics.com/blog/how-build-qt-640-source-ubuntu-linux)

### Step1: Getting the Sources

Source packages are also available as Git repositories, as archives in the Qt Account (commercial users), and on [download.qt.io](https://download.qt.io/official_releases/qt/6.5/6.5.0/) (open-source users).

| official_releases/qt/6.5/6.5.0/   | Last modified     | Size 
|-----------------------------------|-------------------|------
| `submodules/`                     | 02-Apr-2023 10:42 | -
| `single/`                         | 02-Apr-2023 10:44 | -
| `OFFLINE_README.txt`              | 02-Apr-2023 10:33 | 357

| qt/6.5/6.5.0/single               | Last modified     | Size 
|-----------------------------------|-------------------|------
| `qt-everywhere-src-6.5.0.zip`     | 02-Apr-2023 10:35 | 1.2G
| `qt-everywhere-src-6.5.0.tar.xz`  | 02-Apr-2023 10:34 | 755M
| `md5sums.txt`                     | 02-Apr-2023 10:44 | 127

```bash
cd ~/qt6
tar xf ~/Downloads/qt-everywhere-src-6.5.0.tar.xz
```

### Step 2: Install Build Requirements

```bash
$ cmake --version
cmake version 3.26.3

$ cmake --help
Generators
* Unix Makefiles    = Generates standard UNIX makefiles.
  Ninja             = Generates build.ninja files.

$ python --version
Python 3.9.2
```
