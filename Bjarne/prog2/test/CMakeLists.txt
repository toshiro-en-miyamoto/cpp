add_library(ut INTERFACE)
target_sources(ut INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(sniff sniff.cpp)
target_link_libraries(sniff
    PUBLIC incl cxx_std vector19
)

add_subdirectory(ch17)
add_subdirectory(ch18)
