cmake_minimum_required(VERSION 3.23.0)
project(testing CXX)

add_executable()(ex01 ex01.cpp)
target_link_libraries(ex01 PRIVATE Boost:ut)
