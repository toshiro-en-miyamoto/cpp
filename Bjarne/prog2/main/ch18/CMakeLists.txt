add_library(drill drill.cpp)
target_include_directories(drill
    PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)

add_library(exercise exercise.cpp)
target_include_directories(exercise
    PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)

add_executable(exercise_main exercise_main.cpp)
target_link_libraries(exercise_main
    PUBLIC exercise cxx_std
)

add_library(vector vector.cpp)
target_include_directories(vector
    PUBLIC ${PROJECT_SOURCE_DIR}/main/include
)
