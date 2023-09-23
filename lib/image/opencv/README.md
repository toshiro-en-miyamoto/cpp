# Using OpenCV with Conan and CMake

The easiest way of using OpenCV in your code is to use CMake. How to use OpenCV with CMake is described [here](https://docs.opencv.org/4.x/db/df5/tutorial_linux_gcc_cmake.html).

`conanfile.txt`:

```
[requires]
opencv/4.5.5
doctest/2.4.11

[generators]
CMakeDeps
CMakeToolchain

[layout]
cmake_layout
```

## Conan profiles

Two Conan profiles; `release` and `debug`:

```
(conan2) cd ~/.conan2/profiles/
(conan2) profiles % cat release
[settings]
arch=x86_64
build_type=Release
compiler=apple-clang
compiler.cppstd=20
compiler.libcxx=libc++
compiler.version=14
os=Macos

(conan2) profiles % diff release debug
3c3
< build_type=Release
---
> build_type=Debug
```

## Installing dependencies

Install (and build if necessary; `--build=missing`) dependencies in the Release and Debug configurations:

```bash
(conan2) proj % conan install . --build=missing --profile=release
(conan2) proj % conan install . --build=missing --profile=debug
```

## Specifying OpenCV in `CMakeList.txt`

A simple use case using the CMake file name and the global target:

```cmake
cmake_minimum_required(VERSION 2.8)
project( DisplayImage )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
find_package(doctest REQUIRED)

add_executable( DisplayImage DisplayImage.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )

target_link_libraries(YOUR_TARGET doctest::doctest)
```

Then you can include the header file as follows: Let's use a simple program such as `DisplayImage.cpp` shown below.

```c++
#include <opencv2/opencv.hpp>

int main(int argc, char** argv)
{
  cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);
  if (!image.data) return -1;
  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
  cv::imshow("Display Image", image);
  cv::waitKey(0);
}
```

## Building the project using `CMakePresets`

Build the project (using a single-configuration generator):

```bash
(conan2) proj % cmake --preset conan-release
(conan2) proj % cmake --build --preset conan-release

(conan2) proj % cmake --preset conan-debug  
(conan2) proj % cmake --build --preset conan-debug

(conan2) cmake % build/Debug/test/components/zzz/zzz_ut
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases: 1 | 1 passed | 0 failed | 0 skipped
[doctest] assertions: 5 | 5 passed | 0 failed |
[doctest] Status: SUCCESS!
```
