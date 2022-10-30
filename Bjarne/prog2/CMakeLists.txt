cmake_minimum_required(VERSION 3.23)
project(microTestTutorial CXX)
enable_testing()

add_library(cxx_std INTERFACE)
target_compile_features(cxx_std INTERFACE cxx_std_20)

add_subdirectory(main)
add_subdirectory(test)
