cmake_minimum_required(VERSION 3.23)
project(microTestTutorial CXX)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)

add_subdirectory(main bin)
