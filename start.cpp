#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include "image_operations.h"
#include <map>
#include <filesystem>
#include <functional>

namespace fs = std::filesystem;
using ActionFunction = std::function<void(const cv::Mat &, cv::Mat &, int, char **)>;

void Actions(const cv::Mat &input, cv::Mat &output, int argc, char **argv)
{
    std::map<std::string, std::function<void(const cv::Mat &, cv::Mat &, int, char **)>> actionMap = {
        {"grey", [](const cv::Mat &input, cv::Mat &output, int, char **)
         { applyGrey(input, output); }},
        {"rotate90", [](const cv::Mat &input, cv::Mat &output, int, char **)
         { applyRotate90(input, output); }},
        {"blur", [](const cv::Mat &input, cv::Mat &output, int, char **)
         { applyBlur(input, output); }},
        {"rotate180", [](const cv::Mat &input, cv::Mat &output, int, char **)
         { apply180(input, output); }},
        {"flip", [](const cv::Mat &input, cv::Mat &output, int, char **)
         { applyFlip(input, output); }},
        {"resize", [](const cv::Mat &input, cv::Mat &output, int argc, char **argv)
         {
             if (argc < 4)
             {
                 std::cout << "usage: " << argv[0] << " <image path> resize <scale factor>" << std::endl;
                 return;
             }
             double scaleFactor = std::stod(argv[4]);
             applyResize(input, output, scaleFactor);
         }},
        {"canny", [](const cv::Mat &input, cv::Mat &output, int argc, char **argv)
         {
             if (argc < 5)
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
                     return;
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
    if (it != actionMap.end())
    {
        it->second(input, output, argc, argv);
        cv::namedWindow(it->first + " Image", cv::WINDOW_NORMAL);
        cv::imshow(it->first + " Image", output);
    }
    else
    {
        std::cout << "Error: Unknown action." << std::endl;
        return;
    }
}

bool isValidPath(const std::string &path)
{
    return fs::exists(path);
}
bool providePath(std::string &changed_image_directory)
{
    std::cout << "Error: The path doesn't exist." << std::endl;
    std::cout << "Do you want to create the directory? (y/n): ";
    char response;
    std::cin >> response;
    if (response != 'y' && response != 'Y')
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
    std::cout << "usage: " << programName << " <image path> <changed_image path> <action> <parameters>(if they exist)" << std::endl;
    std::cout << "actions: grey, rotate90, blur, rotate180, flip, resize, canny, morphology" << std::endl;
}
int main(int argc, char **argv)
{

    if (argc < 4 || argc > 6 || argv[1] == "--help" || argv[1] == "-h")
    {
        printHelp(argv[0]);
        return -1;
    }

    std::string imagePath = argv[1];

    if (!isValidPath(imagePath))
    {
        std::cout << "Error: The image path doesn't exist." << std::endl;
        return -1;
    };

    std::string action = argv[3];
    std::string changed_image_path = argv[2];
    std::string changed_image_directory = changed_image_path.substr(0, changed_image_path.rfind('/'));
    if (!isValidPath(changed_image_directory))
    {
        if (!providePath(changed_image_directory))
        {
            return -1;
        }
    }

    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    if (image.empty())
    {
        std::cout << "Error: Could not read image file." << std::endl;
        return -1;
    }

    cv::Mat changed_image;
    cv::namedWindow("Original Image", cv::WINDOW_NORMAL);
    cv::imshow("Original Image", image);

    Actions(image, changed_image, argc, argv);

    while (isValidPath(changed_image_path))
    {
        int pos = changed_image_path.find('.');
        if (changed_image_path[pos - 1] == ')')
        {
            changed_image_path[pos - 2] += 1;
        }
        else
        {
            changed_image_path.insert(pos, "(1)");
        }
    }
    cv::imwrite(changed_image_path, changed_image);
    cv::waitKey(1000);
    cv::destroyAllWindows();
    return 0;
}