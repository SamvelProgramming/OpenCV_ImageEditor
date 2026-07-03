#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include "image_operations.h"
#include <map>
#include <functional>

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
    }
    else
    {
        std::cout << "Error: Unknown action." << std::endl;
        return;
    }
}

int main(int argc, char **argv)
{

    if (argc < 4)
    {
        std::cout << "usage:" << argv[0] << " <image path> <changed_image path> <action>" << std::endl;
        std::cout << "actions: grey , rotate90 ,blur , rotate180, flip, resize, canny, morphology" << std::endl;
        return -1;
    }

    std::string imagePath = argv[1];
    std::string action = argv[3];
    std::string changed_image_path = argv[2];

    if (argv[2] == "--help" || argv[2] == "-h")
    {
        std::cout << "usage:" << argv[0] << " <image path> <changed_image path> <action>" << std::endl;
        std::cout << "actions: grey , rotate90 ,blur , rotate180, flip, resize, canny, morphology" << std::endl;
        return 0;
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

    cv::imwrite(changed_image_path, changed_image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}