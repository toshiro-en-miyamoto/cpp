cmake_minimum_required(VERSION 3.20.0)
project(Catch2Tests CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_subdirectory(src bin)
add_subdirectory(test)
