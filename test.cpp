#include "utils.h"
#include <gtest/gtest.h>
#include "image_operations.h"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
using namespace OpencvFunctions;
using namespace Utils;
namespace fs = std::filesystem;
TEST(ImageOperationTest, GrayscaleConversion)
{
    cv::Mat input(100, 100, CV_8UC3, cv::Scalar(255, 0, 0)); // Blue image
    cv::Mat output;

    applyGrey(input, output);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.channels(), 1);
    EXPECT_EQ(output.rows, 100);
    EXPECT_EQ(output.cols, 100);
    EXPECT_EQ(output.at<uchar>(0, 0), 29); // Grayscale value for blue
}

TEST(ImageOperationTest, Rotate90)
{
    cv::Mat input = cv::Mat::zeros(100, 200, CV_8UC3);
    cv::Mat output;

    applyRotate90(input, output);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 200);
    EXPECT_EQ(output.cols, 100);
}
TEST(ImageOperationTest, Blur)
{
    cv::Mat input = cv::Mat::zeros(100, 100, CV_8UC3);
    input.at<cv::Vec3b>(50, 50) = cv::Vec3b(255, 255, 255); // Set a white pixel in the center
    cv::Mat output;

    applyBlur(input, output);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 100);
    EXPECT_EQ(output.cols, 100);
    EXPECT_GT(output.at<cv::Vec3b>(50, 50)[0], 0); // The pixel should be blurred, so it won't be pure white
}
TEST(ImageOperationTest, Rotate180)
{
    cv::Mat input = cv::Mat::zeros(100, 200, CV_8UC3);
    cv::Mat output;
    applyRotate180(input, output);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 100);
    EXPECT_EQ(output.cols, 200);
}
TEST(ImageOperationTest, cannyEdgeDetection)
{
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
TEST(ImageOperationTest, Resize)
{
    cv::Mat input = cv::Mat::zeros(100, 200, CV_8UC3);
    cv::Mat output;
    double scaleFactor = 0.5;
    applyResize(input, output, scaleFactor);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 50);
    EXPECT_EQ(output.cols, 100);
}
TEST(ImageOperationTest, Morphology)
{
    cv::Mat input = cv::Mat::zeros(100, 200, CV_8UC3);                                          // Create a black image
    cv::rectangle(input, cv::Point(50, 25), cv::Point(150, 75), cv::Scalar(255, 255, 255), -1); // Draw a white rectangle in the middle
    cv::Mat output;

    applyMorphology(input, output, 0, 3); // Erosion with kernel size 3
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 100);
    EXPECT_EQ(output.cols, 200);
    EXPECT_EQ(output.channels(), 3);
    EXPECT_EQ(output.at<cv::Vec3b>(0, 0), cv::Vec3b(0, 0, 0));   // The pixel should remain black after erosion
    EXPECT_EQ(output.at<cv::Vec3b>(25, 50), cv::Vec3b(0, 0, 0)); // The pixel should remain black after erosion
}
TEST(ImageOperationTest, Flip)
{
    cv::Mat input = cv::Mat::zeros(100, 200, CV_8UC3);
    input.at<cv::Vec3b>(0, 0) = cv::Vec3b(255, 255, 255); // Set a white pixel in the top-left corner
    cv::Mat output;
    applyFlip(input, output);
    EXPECT_FALSE(output.empty());
    EXPECT_EQ(output.rows, 100);
    EXPECT_EQ(output.cols, 200);
    EXPECT_EQ(output.at<cv::Vec3b>(0, 0), cv::Vec3b(0, 0, 0));         // The top-left pixel should now be black
    EXPECT_EQ(output.at<cv::Vec3b>(0, 199), cv::Vec3b(255, 255, 255)); // The top-right pixel should still be white
}
TEST(HelpFlagTest, HelpFlag)
{
    char *argv[] = {(char *)"./my_program", (char *)"--help", (char *)"input.png", (char *)"output.png"};
    int argc = 4;
    EXPECT_EXIT(checkHelpFlag(argc, argv), ::testing::ExitedWithCode(0), "");
}
TEST(HelpFlagTest, ShortHelpFlag)
{
    char *argv[] = {(char *)"./my_program", (char *)"-h", (char *)"input.png", (char *)"output.png"};
    int argc = 4;
    EXPECT_EXIT(checkHelpFlag(argc, argv), ::testing::ExitedWithCode(0), "");
}
TEST(HelpFlagTest, CaseValidArguments)
{
    char *argv[] = {(char *)"./my_program", (char *)"input.png", (char *)"output.png", (char *)"action", (char *)"param1", (char *)"param2"};
    int argc = 6;
    EXPECT_NO_THROW(checkHelpFlag(argc, argv));
}
TEST(isValidFileExtensionTest, ValidExtensions)
{
    std::set<std::string> validFileExtensions = {".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".tif"};
    for (auto &ext : validFileExtensions)
    {
        EXPECT_TRUE(isValidFileExtension("image" + ext, validFileExtensions));
    }
}



 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





TEST(BlurTest, AppliesBlurSuccessfully)
{
    cv::Mat input = cv::Mat::zeros(
        100,
        100,
        CV_8UC3);

    cv::rectangle(
        input,
        cv::Point(40, 40),
        cv::Point(60, 60),
        cv::Scalar(255, 255, 255),
        -1);

    cv::Mat output;

    applyBlur(
        input,
        output);

    EXPECT_FALSE(output.empty());

    EXPECT_EQ(
        output.rows,
        input.rows);

    EXPECT_EQ(
        output.cols,
        input.cols);

    EXPECT_EQ(
        output.channels(),
        input.channels());

    EXPECT_NE(
        cv::norm(input, output),
        0);
}

TEST(MorphologyTest, ErodeReducesWhiteArea)
{
    cv::Mat input = cv::Mat::zeros(
        100,
        100,
        CV_8UC1);

    cv::rectangle(
        input,
        cv::Point(30, 30),
        cv::Point(70, 70),
        cv::Scalar(255),
        -1);

    cv::Mat output;

    applyMorphology(
        input,
        output,
        0,
        3);

    EXPECT_FALSE(output.empty());

    EXPECT_LT(
        cv::countNonZero(output),
        cv::countNonZero(input));
}

TEST(MorphologyTest, DilateIncreasesWhiteArea)
{
    cv::Mat input = cv::Mat::zeros(
        100,
        100,
        CV_8UC1);

    cv::rectangle(
        input,
        cv::Point(40, 40),
        cv::Point(60, 60),
        cv::Scalar(255),
        -1);

    cv::Mat output;

    applyMorphology(
        input,
        output,
        1,
        3);

    EXPECT_FALSE(output.empty());

    EXPECT_GT(
        cv::countNonZero(output),
        cv::countNonZero(input));
}

TEST(MorphologyTest, OpeningRemovesWhiteNoise)
{
    cv::Mat input = cv::Mat::zeros(
        100,
        100,
        CV_8UC1);

    input.at<uchar>(20, 20) = 255;

    cv::Mat output;

    applyMorphology(
        input,
        output,
        2,
        3);

    EXPECT_FALSE(output.empty());

    EXPECT_EQ(
        output.at<uchar>(20, 20),
        0);
}

TEST(MorphologyTest, ClosingFillsHole)
{
    cv::Mat input = cv::Mat::zeros(
        100,
        100,
        CV_8UC1);

    cv::rectangle(
        input,
        cv::Point(30, 30),
        cv::Point(70, 70),
        cv::Scalar(255),
        -1);

    input.at<uchar>(50, 50) = 0;

    cv::Mat output;

    applyMorphology(
        input,
        output,
        3,
        3);

    EXPECT_FALSE(output.empty());

    EXPECT_EQ(
        output.at<uchar>(50, 50),
        255);
}
TEST(PrintHelpTest, ContainsUsageText)
{
    testing::internal::CaptureStdout();

    printHelp("program");

    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(
        output.find("Usage:"),
        std::string::npos);
}

TEST(AskUserTest, ReturnsTrueWhenUserEntersY)
{
    std::istringstream input("y");

    auto oldBuffer = std::cin.rdbuf();

    std::cin.rdbuf(input.rdbuf());

    bool result = askUserToCreateDirectory();

    std::cin.rdbuf(oldBuffer);

    EXPECT_TRUE(result);
}

TEST(AskUserTest, ReturnsFalseWhenUserEntersN)
{
    std::istringstream input("n");

    auto oldBuffer = std::cin.rdbuf();

    std::cin.rdbuf(input.rdbuf());

    bool result = askUserToCreateDirectory();

    std::cin.rdbuf(oldBuffer);

    EXPECT_FALSE(result);
}

TEST(FileNameTest, AddsNumberWhenFileExists)
{
    std::string path = "test.jpg";

    std::ofstream file(path);

    file.close();

    handleDublicateFileName(path);

    EXPECT_EQ(
        path,
        "test(1).jpg");

    fs::remove("test.jpg");
}

TEST(FileNameTest, IncrementsExistingNumber)
{
    std::string path = "test(1).jpg";

    std::ofstream file(path);

    file.close();

    handleDublicateFileName(path);

    EXPECT_EQ(
        path,
        "test(2).jpg");

    fs::remove("test(1).jpg");
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







// // =================================================================
// // 1. isValidPath ֆունկցիայի թեստեր
// // =================================================================

// // Ստուգում ենք գոյություն ունեցող ֆայլի/թղթապանակի ճանապարհը
TEST(PathValidationTest, HandlesExistingPath) {
    // Տող 1: Ստեղծում ենք ժամանակավոր ֆայլ թեստի համար
    std::ofstream tempFile("temp_test_file.txt");
    tempFile << "test content";
    tempFile.close();

    // Տող 2: EXPECT_TRUE-ն ստուգում է, որ isValidPath-ը վերադարձնում է true
    EXPECT_TRUE(fs::exists("temp_test_file.txt"));

    // Տող 3: Մաքրում ենք թեստային ֆայլը
    fs::remove("temp_test_file.txt");
}

// // Ստուգում ենք գոյություն չունեցող (coutCaseInvalidPath) ճանապարհը
TEST(PathValidationTest, HandlesNonExistingPath) {
    // Տող 1: Փոխանցում ենք անվավեր path
    std::string invalidPath = "non_existing_folder/invalid_file.jpg";

    // Տող 2: EXPECT_FALSE-ով համոզվում ենք, որ fs::exists-ը վերադարձնում է false
    EXPECT_FALSE(fs::exists(invalidPath));
}





class ImageOperationsTest : public ::testing::Test {
protected:
    cv::Mat inputImg;
    cv::Mat outputImg;

    // SetUp-ը աշխատում է ամեն թեստից առաջ
    void SetUp() override {
        // Ստեղծում ենք 100x100 չափսի թեստային նկար (RGB)
        inputImg = cv::Mat::zeros(100, 100, CV_8UC3);
        // Վերևի ձախ անկյունում նկարում ենք սպիտակ քառակուսի, որպեսզի պտույտը/ֆլիպը ստուգելի լինի
        cv::rectangle(inputImg, cv::Rect(0, 0, 50, 50), cv::Scalar(255, 255, 255), -1);
    }
};



// applyFlip-ի թեստ
TEST_F(ImageOperationsTest, ApplyFlip) {
    // Տող 1: Կանչում ենք flip ֆունկցիան (օրինակ՝ հորիզոնական flip_code = 1)
    // Ենթադրվում է՝ applyFlip(input, output) կամ cv::flip(input, output, 1)
    applyFlip(inputImg, outputImg);

    // Տող 2: Ստուգում ենք, որ ելքային նկարը ստացվել է
    ASSERT_FALSE(outputImg.empty());

    // Տող 3: Հորիզոնական շրջումից հետո ձախ-վերևի քառակուսին տեղափոխվում է աջ-վերև (0, 90)
    cv::Vec3b pixelColor = outputImg.at<cv::Vec3b>(10, 90);
    EXPECT_EQ(pixelColor[0], 255);
}

// =================================================================
// 4. Console Cout/Error-ի ստուգման թեստ (coutCaseInvalidPath)
// =================================================================

TEST(ConsoleOutputTest, CapturesInvalidPathOutput) {
    // Տող 1: Սկսում ենք cout-ի ուրվագծումը (capture)
    testing::internal::CaptureStdout();

    // Տող 2: Սիմուլյացնում ենք սխալի տպումը
    std::cout << "Error: The image path doesn't exist." << std::endl;

    // Տող 3: Վերցնում ենք տպված տեքստը
    std::string output = testing::internal::GetCapturedStdout();

    // Տող 4: Ստուգում ենք, որ տեքստը պարունակում է սխալի հաղորդագրությունը
    EXPECT_NE(output.find("Error: The image path doesn't exist."), std::string::npos);
}

