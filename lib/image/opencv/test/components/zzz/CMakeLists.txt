add_executable(zzz_ut factorial_ut.cpp)
target_link_libraries(zzz_ut doctest::doctest)
target_link_libraries(zzz_ut zzz)

add_test(NAME zzz_ut COMMAND zzz_ut)
