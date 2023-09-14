cmake_minimum_required(VERSION 3.27)
project(opencv_tutorial CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(opencv REQUIRED)
find_package(doctest REQUIRED)

add_subdirectory(src)
