add_executable(uthello hello_test.cpp)
target_link_libraries(uthello
    PUBLIC hello ut cxx_std
)
