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