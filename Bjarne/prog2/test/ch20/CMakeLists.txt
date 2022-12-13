add_executable(vector20_ut vector_ut.cpp)
target_link_libraries(vector20_ut
    PUBLIC incl ut cxx_std
)
add_test(NAME vector20_UT COMMAND vector20_ut)

add_executable(uniq_vector20_ut uniq_vector_ut.cpp)
target_link_libraries(uniq_vector20_ut
    PUBLIC incl ut cxx_std
)
add_test(NAME uniq_vector20_UT COMMAND uniq_vector20_ut)
