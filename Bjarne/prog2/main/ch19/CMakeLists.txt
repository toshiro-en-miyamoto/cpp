add_library(vector19 vector.cpp)
target_include_directories(vector19
    PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)
target_link_libraries(vector19
    PUBLIC cxx_std
)
