# Working with Targets

## Targets

All that's required is an `add_executable()` command with the name of the executable target and a list of the files that are to be its elements:

```cmake
add_executable(app1 a.cpp b.cpp c.cpp)
```

In CMake, we can create a target using one of three commands:

- `add_executable()`
- `add_library()`
- `add_custom_target()`

*Custom targets* allow you to specify your own command line that will be executed *without checking whether the produced output is up to date*.
```

### Dependency graph

In the `BankApp` project, we have

- two libraries: `calculations`, `drawing`
- two executables: `terminal_app`, `gui_app`
- a custom target: `checksum`

```
user:
    terminal_app
    gui_app
terminal_app:
    calculations
gui_app:
    calculations
    drawing
checksum:
    gui_app
    terminal_app
```

CMake is pretty flexible when it comes to writing list files for such a solution:

```cmake
cmake_minimum_required(VERSION 3.20.0)
project(BankApp CXX)

add_executable(terminal_app terminal_app.cpp)
add_executable(gui_app gui_app.cpp)

target_link_libraries(terminal_app calculations)
target_link_libraries(gui_app calculations drawing)

add_library(calculations calculations.cpp)
add_library(drawing drawing.cpp)

add_custom_target(checksum ALL
    COMMAND sh -c "cksum terminal_app > terminal.ck"
    COMMAND sh -c "cksum gui_app > gui.ck"
    BYPRODUCTS terminal.ck gui.ck 
    COMMENT "Checking the sums..."
)

add_dependencies(checksum terminal_app gui_app)
```

The `target_link_libraries()` command connect our libraries with executables. Without it, the compilation for executables would fail because of undefined symbols. The command is invoked before actually declaring any of the libraries.

The `add_dependencies()` command at the end of the file garantees that the `checksum` target will be built after executables. Otherwise, it's free to start building the `checksum` first.

- `target_link_libraries()`: is used with actual libraries and allows you to control property propagation
- `add_dependencies()`: is meant to be used only with top-level targets to set their build order

## Commands

## Generator
