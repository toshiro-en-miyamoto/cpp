add_library(drill18 drill.cpp)
target_include_directories(drill18
    PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)
target_link_libraries(drill18
    PUBLIC cxx_std
)

add_library(exercise18 exercise.cpp)
target_include_directories(exercise18
    PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)
target_link_libraries(exercise18
    PUBLIC cxx_std
)

add_library(vector18 vector.cpp)
target_include_directories(vector18
    PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)
target_link_libraries(vector18
    PUBLIC cxx_std
)
