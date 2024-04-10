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
#include "opencv2/features2d.hpp"
#include "image_generator.h"
#include "log_detector.h"
#include "cnctd_comps_detector.h"
#include "evaluator.h"


using namespace cv;
//--------------------------------------------------------------------------------
static ImageWithStats img_stat;
//--------------------------------------------------------------------------------
const int count_slider_max = 255;
int count_slider = 5;
int count;
//--------------------------------------------------------------------------------
const int min_radius_slider_max = 255;
int min_radius_slider = 2;
int min_radius;
//--------------------------------------------------------------------------------
const int max_radius_slider_max = 255;
int max_radius_slider = 25;
int max_radius;
//--------------------------------------------------------------------------------
const int max_contrast_slider_max = 255;
int max_contrast_slider = 255;
int max_contrast;
//--------------------------------------------------------------------------------
const int min_contrast_slider_max = 255;
int min_contrast_slider = 0;
int min_contrast;
//--------------------------------------------------------------------------------
const int std_slider_max = 200;
int std_slider = 0;
double std_value;
//--------------------------------------------------------------------------------
const int threshold_slider_max = 200;
int threshold_slider = 0;
double threshold_value;
//--------------------------------------------------------------------------------
const int blur_slider_max = 20;
int blur_slider = 0;
int blur_value;
//--------------------------------------------------------------------------------
int scaler = 20;
//--------------------------------------------------------------------------------
double beta;
Mat src1;
Mat src2;
Mat init_image;
Mat binary_image;
Mat detected_blobs;
Mat detected_circles;
Mat mask;
//--------------------------------------------------------------------------------
void CreateInitImageWindow()
{
    cv::imshow("Initial image", img_stat.image);
    cv::imwrite("generated.png", img_stat.image);
}
//--------------------------------------------------------------------------------
void DoLoG(Mat& image_src)
{
    Mat log_image = LoG(image_src);
    cv::imshow("LoG image", log_image);
    cv::imwrite("gau.png", log_image);
}
//--------------------------------------------------------------------------------
std::vector<Circle> DoConnectedCompsDetection(Mat& image_src, Mat& image_dst, double max_value,int threshold_area)
{
    std::vector<Circle> detection_vec = DoDetection(image_src, image_dst, max_value, threshold_area);
    cv::imshow("Connected components detection", image_dst);
    cv::imwrite("connected_comps.png", image_dst);
    return detection_vec;
}
//--------------------------------------------------------------------------------
void on_trackbar(int, void*)
{
    count = std::max(5, count_slider);
    min_radius = std::max(2, min_radius_slider);
    max_radius = std::max(12, max_radius_slider);
    min_contrast = std::max(1, min_contrast_slider);
    max_contrast = std::max(1, max_contrast_slider);
    threshold_value = std::max(1, threshold_slider);
    blur_value = std::max(5, blur_slider);
    std_value = std::max(1, std_slider);
    generateImageWithCircles(img_stat, count, min_radius, max_radius, min_contrast, max_contrast, std_value, blur_value, false);
    CreateInitImageWindow();
    DoLoG(img_stat.image);

    Mat dst_image;
    std::vector<Circle> detected_circles = DoConnectedCompsDetection(img_stat.image, dst_image, threshold_value,10);

    int TP = 0, FP = 0, FN = 0;
    evaluateDetections(img_stat.stats.value().circles, detected_circles, TP, FP, FN);
    std::cout << "TP: " << TP << std::endl << "FP: " << FP << std::endl << "FN: " << FN << std::endl;
}

//--------------------------------------------------------------------------------
void CreateTrackbars()
{
    namedWindow("TrackBars", WINDOW_AUTOSIZE);
    char TrackbarName[50];
    sprintf(TrackbarName, "Count");
    createTrackbar(TrackbarName, "TrackBars", &count_slider, count_slider_max, on_trackbar);
    on_trackbar(count_slider, 0);

    sprintf(TrackbarName, "Min Radius");
    createTrackbar(TrackbarName, "TrackBars", &min_radius_slider, min_radius_slider_max, on_trackbar);
    on_trackbar(min_radius_slider, 0);

    sprintf(TrackbarName, "Max Radius");
    createTrackbar(TrackbarName, "TrackBars", &max_radius_slider, max_radius_slider_max, on_trackbar);
    on_trackbar(max_radius_slider, 0);

    sprintf(TrackbarName, "Max Contrast");
    createTrackbar(TrackbarName, "TrackBars", &max_contrast_slider, max_contrast_slider_max, on_trackbar);
    on_trackbar(max_contrast_slider, 0);

    sprintf(TrackbarName, "Min Contrast");
    createTrackbar(TrackbarName, "TrackBars", &min_contrast_slider, min_contrast_slider_max, on_trackbar);
    on_trackbar(min_contrast_slider, 0);

    sprintf(TrackbarName, "Std value");
    createTrackbar(TrackbarName, "TrackBars", &std_slider, std_slider_max, on_trackbar);
    on_trackbar(std_slider, 0);

    sprintf(TrackbarName, "Threshold");
    createTrackbar(TrackbarName, "TrackBars", &threshold_slider, threshold_slider_max, on_trackbar);
    on_trackbar(threshold_slider, 0);

    sprintf(TrackbarName, "Blur");
    createTrackbar(TrackbarName, "TrackBars", &blur_slider, blur_slider_max, on_trackbar);
    on_trackbar(threshold_slider, 0);

}

//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//cv::Mat LoG(const cv::Mat& inputImage, int blockSize, int houghParameters_param, int houghParameters_sz, int houghParameters_p0, int houghParameters_p1, int houghParameters_p2, bool ZEROS, int thresholdValue) {
//    cv::Mat resizedImage;
//    cv::resize(inputImage, resizedImage, cv::Size(inputImage.cols / scaler, inputImage.rows / scaler));
//    cv::Mat blurred, laplacianImage;
//    cv::GaussianBlur(resizedImage, blurred, cv::Size(blockSize, blockSize), 0);
//    cv::Laplacian(blurred, laplacianImage, CV_32F, blockSize);
//    cv::Mat absLaplacianImage = laplacianImage;
//    cv::Mat morph_image;
//    int morph_size = 10;
//    int accumulatorThreshold = 10;
//    int morph_elem = 2;
//    int operation = cv::MORPH_ERODE;
//    cv::Mat element = cv::getStructuringElement(morph_elem, cv::Size(2 * morph_size + 1, 2 * morph_size + 1), cv::Point(morph_size, morph_size));
//    cv::morphologyEx(laplacianImage, morph_image, operation, element);
//    cv::imwrite("xy.png", normalizeGradientImage(laplacianImage));
//    cv::imwrite("morph_image.png", normalizeGradientImage(morph_image));
//    cv::imwrite("morph_image_sub.png", normalizeGradientImage(laplacianImage - morph_image));
//    cv::imwrite("morph_image_sub_final_1.png", 4 >= normalizeGradientImage(laplacianImage - morph_image));
//    applyThreshold_symple(absLaplacianImage, absLaplacianImage, thresholdValue);
//    return inputImage;
//}

//--------------------------------------------------------------------------------
struct Metrics {
    int TP = 0;
    int TN = 0;
    int FP = 0;
    int FN = 0;
};

//--------------------------------------------------------------------------------
int main(void)
{
    CreateTrackbars();

   


   
      

        waitKey(0);
    return 0;
}