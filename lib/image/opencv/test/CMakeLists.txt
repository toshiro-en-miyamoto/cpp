add_library(doctest INTERFACE)
target_sources(doctest INTERFACE
  FILE_SET HEADERS
)

add_executable(hello hello.cpp)
target_link_libraries(hello
  PUBLIC doctest
)
