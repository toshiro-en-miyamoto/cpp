add_executable(vector19_ut vector_ut.cpp)
target_link_libraries(vector19_ut
    PUBLIC incl ut cxx_std
)

add_test(NAME vector19_UT COMMAND vector19_ut)

add_executable(vector_ptr19_ut vector_ptr_ut.cpp)
target_link_libraries(vector_ptr19_ut
    PUBLIC incl ut cxx_std
)

add_test(NAME vector_ptr19_UT COMMAND vector_ptr19_ut)

add_executable(drill19_ut drill_ut.cpp)
target_link_libraries(drill19_ut
    PUBLIC incl ut cxx_std
)

add_test(NAME drill19_UT COMMAND drill19_ut)

add_executable(exercise19_ut exercise_ut.cpp)
target_link_libraries(exercise19_ut
    PUBLIC incl ut cxx_std
)

add_test(NAME exercise19_UT COMMAND exercise19_ut)

add_executable(uniq_file_ut uniq_file_ut.cpp)
target_link_libraries(uniq_file_ut
    PUBLIC incl ut cxx_std
)

add_test(NAME uniq_file_UT COMMAND uniq_file_ut)
