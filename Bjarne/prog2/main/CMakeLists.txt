add_library(incl INTERFACE)
target_sources(incl INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(ch17)
