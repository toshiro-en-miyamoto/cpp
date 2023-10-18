add_library(sut STATIC
  calc.cpp
  run.cpp
)
target_include_directories(sut PUBLIC .)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(unit_tests)
