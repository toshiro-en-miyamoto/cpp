add_executable(x_ut x_ut.cpp)
target_link_libraries(x_ut
    PUBLIC incl ut cxx_std
)

add_test(NAME x_UT COMMAND x_ut)

add_executable(drill18_ut drill_ut.cpp)
target_link_libraries(drill18_ut
    PUBLIC drill18 ut cxx_std
)

add_test(NAME drill18_UT COMMAND drill18_ut)

add_executable(exercise18_ut exercise_ut.cpp)
target_link_libraries(exercise18_ut
    PUBLIC exercise18 ut cxx_std
)

add_test(NAME exercise18_UT COMMAND exercise18_ut)
