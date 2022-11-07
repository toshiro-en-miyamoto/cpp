add_executable(x_ut x_ut.cpp)
target_link_libraries(x_ut
    PUBLIC incl ut cxx_std
)

add_test(NAME x_UT COMMAND x_ut)
