cmake_minimum_required(VERSION 3.26)
project(nlohmann_json_tutorial CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(doctest REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(tutorial1 src/tutorial1.cpp)
target_link_libraries(tutorial1 doctest::doctest)
target_link_libraries(tutorial1 nlohmann_json::nlohmann_json)
