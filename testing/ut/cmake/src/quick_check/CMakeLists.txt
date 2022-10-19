cmake_minimum_required(VERSION 3.23.0)
project(quick_check CXX)

add_executable(a.out main.cpp)

try_run(run_result compile_result
    ${CMAKE_BINARY_DIR}/quick_check/test_output
    ${CMAKE_SOURCE_DIR}/quick_check/main.cpp
    RUN_OUTPUT_VARIABLE output
)
message("run_result: ${run_result}")
message("compile_result: ${compile_result}")
message("output:\n\n${output}")
