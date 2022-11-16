add_library(typesize typesize.cpp)
target_include_directories(typesize
    PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)
target_link_libraries(typesize
    PUBLIC cxx_std
)
