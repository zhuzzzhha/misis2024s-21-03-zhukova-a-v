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
void DoConnectedCompsDetection(Mat& image_src)
{
    Mat comps_detection_image = DoDetection(image_src, threshold_value);
    cv::imshow("Connected components detection", comps_detection_image);
    cv::imwrite("connected_comps.png", comps_detection_image);
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
    DoConnectedCompsDetection(img_stat.image);
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
cv::Mat normalizeGradientImage(const cv::Mat src) {
    double minVal, maxVal;
    cv::Mat new_src = src;
    cv::minMaxLoc(src, &minVal, &maxVal);
    double scale = 255.0 / (maxVal - minVal);
    double shift = -minVal * scale;
    cv::Mat dst;
    src.convertTo(dst, CV_8U, scale, shift);
    //dst.convertTo(dst, CV_32F);
    return dst;
}
//--------------------------------------------------------------------------------
void applyThreshold_symple(cv::Mat& src, cv::Mat& dst, int threshold) {
    cv::threshold(src, dst, threshold, 255, cv::THRESH_BINARY);
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
cv::Mat createMaskForBlackPixels(const cv::Mat& inputImage, int kernel_size) {
    
    cv::Mat new_img;
    inputImage.convertTo(new_img,CV_8U);
    cv::Mat mask(new_img.size(), CV_8U, cv::Scalar(0));

    for (int y = 0; y < new_img.rows; ++y) {
        for (int x = 0; x < new_img.cols; ++x) {
            int brightness = new_img.at<uchar>(y, x);

            if (brightness >= 0 && brightness<=5) {
                cv::circle(mask, cv::Point(x, y), kernel_size, cv::Scalar(255, 255, 255),1);
            }
        }
    }
    imwrite("mask.png",mask);
    return mask;
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void applyThreshold(cv::Mat& src, cv::Mat& dst, double threshold, int blockSize) {

    cv::adaptiveThreshold(src, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C,
        cv::THRESH_BINARY, blockSize, threshold);
}
//--------------------------------------------------------------------------------
struct Metrics {
    int TP = 0;
    int TN = 0;
    int FP = 0;
    int FN = 0;
};
//--------------------------------------------------------------------------------
//double calculateIoU(const cv::Mat& mask, const Circle& groundTruth) {
//    cv::Mat binaryMask;
//    cv::threshold(mask, binaryMask, 0, 1, cv::THRESH_BINARY);
//
//    // Создаем круговую маску для ground truth
//    cv::Mat circleMask = cv::Mat::zeros(mask.size(), CV_8U);
//    cv::circle(circleMask, cv::Point(groundTruth.x, groundTruth.y), groundTruth.r, cv::Scalar(255), 1);
//
//    double intersection = cv::countNonZero(binaryMask & circleMask);
//    double unionAreaValue = cv::countNonZero(binaryMask) + cv::countNonZero(circleMask) - intersection;
//
//    double iou = intersection / unionAreaValue;
//    return iou;
//}
//--------------------------------------------------------------------------------
Metrics calculateMetrics(std::vector<double> iou) {
    double threshold_low = 20;
    Metrics current_metric;
    for (double metric : iou)
    {
        if (threshold_low <= metric)
            current_metric.TP++;
        else if (metric >= 0)
            current_metric.FN++;
        else
            current_metric.FP++;
    }
    return current_metric;
}
//--------------------------------------------------------------------------------
//void evaluateQuality()
//{
//   ImageWithStats eval_image = generateImageWithCircles(6, 3, 20, min_contrast, max_contrast, 3, 2, false);
//   imwrite("initial_img.png", eval_image.image);
//
//   detected_circles = LoG(eval_image.image);
//   imwrite("log_img.png", detected_circles);
//
//   Mat  cur_mask = createMaskForBlackPixels(detected_circles, 7);
//
//   std::vector<double> iou;
//   for (int i = 0; i < eval_image.stats.value().circles.size(); i++)
//   {
//       double cur_iou = calculateIoU(cur_mask, eval_image.stats.value().circles[i]);
//       iou.push_back(cur_iou);
//   }
//   Metrics final_metric = calculateMetrics(iou);
//   cout << "TP: " << final_metric.FP << std::endl << "FP:" << final_metric.FP<< std::endl << "FN: " << final_metric.FN;
//}
//--------------------------------------------------------------------------------
int main(void)
{
    CreateTrackbars();

   


   
      

        waitKey(0);
    return 0;
}