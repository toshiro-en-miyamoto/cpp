add_executable(binary_search_ut binary_search_ut.cpp)
target_link_libraries(binary_search_ut
    PUBLIC incl ut cxx_std
)

add_test(NAME binary_search_UT COMMAND binary_search_ut)
