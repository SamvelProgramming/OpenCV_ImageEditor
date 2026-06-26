#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include "image_operations.h"



void actions(const std::string& action, const cv::Mat& image, cv::Mat& changed_image, int argc, char **argv)
{
    if (action == "grey")
    {
        applyGrey(image, changed_image);
    }
    else if (action == "rotate90")
    {
        applyRotate90(image, changed_image);
    }
    else if (action == "blur")
    {
        applyBlur(image, changed_image);
    }
    else if (action == "rotate180")
    {
        apply180(image, changed_image);
    }
    else if (action == "flip")
    {
        applyFlip(image, changed_image);
    }
    else if (action == "resize")
    {
        if (argc < 4)
        {
            std::cout << "usage: " << argv[0] << " <image path> resize <scale factor>" << std::endl;
            return;
        }
        double scaleFactor = std::stod(argv[3]);
        applyResize(image, changed_image, scaleFactor);
    }
    else if (action == "canny")
    {
        if (argc < 5)
        {
            std::cout << "usage: " << argv[0] << " <image path> canny <threshold1> <threshold2>" << std::endl;
            return;
        }
        try
        {
            double threshold1 = std::stod(argv[3]);
            double threshold2 = std::stod(argv[4]);
            applyCanny(image, changed_image, threshold1, threshold2);
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Error: Invalid threshold values. Please provide numeric values." << std::endl;
            return;
        }
    }
}



int main(int argc, char **argv)
{


    if (argc < 3)
    {
        std::cout << "usage:" << argv[0] << " <image path> <action>" << std::endl;
        std::cout << "actions: grey , rotate90 ,blur , rotate180, flip, resize, canny" << std::endl;
        return -1;
    }

    std::string imagePath = argv[1];
    std::string action = argv[2];

    if (argv[2] == "--help" || argv[2] == "-h"){        
        std::cout << "usage:" << argv[0] << " <image path> <action>" << std::endl;
        std::cout << "actions: grey , rotate90 ,blur , rotate180, flip, resize, canny" << std::endl;
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
    actions(action, image, changed_image, argc, argv);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}