#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

#include <opencv2/opencv.hpp>
namespace OpencvFunctions
{
    void applyGrey(const cv::Mat &input, cv::Mat &output);
    void applyRotate90(const cv::Mat &input, cv::Mat &output);
    void applyBlur(const cv::Mat &input, cv::Mat &output);
    void applyRotate180(const cv::Mat &input, cv::Mat &output);
    void applyFlip(const cv::Mat &input, cv::Mat &output);
    void applyResize(const cv::Mat &input, cv::Mat &output, double scaleFactor);
    void applyCanny(const cv::Mat &input, cv::Mat &output, double threshold1, double threshold2);
    void applyMorphology(const cv::Mat &input, cv::Mat &output, int operation, int kernelSize);
}
namespace Utils
{
    void checkHelpFlag(int argc, char **argv);
    void printHelp(const std::string &programName);
    void handleInvalidFileExtension(const std::set<std::string> &validFileExtensions);
    bool isValidFileExtension(const std::string &filePath, const std::set<std::string> &validExtensions);
    bool isValidPath(const std::string &path);
    void coutCaseInvalidPath(const std::string &path);
    void handleDublicateFileName(std::string &changed_image_path);
    void exitCaseImageEmpty(const cv::Mat &image);
    void exitCaseInvalidFileExtension(const std::string &imagePath);
    void coutCaseInvalidFileExtension(const std::string &imagePath, const std::set<std::string> &validFileExtensions);
    void exitCaseInvalidPath(const std::string &changed_image_path);
    bool askUserToCreateDirectory();
    bool handleMissingDirectory(std::string &changed_image_directory);

}

#endif