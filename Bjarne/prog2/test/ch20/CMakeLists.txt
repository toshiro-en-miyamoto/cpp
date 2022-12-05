add_executable(vector20_ut vector_ut.cpp)
target_link_libraries(vector20_ut
    PUBLIC incl ut cxx_std
)

add_test(NAME vector20_UT COMMAND vector20_ut)
