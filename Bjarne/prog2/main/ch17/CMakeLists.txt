add_library(typesize INTERFACE)
target_sources(typesize INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/main/include
)
