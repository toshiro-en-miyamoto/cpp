add_executable(item01_ut item01_ut.cpp)
target_link_libraries(item01_ut
    PUBLIC incl ut cxx_std
)
add_test(NAME item01_UT COMMAND item01_ut)

add_executable(item02_ut item02_ut.cpp)
target_link_libraries(item02_ut
PUBLIC incl ut cxx_std
)
add_test(NAME item02_UT COMMAND item02_ut)
