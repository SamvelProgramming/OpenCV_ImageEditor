#include <gtest/gtest.h>
#include "image_operations.h"
#include <opencv2/opencv.hpp>
using namespace OpencvFunctions;

TEST(ImageOperationTest, GrayscaleConversion){
    cv::Mat input(100, 100, CV_8UC3, cv::Scalar(255, 0, 0)); // Blue image
    cv::Mat output;
    
    applyGrey(input, output);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.channels(), 1);
    EXPECT_EQ(output.rows, 100);
    EXPECT_EQ(output.cols, 100);
    EXPECT_EQ(output.at<uchar>(0, 0), 29); // Grayscale value for blue
}

TEST(ImageOperationTest, Rotate90){
    cv::Mat input = cv::Mat::zeros(100, 200, CV_8UC3);
    cv::Mat output;
    
    applyRotate90(input, output);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 200);
    EXPECT_EQ(output.cols, 100);
}
TEST(ImageOperationTest, Blur){
    cv::Mat input = cv::Mat::zeros(100, 100, CV_8UC3);
    input.at<cv::Vec3b>(50, 50) = cv::Vec3b(255, 255, 255); // Set a white pixel in the center
    cv::Mat output;
    
    applyBlur(input, output);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 100);
    EXPECT_EQ(output.cols, 100);
    EXPECT_GT(output.at<cv::Vec3b>(50, 50)[0], 0); // The pixel should be blurred, so it won't be pure white
}
TEST(ImageOperationTest, Rotate180){
    cv::Mat input = cv::Mat::zeros(100, 200, CV_8UC3);
    cv::Mat output; 
    applyRotate180(input, output);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 100);
    EXPECT_EQ(output.cols, 200);
}
TEST(ImageOperationTest, cannyEdgeDetection){
    cv::Mat input = cv::Mat::zeros(100, 200, CV_8UC3);
    cv::rectangle(input, cv::Point(50, 25), cv::Point(150, 75), cv::Scalar(255, 255, 255), -1); // Draw a white rectangle
    cv::Mat output; 
    applyCanny(input, output, 50, 150);

    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 100);    
    EXPECT_EQ(output.cols, 200);
    EXPECT_EQ(output.channels(), 1); 
    EXPECT_EQ(output.type(), CV_8UC1);
    EXPECT_GT(cv::countNonZero(output), 0);
    EXPECT_EQ(output.at<uchar>(0, 0), 0);
    EXPECT_EQ(output.at<uchar>(25, 50), 255);
}
TEST(ImageOperationTest, Resize){
    cv::Mat input = cv::Mat::zeros(100, 200, CV_8UC3);
    cv::Mat output; 
    double scaleFactor = 0.5;
    applyResize(input, output, scaleFactor);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 50);    
    EXPECT_EQ(output.cols, 100);
}
TEST(ImageOperationTest, Morphology){
    cv::Mat input = cv::Mat::zeros(100, 200, CV_8UC3); // Create a black image
    cv::rectangle(input, cv::Point(50, 25), cv::Point(150, 75), cv::Scalar(255, 255, 255), -1); // Draw a white rectangle in the middle
    cv::Mat output;

    applyMorphology(input, output, 0, 3); // Erosion with kernel size 3
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 100);
    EXPECT_EQ(output.cols, 200);
    EXPECT_EQ(output.channels(), 3);
    EXPECT_EQ(output.at<cv::Vec3b>(0, 0), cv::Vec3b(0, 0, 0)); // The pixel should remain black after erosion
    EXPECT_EQ(output.at<cv::Vec3b>(25, 50), cv::Vec3b(0, 0, 0)); // The pixel should remain black after erosion
}
TEST(ImageOperationTest, Flip){
    cv::Mat input = cv::Mat::zeros(100, 200, CV_8UC3);
    input.at<cv::Vec3b>(0, 0) = cv::Vec3b(255, 255, 255); // Set a white pixel in the top-left corner
    cv::Mat output; 
    applyFlip(input, output);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 100);    
    EXPECT_EQ(output.cols, 200);
    EXPECT_EQ(output.at<cv::Vec3b>(0, 0), cv::Vec3b(0, 0, 0)); // The top-left pixel should now be black
    EXPECT_EQ(output.at<cv::Vec3b>(0, 199), cv::Vec3b(255, 255, 255)); // The top-right pixel should still be white
}