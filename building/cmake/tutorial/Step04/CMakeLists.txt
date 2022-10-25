cmake_minimum_required(VERSION 3.10)
project(Tutorial VERSION 1.0)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_11)

configure_file(TutorialConfig.h.in TutorialConfig.h)

add_subdirectory(MathFunctions)

add_executable(Tutorial tutorial.cxx)

# TODO 2: Link to tutorial_compiler_flags

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

target_include_directories(Tutorial PUBLIC
    "${PROJECT_BINARY_DIR}"
)
