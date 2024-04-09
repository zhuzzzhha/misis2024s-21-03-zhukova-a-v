#pragma once
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <fstream>
#include <optional>
#include <vector>
#include <random>
#include <functional>
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>
#include "opencv2/features2d.hpp"
#include "image_generator.h"

using namespace cv;
static double M_PI = 3.14;
//--------------------------------------------------------------------------------
cv::Mat makeAdaptiveBinarization(Mat& image, double max_value);
//--------------------------------------------------------------------------------
std::vector<Circle> detectConnectedComponents(const cv::Mat& inputImage, Mat& visualizationImage, int areaThreshold);
//--------------------------------------------------------------------------------
std::vector<Circle> DoDetection(Mat& src_image, Mat& dst_image, double max_value, int treshold_area);
//--------------------------------------------------------------------------------