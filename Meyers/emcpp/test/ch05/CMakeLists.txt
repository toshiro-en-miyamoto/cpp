add_executable(item23_ut item23_ut.cpp)
target_link_libraries(item23_ut
    PUBLIC incl ut cxx_std
)
add_test(NAME item23_UT COMMAND item23_ut)

add_executable(item24_ut item24_ut.cpp)
target_link_libraries(item24_ut
    PUBLIC incl ut cxx_std
)
add_test(NAME item24_UT COMMAND item24_ut)
