add_library(hello hello.cpp)
target_include_directories(hello
    PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)
