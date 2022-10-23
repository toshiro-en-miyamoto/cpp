cmake_minimum_required(VERSION 3.10)
project(Tutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# TODO 7: Create a variable MY_MATH using option and set default to ON

configure_file(TutorialConfig.h.in TutorialConfig.h)

# TODO 8: Use list() and APPEND to create a list of optional libraries
# called  EXTRA_LIBS and a list of optional include directories called
# EXTRA_INCLUDES. Add the MathFunctions library and source directory to
# the appropriate lists.
#
# Only call add_subdirectory and only add MathFunctions specific values
# to EXTRA_LIBS and EXTRA_INCLUDES if USE_MYMATH is true.

add_subdirectory(MathFunctions)

add_executable(Tutorial tutorial.cxx)

# TODO 9: Use EXTRA_LIBS instead of the MathFunctions specific values
# in target_link_libraries.

target_link_libraries(Tutorial PUBLIC MathFunctions)

# TODO 10: Use EXTRA_INCLUDES instead of the MathFunctions specific values
# in target_include_directories.

target_include_directories(Tutorial PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/MathFunctions"
)
