cmake_minimum_required(VERSION 3.10)
project(IMAGE_REDACT)

find_package(OpenCV REQUIRED)
add_executable(my_program start.cpp image_operations.cpp )

target_link_libraries(my_program PRIVATE ${OpenCV_LIBS})
