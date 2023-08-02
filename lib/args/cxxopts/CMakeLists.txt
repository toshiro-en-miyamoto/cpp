cmake_minimum_required(VERSION 3.26)
project(nlohmann_json_tutorial CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(cxxopts REQUIRED)
find_package(doctest REQUIRED)

add_executable(example src/main.cpp)
target_link_libraries(example cxxopts::cxxopts)
target_link_libraries(example doctest::doctest)
