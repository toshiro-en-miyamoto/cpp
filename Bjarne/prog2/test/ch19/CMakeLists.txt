add_executable(vector19_ut vector_ut.cpp)
target_link_libraries(vector19_ut
    PUBLIC incl ut cxx_std
)

add_test(NAME vector19_UT COMMAND vector19_ut)
