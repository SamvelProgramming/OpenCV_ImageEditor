#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include "image_operations.h"
#include <map>
#include <set>
#include <filesystem>
#include <functional>
using namespace OpencvFunctions;

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

bool isValidPath(const std::string &path)
{
    return fs::exists(path);
}
void coutCaseInvalidPath(const std::string &path)
{
    if (!isValidPath(path))
    {
        std::cout << "Error: The path doesn't exist." << std::endl;
        exit(-1);
    }
}
bool askUserToCreateDirectory()
{
    std::cout << "Error: The path doesn't exist." << std::endl;
    std::cout << "Do you want to create the directory? (y/n): ";
    char response;
    std::cin >> response;
    return (response == 'y' || response == 'Y');
}
bool handleMissingDirectory(std::string &changed_image_directory)
{
    if (!askUserToCreateDirectory())
    {
        std::cout << "Please provide a valid path for the changed image." << std::endl;
        return false;
    }
    fs::create_directories(changed_image_directory);
    std::cout << "Directory created: " << changed_image_directory << std::endl;
    return true;
}

void printHelp(const std::string &programName)
{
    std::cout << "Usage: " << programName << " <image_path> <changed_image_path> <action> <parameters>\n"
              << "Available Actions & Descriptions:\n\n"
              << "  grey        - Converts the image to grayscale (black and white).\n"
              << "                No extra parameters needed.\n\n"
              << "  rotate90    - Rotates the image 90 degrees clockwise.\n"
              << "                No extra parameters needed.\n\n"
              << "  rotate180   - Rotates the image 180 degrees.\n"
              << "                No extra parameters needed.\n\n"
              << "  blur        - Blurs the image using a Gaussian filter.\n"
              << "                Parameter: <kernel_size> (Must be an odd number, e.g., 5).\n\n"
              << "  flip        - Flips the image.\n"
              << "                Parameter: <flip_code> (0: vertical, 1: horizontal, -1: both).\n\n"
              << "  resize      - Resizes the image to new dimensions.\n"
              << "                Parameters: <width> <height> (e.g., 800 600).\n\n"
              << "  canny       - Detects edges using the Canny edge detector.\n"
              << "                Parameters: <low_threshold> <high_threshold> (e.g., 50 150).\n\n"
              << "  morphology  - Applies morphological operations (Erosion/Dilation).\n"
              << "                Parameters: <op_type> <kernel_size>\n"
              << "                (op_type: 0 for Erode, 1 for Dilate; kernel_size: odd number).\n";
}

bool isValidFileExtension(const std::string &filePath, const std::set<std::string> &validExtensions)
{
    std::string extension = fs::path(filePath).extension().string();
    return validExtensions.find(extension) != validExtensions.end();
}

void handleInvalidFileExtension(const std::set<std::string> &validExtensions)
{
    std::cout << "Error: Invalid file extension. Supported extensions are: ";
    for (const auto &ext : validExtensions)
    {
        std::cout << ext << " ";
    }
    std::cout << std::endl;
}
void checkHelpFlag(int argc, char **argv)
{
    if (argc < 4 || argc > 6 || std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")
    {
        printHelp(argv[0]);
        exit(-1);
    }
}
void exitCaseInvalidPath(const std::string &changed_image_path)
{
    std::string changed_image_directory = changed_image_path.substr(0, changed_image_path.rfind('/'));
    if (!isValidPath(changed_image_directory) && !handleMissingDirectory(changed_image_directory))
    {
        exit(-1);
    }
}
void forDublicateFileName(std::string &changed_image_path)
{
    while (isValidPath(changed_image_path))
    {
        int pos = changed_image_path.find('.');
        if (changed_image_path[pos - 1] == ')')
        {
            if (changed_image_path[pos - 2] == '9')
            {
                std::cout << "Error: Too many files with the same name. Please choose a different name." << std::endl;
                exit(-1);
            }
            changed_image_path[pos - 2] += 1;
        }
        else
        {
            changed_image_path.insert(pos, "(1)");
        }
    }
}
void exitCaseImageEmpty(const cv::Mat &image)
{
    if (image.empty())
    {
        std::cout << "Error: Could not read image file." << std::endl;
        exit(-1);
    }
}
void exitCaseInvalidFileExtension(const std::string &imagePath)
{
    std::set<std::string> validFileExtensions = {".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".tif"};
    if (!isValidFileExtension(imagePath, validFileExtensions))
    {
        handleInvalidFileExtension(validFileExtensions);
        exit(-1);
    }
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

    forDublicateFileName(changed_image_path);

    cv::imwrite(changed_image_path, changed_image);
    cv::waitKey(1000);
    cv::destroyAllWindows();
    return 0;
}