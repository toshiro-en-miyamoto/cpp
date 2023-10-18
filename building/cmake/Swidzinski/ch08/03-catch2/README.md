# Catch2

`test/CMakeLists.txt` uses `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(unit_tests
  calc_test.cpp
  run_test.cpp
)
target_link_libraries(unit_tests PRIVATE
  sut
  Catch2::Catch2WithMain
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(unit_tests)
```

According to [Catch2: CMake integration](https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md), the `find_package` call is provided:

```cmake
find_package(Catch2 3 REQUIRED)

add_executable(unit_tests
  calc_test.cpp
  run_test.cpp
)
target_link_libraries(unit_tests PRIVATE
  sut
  Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(unit_tests)
```

When using `FetchContent`, `include(Catch)` will fail unless `CMAKE_MODULE_PATH` is explicitly updated to include the `extras` directory.
