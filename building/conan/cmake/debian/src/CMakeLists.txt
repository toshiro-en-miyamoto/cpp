add_library(sut STATIC
  factorial.cpp
)
target_include_directories(sut PUBLIC .)
