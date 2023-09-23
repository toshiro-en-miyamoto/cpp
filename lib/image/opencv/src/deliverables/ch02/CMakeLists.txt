add_executable(shimg show_image.cpp)
target_link_libraries(shimg cxxopts::cxxopts)
target_link_libraries(shimg ${OpenCV_LIBRARIES})
