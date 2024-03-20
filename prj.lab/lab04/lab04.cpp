#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <random>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/features2d.hpp"

///////////////////////////////////////////////////////////////////////////////////////
using namespace cv;
using std::cout;

const int count_slider_max = 255;
int count_slider = 5;
int count;

const int min_radius_slider_max = 255;
int min_radius_slider = 2;
int min_radius;

const int max_radius_slider_max = 255;
int max_radius_slider = 25;
int max_radius;

const int max_contrast_slider_max = 255;
int max_contrast_slider = 255;
int max_contrast;

const int min_contrast_slider_max = 255;
int min_contrast_slider = 0;
int min_contrast;

const int std_slider_max = 200;
int std_slider = 0;
double std_value = 0;

const int threshold_slider_max = 200;
int threshold_slider = 0;
double threshold_value = 0;

double beta;
Mat src1;
Mat src2;
Mat init_image;
Mat binary_image;

///////////////////////////////////////////////////////////////////////////////////////
cv::Mat addGaussianNoise(cv::Mat image, double std)
{

    cv::Mat gaussian_noise = cv::Mat::zeros(image.rows, image.cols, CV_32F);

    cv::randn(gaussian_noise, 0, std);

    cv::Mat noisy_image;
    image.convertTo(noisy_image, CV_32F);
    noisy_image += gaussian_noise;

    cv::Mat clipped_noisy_image;
    cv::threshold(noisy_image, clipped_noisy_image, 255, 255, cv::THRESH_TRUNC);
    cv::threshold(clipped_noisy_image, clipped_noisy_image, 0, 0, cv::THRESH_TOZERO);
    clipped_noisy_image.convertTo(clipped_noisy_image, CV_8UC1);

    return clipped_noisy_image;
}

/////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat generateImageWithCircles(int count, int min_radius, int max_radius, int min_contrast, int max_contrast, double std) {
    cv::Mat image(max_radius * 2 * (count + 1), max_radius * 2 * (count + 1), CV_8UC1, cv::Scalar(0, 0, 0));
    int grid_spacing = max_radius * 2;
    int step = (max_radius - min_radius) / count;
    cv::Scalar color(min_contrast, min_contrast, min_contrast);
    int step_color = (max_contrast - min_contrast) / count;

    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < count; ++j) {

            cv::Point center((j + 1) * grid_spacing, (i + 1) * grid_spacing);
            cv::Scalar color(min_contrast + step_color * j, min_contrast + step_color * j, min_contrast + step_color * j);
            cv::circle(image, center, min_radius + step * i, color, -1);
        }
    }

    Mat new_image = addGaussianNoise(image, std) + image;
    imwrite("result.png", new_image);
    return new_image;
}

////////////////////////////////////////////////////////////////////////////////

cv::Mat makeAdaptiveBinarization(Mat image, double max_value)
{
    Mat binary_image;
    image.convertTo(image, CV_8UC1);
    cv::adaptiveThreshold(image, binary_image, max_value, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 55, 0);
    return binary_image;

}
////////////////////////////////////////////////////////////////////////////////
static void on_count_trackbar(int, void*)
{
    count = std::max(count_slider,1);
    init_image = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value);
    imshow("Image", init_image);
}
////////////////////////////////////////////////////////////////////////////////
static void on_min_radius_trackbar(int, void*)
{
    min_radius = std::max(min_radius_slider, 1);
    init_image = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value);
    imshow("Image", init_image);
}
////////////////////////////////////////////////////////////////////////////////
static void on_max_radius_trackbar(int, void*)
{
    max_radius = std::max(max_radius_slider, 1);
    init_image = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value);
    imshow("Image", init_image);
}
////////////////////////////////////////////////////////////////////////////////
static void on_max_contrast_trackbar(int, void*)
{
    max_contrast = std::max(max_contrast_slider, 1);
    init_image = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value);
    imshow("Image", init_image);
}
////////////////////////////////////////////////////////////////////////////////
static void on_min_contrast_trackbar(int, void*)
{
    min_contrast = std::max(min_contrast_slider, 1);
    init_image = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value);
    imshow("Image", init_image);
}
////////////////////////////////////////////////////////////////////////////////
static void on_std_slider(int, void*)
{
    std_value = std::max(std_slider, 1);
    init_image = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value);
    imshow("Image", init_image);
}
////////////////////////////////////////////////////////////////////////////////
static void on_threshold_slider(int, void*)
{
    threshold_value = std::max(threshold_slider, 1);
    binary_image = makeAdaptiveBinarization(init_image, threshold_value);
    imshow("Binary Image", binary_image);
}

////////////////////////////////////////////////////////////////////////////////
int cv::connectedComponentsWithStats(InputArray img_, OutputArray _labels, OutputArray statsv,
    OutputArray centroids, int connectivity, int ltype)
{
    return cv::connectedComponentsWithStats(img_, _labels, statsv, centroids, connectivity, ltype, CCL_DEFAULT);
}

////////////////////////////////////////////////////////////////////////////////
int makeDetection(cv::Mat image)
{
    Mat labeledImage;
    int numLabels = connectedComponents(image, labeledImage);
    return numLabels;
}
////////////////////////////////////////////////////////////////////////////////
std::vector<KeyPoint> makeBlobDetection(Mat img)
{
    Mat image;
    img.convertTo(image, CV_8UC1);

    SimpleBlobDetector::Params params;

    // Change thresholds
    params.minThreshold = 10;
    params.maxThreshold = 200;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 1500;


    // Storage for blobs
    std::vector<KeyPoint> keypoints;


#if CV_MAJOR_VERSION < 3   // If you are using OpenCV 2

    // Set up detector with params
    SimpleBlobDetector detector(params);

    // Detect blobs
    detector.detect(im, keypoints);
#else 

    // Set up detector with params
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    // Detect blobs
    detector->detect(image, keypoints);
#endif 

    // Draw detected blobs as red circles.
    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
    // the size of the circle corresponds to the size of blob
    return keypoints;
    
}
////////////////////////////////////////////////////////////////////////////////
cv::Mat divideIntoConnectedComponents(cv::Mat img)
{
    cv::Mat image;
    img.convertTo(image, CV_8S);

    cv::Mat labelImage(image.size(), CV_8S);
    int nLabels = connectedComponents(labelImage, image, 8);

    std::vector<cv::Vec3b> colors;
    colors.push_back(cv::Vec3b(0, 0, 0));//background

    for (int label = 1; label < nLabels; ++label)
    {
        colors.push_back(cv::Vec3b((rand() & 255), (rand() & 255), (rand() & 255)));
    }

    cv::Mat dst(image.size(), CV_8S);
    for (int r = 0; r < dst.rows; ++r)
    {
        for (int c = 0; c < dst.cols; ++c)
        {
            int label = labelImage.at<int>(r, c);
            cv::Vec3b& pixel = dst.at<cv::Vec3b>(r, c);
            pixel = colors[label];
        }
    }
    return dst;
}
/////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat applyLoG(const cv::Mat& inputImage, int kernelSize, double sigma) {
    cv::Mat blurredImage;
    cv::GaussianBlur(inputImage, blurredImage, cv::Size(kernelSize, kernelSize), sigma, sigma);

    cv::Mat resultImage;
    cv::Laplacian(blurredImage, resultImage, CV_32F);

    cv::normalize(resultImage, resultImage, 0, 255, cv::NORM_MINMAX, CV_8U);

    return resultImage;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void applyThreshold(cv::Mat& src, cv::Mat& dst, double threshold, int blockSize) {

    cv::adaptiveThreshold(src, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C,
        cv::THRESH_BINARY, blockSize, threshold);
}
/////////////////////////////////////////////////////////////////////////////////////////////
struct Metrics {
    int TP;
    int TN;
    int FP;
    int FN;
};
/////////////////////////////////////////////////////////////////////////////////////////////
Metrics computeMetrics(const std::vector<KeyPoint>& keypoints, const Mat& groundTruth) {
    groundTruth.convertTo(groundTruth,CV_8UC1);
    Metrics metrics = { 0, 0, 0, 0 };

    for (const KeyPoint& kp : keypoints) {
        Point pt = kp.pt;

        uchar gtPixel = groundTruth.at<uchar>(pt);

        if (gtPixel == 255) {
            metrics.TP++;
        }
        else {
            metrics.FP++;
        }
    }

    int totalPixels = groundTruth.rows * groundTruth.cols;

    // Вычисление FN и TN
    metrics.FN = totalPixels - metrics.TP;
    metrics.TN = 0;

    return metrics;
}

/////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    src1 = src2 =  generateImageWithCircles(5, 5, 25,0,255, std_value);
    count_slider = 5;
    min_radius = 2;
    max_radius = 25;
    namedWindow("Image", WINDOW_AUTOSIZE);
    namedWindow("Binary Image", WINDOW_AUTOSIZE);
    namedWindow("TrackBars", WINDOW_AUTOSIZE);

    char TrackbarName[50];
    sprintf(TrackbarName, "Count");
    createTrackbar(TrackbarName, "TrackBars", &count_slider, count_slider_max, on_count_trackbar);
    on_count_trackbar(count_slider, 0);

    sprintf(TrackbarName, "Min Radius");
    createTrackbar(TrackbarName, "TrackBars", &min_radius_slider, min_radius_slider_max, on_min_radius_trackbar);
    on_min_radius_trackbar(min_radius_slider, 0);

    sprintf(TrackbarName, "Max Radius");
    createTrackbar(TrackbarName, "TrackBars", &max_radius_slider, max_radius_slider_max, on_max_radius_trackbar);
    on_max_radius_trackbar(max_radius_slider, 0);

    sprintf(TrackbarName, "Max Contrast");
    createTrackbar(TrackbarName, "TrackBars", &max_contrast_slider, max_contrast_slider_max, on_max_contrast_trackbar);
    on_max_contrast_trackbar(max_contrast_slider, 0);

    sprintf(TrackbarName, "Min Contrast");
    createTrackbar(TrackbarName, "TrackBars", &min_contrast_slider, min_contrast_slider_max, on_min_contrast_trackbar);
    on_min_contrast_trackbar(min_contrast_slider, 0);

    sprintf(TrackbarName, "Std value");
    createTrackbar(TrackbarName, "TrackBars", &std_slider, std_slider_max, on_std_slider);
    on_min_contrast_trackbar(min_contrast_slider, 0);

    sprintf(TrackbarName, "Threshold");
    createTrackbar(TrackbarName, "TrackBars", &threshold_slider, threshold_slider_max, on_threshold_slider); 
    on_threshold_slider(threshold_slider, 0);

    resizeWindow("Image", src1.cols, src1.rows);


   
   Mat im_with_keypoints;
   auto keypoints = makeBlobDetection(src1);
   Mat keypointsImage;
   //auto metrics = computeMetrics(keypoints, src1);

   drawKeypoints(src1, keypoints, keypointsImage, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
   auto connected_components_img = divideIntoConnectedComponents(src1);

    // Show blobs
   imshow("connected components", connected_components_img);
    waitKey(0);
    return 0;
}