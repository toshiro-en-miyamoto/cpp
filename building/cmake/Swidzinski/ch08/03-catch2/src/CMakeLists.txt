add_library(sut STATIC
  calc.cpp
  run.cpp
)
target_include_directories(sut PUBLIC .)
