cmake_minimum_required(VERSION 3.26)
project(test CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(doctest REQUIRED)

add_executable(example src/main.cpp)
target_link_libraries(example doctest::doctest)
