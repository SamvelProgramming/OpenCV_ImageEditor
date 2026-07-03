#include "image_operations.h"

void applyGrey(const cv::Mat &input, cv::Mat &output)
{
    cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
    cv::namedWindow("Gray Image", cv::WINDOW_NORMAL);
    cv::imshow("Gray Image", output);
}
void applyRotate90(const cv::Mat &input, cv::Mat &output)
{
    cv::rotate(input, output, cv::ROTATE_90_CLOCKWISE);
    cv::namedWindow("Rotated Image", cv::WINDOW_NORMAL);
    cv::imshow("Rotated Image", output);
}
void applyBlur(const cv::Mat &input, cv::Mat &output)
{
    cv::GaussianBlur(input, output, cv::Size(15, 15), 0, 0);
    cv::namedWindow("Blurred Image", cv::WINDOW_NORMAL);
    cv::imshow("Blurred Image", output);
}
void apply180(const cv::Mat& input ,cv::Mat& output){
    
    cv::rotate(input,output,cv::ROTATE_180);
    cv::namedWindow("180 Rotated Image", cv::WINDOW_NORMAL);
    cv::imshow("180 Rotated Image", output);
}
void applyFlip(const cv::Mat& input, cv::Mat& output){
    cv::flip(input,output,1);
    cv::namedWindow("Flipped Image", cv::WINDOW_NORMAL);
    cv::imshow("Flipped Image", output);
}
void applyResize(const cv::Mat& input, cv::Mat& output, double scaleFactor){
    cv::resize(input,output,cv::Size(),scaleFactor,scaleFactor);
    cv::imshow("Resized Image", output);
}
void applyCanny(const cv::Mat& input, cv::Mat& output, double threshold1, double threshold2){
    cv::Canny(input, output, threshold1, threshold2);
    cv::namedWindow("Canny Edge Image", cv::WINDOW_NORMAL);
    cv::imshow("Canny Edge Image", output);
}
void applyMorphology(const cv::Mat& input, cv::Mat& output, int operation, int kernelSize){
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(input, output, operation, element);
    cv::namedWindow("Morphology Image", cv::WINDOW_NORMAL);
    cv::imshow("Morphology Image", output);
}