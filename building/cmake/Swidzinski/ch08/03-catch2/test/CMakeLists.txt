include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.0.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(unit_tests
  clac_test.cpp
  run_test.cpp
)
target_link_libraries(unit_tests PRIVATE
  sut
  Catch2::Catch2WithMain
)
