add_executable(vector20_ut vector_ut.cpp)
target_link_libraries(vector20_ut
    PUBLIC incl ut cxx_std
)
add_test(NAME vector20_UT COMMAND vector20_ut)

add_executable(alloc_vector20_ut alloc_vector_ut.cpp)
target_link_libraries(alloc_vector20_ut
    PUBLIC incl ut cxx_std
)
add_test(NAME alloc_vector20_UT COMMAND alloc_vector20_ut)
