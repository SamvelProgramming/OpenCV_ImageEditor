#include "image_operations.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <map>
#include <set>
#include <filesystem>
#include <functional>
using namespace OpencvFunctions;
using namespace Utils;

namespace fs = std::filesystem;

using ActionFunction = std::function<void(const cv::Mat &, cv::Mat &, int, char **)>;

void processImageAction(const cv::Mat &input, cv::Mat &output, int argc, char **argv)
{
    std::map<std::string, ActionFunction> actionMap = {
        {"grey", [](const cv::Mat &input, cv::Mat &output, int, char **)
         { applyGrey(input, output); }},
        {"rotate90", [](const cv::Mat &input, cv::Mat &output, int, char **)
         { applyRotate90(input, output); }},
        {"blur", [](const cv::Mat &input, cv::Mat &output, int, char **)
         { applyBlur(input, output); }},
        {"rotate180", [](const cv::Mat &input, cv::Mat &output, int, char **)
         { applyRotate180(input, output); }},
        {"flip", [](const cv::Mat &input, cv::Mat &output, int, char **)
         { applyFlip(input, output); }},
        {"resize", [](const cv::Mat &input, cv::Mat &output, int argc, char **argv)
         {
             if (argc < 5)
             {
                 std::cout << "usage: " << argv[0] << " <image path> resize <scale factor>" << std::endl;
                 return;
             }
             double scaleFactor = std::stod(argv[4]);
             try
             {
                applyResize(input, output, scaleFactor);
             }
             catch (const std::invalid_argument &e)
             {
                 std::cout << "Error: Invalid scale factor. Please provide a numeric value." << std::endl;
                 return;
             }
         }},
        {"canny", [](const cv::Mat &input, cv::Mat &output, int argc, char **argv)
         {
             if (argc < 6)
             {
                 std::cout << "usage: " << argv[0] << " <image path> canny <threshold1> <threshold2>" << std::endl;
                 return;
             }
             try
             {
                 double threshold1 = std::stod(argv[4]);
                 double threshold2 = std::stod(argv[5]);
                 applyCanny(input, output, threshold1, threshold2);
             }
             catch (const std::invalid_argument &e)
             {
                 std::cout << "Error: Invalid threshold values. Please provide numeric values." << std::endl;
                 return;
             }
         }},
        {"morphology", [](const cv::Mat &input, cv::Mat &output, int argc, char **argv)
         {
             if (argc < 6)
             {
                 std::cout << "usage: " << argv[0] << " <image path> morphology <operation> <kernel size>" << std::endl;
                 return;
             }
             try
             {
                 int operation = std::stoi(argv[4]);
                 int kernelSize = std::stoi(argv[5]);
                 if (kernelSize <= 0 || kernelSize % 2 == 0 || operation < 0 || operation > 3)
                 {
                     std::cout << "Error: Kernel size must be a positive odd integer." << std::endl;
                     std::cout << "Error: Operation must be 0 (MORPH_ERODE), 1 (MORPH_DILATE), 2 (MORPH_OPEN), or 3 (MORPH_CLOSE)." << std::endl;
                     exit(-1);
                 }
                 applyMorphology(input, output, operation, kernelSize);
             }
             catch (const std::invalid_argument &e)
             {
                 std::cout << "Error: Invalid operation or kernel size values. Please provide numeric values." << std::endl;
                 return;
             }
         }}};
    auto it = actionMap.find(argv[3]);

    if (it == actionMap.end())
    {
        std::cout << "Error: Unknown action." << std::endl;
        return;
    }

    it->second(input, output, argc, argv);
    if (it->first == "resize")
    {
        cv::imshow(it->first + " Image", output);
        return;
    }
    cv::namedWindow(it->first + " Image", cv::WINDOW_NORMAL);
    cv::imshow(it->first + " Image", output);
}


int main(int argc, char **argv)
{
    checkHelpFlag(argc, argv);

    std::string imagePath = argv[1];
    exitCaseInvalidFileExtension(imagePath);
    coutCaseInvalidPath(imagePath);

    std::string action = argv[3];
    std::string changed_image_path = argv[2];
    exitCaseInvalidPath(changed_image_path);


    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    exitCaseImageEmpty(image);

    cv::Mat changed_image;
    cv::namedWindow("Original Image", cv::WINDOW_NORMAL);
    cv::imshow("Original Image", image);

    processImageAction(image, changed_image, argc, argv);

    handleDublicateFileName(changed_image_path);

    cv::imwrite(changed_image_path, changed_image);
    cv::waitKey(1000);
    cv::destroyAllWindows();
    return 0;
}