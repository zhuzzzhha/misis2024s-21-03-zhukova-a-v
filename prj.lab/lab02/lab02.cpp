#include <opencv2/opencv.hpp>

using namespace cv;

//cv::Mat calculateHistogram(int hist_size, float range[], cv::Mat image)
//{
//    int histSize = 256;
//    float range[] = { 0, 256 };
//    const float* histRange = { range };
//    Mat hist;
//    calcHist(&image, 1, 0, Mat(), hist, 1, &histSize, &histRange);
//}

cv::Mat makeImage(std::vector<unsigned char> brightness, int height_1 = 256, int height_2 = 209, int radius = 83)
{
    Mat image = Mat::zeros(height_1, height_1, CV_8UC1);
    image.setTo(Scalar(brightness[0], brightness[0], brightness[0]));

    rectangle(image, Point((height_1 - height_2) / 2, (height_1 - height_2) / 2), Point((height_1 - height_2) / 2 + height_2, (height_1 - height_2) / 2 + height_2), Scalar(brightness[1], brightness[1], brightness[1]), -1);

    circle(image, Point(height_1 / 2, height_1 / 2), radius, Scalar(brightness[2], brightness[2], brightness[2]), -1);

    return image;
}

cv::Mat makeHistogram(const cv::Mat& image, int max_value = 256, int hist_size = 256)
{
    float range[] = { 0, max_value };
    const float* hist_range[] = { range };
    int channels[] = { 0 };
    bool uniform = true, accumulate = false;
    cv::Mat hist;
    calcHist(&image, 1, channels, Mat(), hist, 1, &hist_size, hist_range);

    int hist_w = 256, hist_h = 256;
    int bin_w = cvRound((double)hist_w / hist_size);
    cv::Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(230, 230, 230));
    normalize(hist, hist, histImage.rows, 0);

    for (int i = 1; i < hist_size; i++)
    {
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
            Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
            Scalar(0, 0, 0), 1, 8, 0);
    }
    return histImage;
}
cv::Mat addGaussianNoise(cv::Mat image, double std)
{
    cv::Mat gaussian_noise = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
    
    cv::randn(gaussian_noise, 0, std);
    return gaussian_noise;
}
int main() {

    int height_1 = 256, height_2 = 209, radius = 83, point_1 = 0, point_2 = 10, point_3 = 20;

    std::vector<std::vector<unsigned char>> brightnesses = { {0,127,255},{20,127,235},
        {55,127,235}, {90,127,165} };
    Mat src_image = makeImage((brightnesses[0]), height_1, height_2, radius);
    Mat histogram = makeHistogram(src_image, 230, 256);
    Mat noisy_image_1 = addGaussianNoise(src_image, 3) + src_image;
    Mat noisy_image_2 = addGaussianNoise(src_image, 7) + src_image;
    Mat noisy_image_3 = addGaussianNoise(src_image, 15) + src_image;
    for (int i = 1; i < 4; ++i)
    {
        Mat image = makeImage((brightnesses[i]), height_1, height_2, radius);
       // bool f = cv::imwrite("C:/misis2024s-21-03-zhukova-a-v/bin.dbg/image.bmp", image);
        Mat hist = makeHistogram(image, 230, 256);
        Mat noisy_1 = addGaussianNoise(image, 3) + image;
        Mat noisy_2 = addGaussianNoise(image, 7) + image;
        Mat noisy_3 = addGaussianNoise(image, 15) + image;
        cv::hconcat(src_image, image, src_image);
        cv::hconcat(noisy_image_1, noisy_1, noisy_image_1);
        cv::hconcat(noisy_image_2, noisy_2, noisy_image_2);
        cv::hconcat(noisy_image_3, noisy_3, noisy_image_3);
        cv::hconcat(histogram,hist, histogram);

    }
    cv::Mat result_image;
    cv::vconcat(src_image, noisy_image_1, result_image);
    cv::vconcat(result_image, noisy_image_2, result_image);
    cv::vconcat(result_image, noisy_image_3, result_image);
    cv::vconcat(result_image, histogram, result_image);


    imshow("", result_image);
    imwrite("C:/misis2024s-21-03-zhukova-a-v/bin.dbg/result.bmp", result_image);
        waitKey(0);
    destroyAllWindows();

    return 0;
}
