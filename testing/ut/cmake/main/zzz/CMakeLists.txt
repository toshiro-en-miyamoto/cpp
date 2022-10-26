add_library(Hello hello.cpp)
target_include_directories(Hello
    PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)
