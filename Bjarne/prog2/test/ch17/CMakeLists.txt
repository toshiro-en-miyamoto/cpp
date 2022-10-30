add_executable(typesize_ut typesize_test.cpp)
target_link_libraries(typesize_ut
    PUBLIC typesize ut cxx_std
)

add_test(NAME type_size_UT COMMAND typesize_ut)
