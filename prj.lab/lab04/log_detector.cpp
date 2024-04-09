#include "log_detector.h"

using namespace cv;

Mat LoG(Mat& src_image)
{
    int blockSize = 15;
    int scaler = 20;
    //cv::Mat resizedImage;
    //cv::resize(src_image, resizedImage, cv::Size(src_image.cols / scaler, src_image.rows / scaler));
    cv::Mat blurred, laplacianImage;
    cv::GaussianBlur(src_image, blurred, cv::Size(blockSize, blockSize), 0);
    cv::Laplacian(blurred, laplacianImage, CV_32F, blockSize);

    cv::Mat morph_image;
    int morph_size = 20;
    int accumulatorThreshold = 20;
    int morph_elem = 2;
    int operation = cv::MORPH_ERODE;
    cv::Mat element = cv::getStructuringElement(morph_elem, cv::Size(2 * morph_size + 1, 2 * morph_size + 1), cv::Point(morph_size, morph_size));
    cv::morphologyEx(laplacianImage, morph_image, operation, element);
    return morph_image;
}