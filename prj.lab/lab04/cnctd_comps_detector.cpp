#include "cnctd_comps_detector.h"

using namespace cv;
//--------------------------------------------------------------------------------
Mat makeAdaptiveBinarization(Mat& image, double max_value)
{
    Mat binary_image;
    image.convertTo(image, CV_8UC1);
    cv::adaptiveThreshold(image, binary_image, max_value, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 55, 0);
    return binary_image;
}
//--------------------------------------------------------------------------------
std::vector<Circle> detectConnectedComponents(const cv::Mat& inputImage, Mat& visualizationImage, int areaThreshold)
{
    cv::Mat grayImage, binaryImage;

    if (inputImage.channels() == 3) {
        cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    }
    else {
        grayImage = inputImage.clone();
    }

    cv::threshold(grayImage, binaryImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::Mat labels, stats, centroids;
    int nLabels = cv::connectedComponentsWithStats(binaryImage, labels, stats, centroids, 8, CV_32S);

    std::vector<Circle> circles;
    if (inputImage.channels() == 1) {
        cv::cvtColor(inputImage, visualizationImage, cv::COLOR_GRAY2BGR);
    }
    else {
        visualizationImage = inputImage.clone();
    }

    visualizationImage = cv::Mat::zeros(inputImage.size(), CV_8UC3); // Изображение исходного размера

    for (int i = 1; i < nLabels; i++) {
        if (stats.at<int>(i, cv::CC_STAT_AREA) > areaThreshold) {
            double centroidX = centroids.at<double>(i, 0);
            double centroidY = centroids.at<double>(i, 1);
            int area = stats.at<int>(i, cv::CC_STAT_AREA);
            double radius = std::sqrt(area / M_PI);
            circles.push_back({ centroidX, centroidY, radius });
            cv::circle(visualizationImage, cv::Point(centroidX, centroidY), radius, cv::Scalar(0, 0, 255), 2);
        }
    }

    return circles;
}
//--------------------------------------------------------------------------------
std::vector<Circle> DoDetection(Mat& src_image, Mat& dst_image, double max_value, int treshold_area)
{
    Mat binary_image = makeAdaptiveBinarization(src_image, max_value);
    std::vector<Circle> detection_vec = detectConnectedComponents(binary_image, dst_image, treshold_area);
    return detection_vec;
}
//--------------------------------------------------------------------------------