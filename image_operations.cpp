#include "image_operations.h"
#include <filesystem>
namespace fs = std::filesystem;
namespace OpencvFunctions
{
    void applyGrey(const cv::Mat &input, cv::Mat &output)
    {
        cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
    }
    void applyRotate90(const cv::Mat &input, cv::Mat &output)
    {
        cv::rotate(input, output, cv::ROTATE_90_CLOCKWISE);
    }
    void applyBlur(const cv::Mat &input, cv::Mat &output)
    {
        cv::GaussianBlur(input, output, cv::Size(15, 15), 0, 0);
    }
    void applyRotate180(const cv::Mat &input, cv::Mat &output)
    {
        cv::rotate(input, output, cv::ROTATE_180);
    }
    void applyFlip(const cv::Mat &input, cv::Mat &output)
    {
        cv::flip(input, output, 1);
    }
    void applyResize(const cv::Mat &input, cv::Mat &output, double scaleFactor)
    {
        cv::resize(input, output, cv::Size(), scaleFactor, scaleFactor);
    }
    void applyCanny(const cv::Mat &input, cv::Mat &output, double threshold1, double threshold2)
    {
        cv::Canny(input, output, threshold1, threshold2);
    }
    void applyMorphology(const cv::Mat &input, cv::Mat &output, int operation, int kernelSize)
    {
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
        cv::morphologyEx(input, output, operation, element);
    }
}
namespace Utils
{

    void checkHelpFlag(int argc, char **argv)
    {
        if(argc < 4 || argc > 6){
            printHelp(argv[0]);
            exit(-1);
        }
        for (int i = 1; i < argc; ++i)
        {
            if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h")
            {
                printHelp(argv[0]);
                exit(0);
            }
        }
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
    void exitCaseInvalidPath(const std::string &changed_image_path)
    {
        std::string changed_image_directory = changed_image_path.substr(0, changed_image_path.rfind('/'));
        if (!isValidPath(changed_image_directory) && !handleMissingDirectory(changed_image_directory))
        {
            exit(-1); 
        }
    }
    void handleDublicateFileName(std::string &changed_image_path)
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
}