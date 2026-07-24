#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include <string>

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