add_executable(x_ut x_ut.cpp)
target_link_libraries(x_ut
    PUBLIC incl ut cxx_std
)

add_test(NAME x_UT COMMAND x_ut)

add_executable(drill_ut drill_ut.cpp)
target_link_libraries(drill_ut
    PUBLIC drill ut cxx_std
)

add_test(NAME drill_UT COMMAND drill_ut)

add_executable(exercise_ut exercise_ut.cpp)
target_link_libraries(exercise_ut
    PUBLIC exercise ut cxx_std
)

add_test(NAME exercise_UT COMMAND exercise_ut)
