#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

#include <opencv2/opencv.hpp>

void applyGrey(const cv::Mat& input, cv::Mat& output);
void applyRotate90(const cv::Mat& input, cv::Mat& output);
void applyBlur(const cv::Mat& input, cv::Mat& output);
void apply180(const cv::Mat& input ,cv::Mat& output);
void applyFlip(const cv::Mat& input, cv::Mat& output);
void applyResize(const cv::Mat& input, cv::Mat& output, double scaleFactor);
void applyCanny(const cv::Mat& input, cv::Mat& output, double threshold1, double threshold2);
void applyMorphology(const cv::Mat& input, cv::Mat& output, int operation, int kernelSize);

#endif