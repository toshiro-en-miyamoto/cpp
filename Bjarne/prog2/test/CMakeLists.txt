add_library(ut INTERFACE)
target_sources(ut INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(ch17)
