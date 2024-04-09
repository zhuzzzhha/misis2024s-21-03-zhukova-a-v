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
Mat connectedComponentsDetection(Mat& image) 
{
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
//--------------------------------------------------------------------------------
Mat DoDetection(Mat& image, double max_value)
{
    Mat binary_image = makeAdaptiveBinarization(image, max_value);
    Mat connected_comps_image = connectedComponentsDetection(binary_image);
    return connected_comps_image;
}