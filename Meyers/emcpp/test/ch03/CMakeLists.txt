add_executable(item07_ut item07_ut.cpp)
target_link_libraries(item07_ut
    PUBLIC incl ut cxx_std
)
add_test(NAME item07_UT COMMAND item07_ut)
