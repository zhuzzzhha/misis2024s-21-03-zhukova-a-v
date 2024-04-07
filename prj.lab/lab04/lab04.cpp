#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <fstream>
#include <optional>
#include <vector>
#include <random>

#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/features2d.hpp"


#define GENERATE_FILENAME(name, kernel_size) (name + std::to_string(kernel_size) + ".png")
#define GENERATE_JSONNAME(name, kernel_size) (name + std::to_string(kernel_size) + ".json")
///////////////////////////////////////////////////////////////////////////////////////

struct Circle
{
    double x;
    double y;
    double r;
    int brightness;
};
///////////////////////////////////////////////////////////////////////////////////////
static int num_file = 1;
///////////////////////////////////////////////////////////////////////////////////////
struct Statistics
{
    std::vector<Circle> circles;
    int image_width;
    int image_height;
    int background_color;
    int blur;
    double noise;
};

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

const int blur_slider_max = 20;
int blur_slider = 0;
int blur_value = 2;

double beta;
Mat src1;
Mat src2;
Mat init_image;
Mat binary_image;
Mat detected_blobs;
Mat detected_circles;
Mat mask;

///////////////////////////////////////////////////////////////////////////////////////
struct ImageWithStats {
    cv::Mat image;
    std::optional<Statistics> stats;
};
///////////////////////////////////////////////////////////////////////////////////////
void writeInJSON(const std::string& filename, Statistics& stats)
{
    std::ostringstream jsonStream;
    jsonStream << "{";
    jsonStream << "\"background\":{";
    jsonStream << "\"size\":[" << stats.image_width << "," << stats.image_height << "],";
    jsonStream << "\"color\":"<< stats.background_color<<",";
    jsonStream << "\"noise\":" << stats.noise << ",";
    jsonStream << "\"blur\":" << stats.blur;
    jsonStream << "},";
    jsonStream << "\"objects\":[";

    for (size_t i = 0; i < stats.circles.size(); ++i) {
        jsonStream << "{";
        jsonStream << "\"c\":" << stats.circles[i].brightness << ",";
        jsonStream << "\"p\":[" << (stats.circles[i].x) << "," << (stats.circles[i].y) << "," << (stats.circles[i].r);
        jsonStream << "]}";
        if (i != stats.circles.size() - 1)
            jsonStream << ",";
    }
    jsonStream << "]";
    jsonStream << "}";

    std::ofstream output_file(filename);

    output_file << jsonStream.str();
    output_file.close();
}
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
ImageWithStats generateImageWithCircles(int count, int min_radius, int max_radius, int min_contrast, int max_contrast, double std, int blur, bool bSave) {
    cv::Mat image(max_radius * 2 * (count + 1), max_radius * 2 * (count + 1), CV_8UC1, cv::Scalar(0, 0, 0));
    int grid_spacing = max_radius * 2;
    int step = (max_radius - min_radius) / count;
    cv::Scalar color(min_contrast, min_contrast, min_contrast);
    int step_color = (max_contrast - min_contrast) / count;

    Statistics stats;

    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < count; ++j) {

            
            cv::Point center((j + 1) * grid_spacing, (i + 1) * grid_spacing);
            cv::Scalar color(min_contrast + step_color * j, min_contrast + step_color * j, min_contrast + step_color * j);
            cv::circle(image, center, min_radius + step * i, color, -1);
            
            Circle circle;
            circle.x = (j + 1) * grid_spacing;
            circle.y = (i + 1) * grid_spacing;
            circle.r = min_radius + step * i;
            circle.brightness = min_contrast + step_color * j;

            stats.circles.push_back(circle);
        }
        
    }
    stats.background_color = 0;
    stats.image_width = max_radius * 2 * (count + 1);
    stats.image_height = max_radius * 2 * (count + 1);
    stats.noise = std;
    stats.blur = blur;

    Mat new_image = addGaussianNoise(image, std) + image;
    cv::blur(new_image, new_image, cv::Size(blur, blur));
    ImageWithStats stats_img;
    stats_img.image = new_image;
    stats_img.stats = stats;
    if (bSave)
    {
        imwrite(GENERATE_FILENAME("generated_image_", num_file), new_image);
        num_file += 1;
        
    }
    
    
    return stats_img;
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
Mat connectedComponentsDetection(Mat image) {
    Mat labels;
    int num_objects = connectedComponents(image, labels);

    Mat colored_labels;
    cvtColor(image, colored_labels, COLOR_GRAY2BGR);

    for (int i = 1; i < num_objects; i++) {
        Mat component_mask = (labels == i);
        Vec3b color(255, 255, 255);
        colored_labels.setTo(color, component_mask);
    }
    cvtColor(image, colored_labels, COLOR_GRAY2BGR);
    return colored_labels;
}

////////////////////////////////////////////////////////////////////////////////
Mat LoG(Mat detected_blobs)
{
    Mat  dst;
    int kernel_size = 7;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
  
    GaussianBlur(detected_blobs, detected_blobs, Size(kernel_size, kernel_size), 0, 0, BORDER_DEFAULT);
    imwrite(GENERATE_FILENAME("gau_", kernel_size), detected_blobs);
    Laplacian(detected_blobs, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(dst, &minVal, &maxVal, &minLoc, &maxLoc);
    dst = (dst - minVal) * (255.0 / (maxVal - minVal)); 
    //dst.convertTo(dst,CV_8U);

    
   /* for (int y = 0; y < dst.rows; ++y) {
        for (int x = 0; x < dst.cols; ++x) {
            int brightness = dst.at<char>(y, x);

            if (brightness == 0) {
                cv::circle(dst, cv::Point(x, y), kernel_size, cv::Scalar(0, 0, 255), 1);
            }
        }
    }*/
    imwrite(GENERATE_FILENAME("gau_", kernel_size), dst);
    return dst;
    }
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

static void on_count_trackbar(int, void*)
{
    count = std::max(count_slider,1);
    ImageWithStats img = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value, blur_value, false);
    imshow("Initial image", img.image);

    /*binary_image = makeAdaptiveBinarization(init_image, threshold_value);
    imshow("Binary Image", binary_image);

    detected_blobs = connectedComponentsDetection(binary_image);
    imshow("Detected blobs", detected_blobs);*/

    detected_circles = LoG(img.image);
    imshow("Detected circles", detected_circles);

    mask = createMaskForBlackPixels(detected_circles, 7);
    imshow("Mask", mask);
}
////////////////////////////////////////////////////////////////////////////////
static void on_min_radius_trackbar(int, void*)
{
    min_radius = std::max(min_radius_slider, 1);
    ImageWithStats img = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value, blur_value, false);
    imshow("Initial image", img.image);

   /* binary_image = makeAdaptiveBinarization(init_image, thre  shold_value);
    imshow("Binary Image", binary_image);

    detected_blobs = connectedComponentsDetection(binary_image);
    imshow("Detected blobs", detected_blobs);*/

    detected_circles = LoG(img.image);
    imshow("Detected circles", detected_circles);
    mask = createMaskForBlackPixels(detected_circles, 7);
    imshow("Mask", mask);
}
////////////////////////////////////////////////////////////////////////////////
static void on_max_radius_trackbar(int, void*)
{
    max_radius = std::max(max_radius_slider, 1);
    ImageWithStats img = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value, blur_value, false);
    imshow("Initial image", img.image);

    /*binary_image = makeAdaptiveBinarization(init_image, threshold_value);
    imshow("Binary Image", binary_image);

    detected_blobs = connectedComponentsDetection(init_image);
    imshow("Detected blobs", detected_blobs);*/

    detected_circles = LoG(img.image);
    imshow("Detected circles", detected_circles);
    mask = createMaskForBlackPixels(detected_circles, 7);
    imshow("Mask", mask);
}
////////////////////////////////////////////////////////////////////////////////
static void on_max_contrast_trackbar(int, void*)
{
    max_contrast = std::max(max_contrast_slider, 1);
    ImageWithStats img = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value, blur_value, false);
    imshow("Initial image", img.image);

    /*binary_image = makeAdaptiveBinarization(init_image, threshold_value);
    imshow("Binary Image", binary_image);

    detected_blobs = connectedComponentsDetection(init_image);
    imshow("Detected blobs", detected_blobs);*/

    detected_circles = LoG(img.image);
    imshow("Detected circles", detected_circles);
    mask = createMaskForBlackPixels(detected_circles, 7);
    imshow("Mask", mask);
}
////////////////////////////////////////////////////////////////////////////////
static void on_min_contrast_trackbar(int, void*)
{
    min_contrast = std::max(min_contrast_slider, 1);
    ImageWithStats img = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value, blur_value, false);
    imshow("Initial image", img.image);

   /* binary_image = makeAdaptiveBinarization(init_image, threshold_value);
    imshow("Binary Image", binary_image);

    detected_blobs = connectedComponentsDetection(init_image);
    imshow("Detected blobs", detected_blobs);*/

    detected_circles = LoG(img.image);
    imshow("Detected circles", detected_circles);
    mask = createMaskForBlackPixels(detected_circles, 7);
    imshow("Mask", mask);
}
////////////////////////////////////////////////////////////////////////////////
static void on_std_trackbar(int, void*)
{
    std_value = std::max(std_slider, 1);
    ImageWithStats img = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value, blur_value, false);
    imshow("Initial image", img.image);

    /*binary_image = makeAdaptiveBinarization(init_image, threshold_value);
    imshow("Binary Image", binary_image);

    detected_blobs = connectedComponentsDetection(init_image);
    imshow("Detected blobs", detected_blobs);*/

    detected_circles = LoG(img.image);
    imshow("Detected circles", detected_circles);
    mask = createMaskForBlackPixels(detected_circles, 7);
    imshow("Mask", mask);
}
////////////////////////////////////////////////////////////////////////////////
//static void on_threshold_trackbar(int, void*)
//{
//    std_value = std::max(std_slider, 1);
//    ImageWithStats img = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value, blur_value, false);
//    imshow("Initial image", img.image);
//   /* threshold_value = std::max(threshold_slider, 1);
//    binary_image = makeAdaptiveBinarization(init_image, threshold_value);
//    imshow("Binary Image", binary_image);*/
//
//   /* detected_blobs = connectedComponentsDetection(init_image);
//    imshow("Detected blobs", detected_blobs);*/
//    
//    detected_circles = LoG(img.image);
//    imshow("Detected circles", detected_circles);
//}
/////////////////////////////////////////////////////////////////////////////////////////////
static void on_blur_trackbar(int, void*)
{    
    int blur = std::max(blur_value, 1);
    ImageWithStats img = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value, blur_value, false);
    imshow("Initial image", img.image);
    /* binary_image = makeAdaptiveBinarization(init_image, threshold_value);
     imshow("Binary Image", binary_image);

     detected_blobs = connectedComponentsDetection(binary_image);
     imshow("Detected blobs", detected_blobs);*/

    detected_circles = LoG(img.image);
    imshow("Detected circles", detected_circles);

    mask = createMaskForBlackPixels(detected_circles, 7);
    imshow("Mask", mask);

}
/////////////////////////////////////////////////////////////////////////////////////////////
void applyThreshold(cv::Mat& src, cv::Mat& dst, double threshold, int blockSize) {

    cv::adaptiveThreshold(src, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C,
        cv::THRESH_BINARY, blockSize, threshold);
}
/////////////////////////////////////////////////////////////////////////////////////////////
struct Metrics {
    int TP = 0;
    int TN = 0;
    int FP = 0;
    int FN = 0;
};
/////////////////////////////////////////////////////////////////////////////////////////////
double calculateIoU(const cv::Mat& mask, const Circle& groundTruth) {
    cv::Mat binaryMask;
    cv::threshold(mask, binaryMask, 0, 1, cv::THRESH_BINARY);

    // Ñîçäàåì êðóãîâóþ ìàñêó äëÿ ground truth
    cv::Mat circleMask = cv::Mat::zeros(mask.size(), CV_8U);
    cv::circle(circleMask, cv::Point(groundTruth.x, groundTruth.y), groundTruth.r, cv::Scalar(255), 1);

    double intersection = cv::countNonZero(binaryMask & circleMask);
    double unionAreaValue = cv::countNonZero(binaryMask) + cv::countNonZero(circleMask) - intersection;

    double iou = intersection / unionAreaValue;
    return iou;
}
/////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////
void evaluateQuality()
{
   ImageWithStats eval_image = generateImageWithCircles(count, min_radius, max_radius, min_contrast, max_contrast, std_value, blur_value, false);
   Mat  cur_mask = createMaskForBlackPixels(detected_circles, 7);

   std::vector<double> iou;
   for (int i = 0; i < eval_image.stats.value().circles.size(); i++)
   {
       double cur_iou = calculateIoU(cur_mask, eval_image.stats.value().circles[i]);
       iou.push_back(cur_iou);
   }
   Metrics final_metric = calculateMetrics(iou);
   cout << "TP: " << final_metric.FP << std::endl << "FP:" << final_metric.FP<< std::endl << "FN: " << final_metric.FN;
}
/////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    ImageWithStats s =  generateImageWithCircles(5, 5, 25,0,255, std_value,blur_value,false);
    src1 = src2 = s.image;
    count_slider = 5;
    min_radius = 2;
    max_radius = 25;
    //namedWindow("Image", WINDOW_AUTOSIZE);
    //namedWindow("Binary Image", WINDOW_AUTOSIZE);
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
    createTrackbar(TrackbarName, "TrackBars", &std_slider, std_slider_max, on_std_trackbar);
    on_std_trackbar(std_slider, 0);

    /*sprintf(TrackbarName, "Threshold");
    createTrackbar(TrackbarName, "TrackBars", &threshold_slider, threshold_slider_max, on_threshold_trackbar); 
    on_threshold_trackbar(threshold_slider, 0);*/

    sprintf(TrackbarName, "Blur");
    createTrackbar(TrackbarName, "TrackBars", &blur_slider, blur_slider_max, on_blur_trackbar);
    on_blur_trackbar(threshold_slider, 0);

    evaluateQuality();

    cv::Mat mask = createMaskForBlackPixels(detected_circles, 7);

    cv::imshow("Mask", mask);
    cv::waitKey(0);

    std::vector<int> counts {7,9,11,13,15};
    std::vector<int> min_radius{5, 12, 14, 16, 20};
    std::vector<int> max_radius{15, 25, 35, 50, 100};    
    std::vector<int> min_contrast{15, 20, 25, 30, 40};
    std::vector<int> max_contrast{100, 120, 140, 160, 180};
    std::vector<double> std{10,50,100,120,150};
    std::vector<int> blur{2, 2, 4, 4, 6};
    for (int i = 0; i < 5; i++)
    {
        ImageWithStats cur_stat = generateImageWithCircles(counts[i], min_radius[i], max_radius[i], min_contrast[i], max_contrast[i], std[i], blur[i], true);
        cv::imwrite(GENERATE_FILENAME("generated_image_", i), cur_stat.image);
        if(cur_stat.stats.has_value())
            writeInJSON(GENERATE_JSONNAME("parameters_", i), cur_stat.stats.value());
    }
   
 
    waitKey(0);
    return 0;
}
