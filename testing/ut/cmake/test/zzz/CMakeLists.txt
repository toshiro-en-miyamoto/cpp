add_executable(hello_ut hello_test.cpp)
target_link_libraries(hello_ut
    PUBLIC hello ut cxx_std
)

add_test(NAME Hello_UT COMMAND hello_ut)
