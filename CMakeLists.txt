cmake_minimum_required(VERSION 3.10)
project(IMAGE_REDACT)

add_executable(my_program start.cpp image_operations.cpp )

find_package(OpenCV REQUIRED)
target_link_libraries(my_program PRIVATE ${OpenCV_LIBS})
