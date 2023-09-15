cmake_minimum_required(VERSION 3.23)
project(docteestTutorial CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(main bin)
add_subdirectory(test)
