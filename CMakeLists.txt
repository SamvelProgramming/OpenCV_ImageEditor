cmake_minimum_required(VERSION 3.10)
project(IMAGE_REDACT)

find_package(OpenCV REQUIRED)

add_executable(my_program start.cpp image_operations.cpp utils.cpp)

target_link_libraries(my_program PRIVATE ${OpenCV_LIBS}) 

enable_testing()  #Enable testing for the project

find_package(GTest REQUIRED) #Find the GTest package

add_executable(image_redactor_tests test.cpp image_operations.cpp utils.cpp) #Create a test executable

target_link_libraries(image_redactor_tests 
    PRIVATE
    ${OpenCV_LIBS}
    GTest::gtest              # Provides the core GoogleTest framework (assertions like EXPECT_EQ, ASSERT_TRUE, etc.).
    GTest::gtest_main         # Provides a default main() entry point supplied by GoogleTest, so you don't have to write a main() function inside test.cpp
    )                         # Link the test executable with OpenCV and GTest libraries

include(GoogleTest)           # Include the GoogleTest module
gtest_discover_tests(image_redactor_tests)  # Automatically discover and register tests in the test executable  