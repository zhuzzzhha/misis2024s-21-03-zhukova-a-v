#include <opencv2/opencv.hpp>
#include <algorithm>
#include <filesystem>
#include <string>

using namespace cv;

///////////////////////////////////////////////////////////////////////////////
void calcHistogramColor(const cv::Mat& image, std::vector<cv::Mat>& histograms,int max_value = 256, int hist_size = 256)
{
    std::vector<cv::Mat> channels;
    cv::split(image, channels);

    float range[] = { 0, 256 };
    const float* hist_range = { range };
    bool uniform = true;
    bool accumulate = false;

    Mat hist;
    cv::Mat histBlue, histGreen, histRed;
    cv::calcHist(&channels[0], 1, 0, cv::Mat(), histBlue, 1, &hist_size, &hist_range, uniform, accumulate);
    cv::calcHist(&channels[1], 1, 0, cv::Mat(), histGreen, 1, &hist_size, &hist_range, uniform, accumulate);
    cv::calcHist(&channels[2], 1, 0, cv::Mat(), histRed, 1, &hist_size, &hist_range, uniform, accumulate);


    histograms = { histBlue, histGreen, histRed };

}
///////////////////////////////////////////////////////////////////////////////
void calcHistogramGray(const cv::Mat& image, cv::Mat& histogram, int max_value = 256, int hist_size = 256)
{
    std::vector<cv::Mat> channels;

    float range[] = { 0, 256 };
    const float* hist_range = { range };
    bool uniform = true;
    bool accumulate = false;

    Mat hist;
    cv::calcHist(&channels[0], 1, 0, cv::Mat(), hist, 1, &hist_size, &hist_range, uniform, accumulate);

}
///////////////////////////////////////////////////////////////////////////////
void normalizeHistogram(cv::Mat histogram, float percentile)
{
    //size_t total_pixels = std::accumulate(histogram, histogram, 0);
    //size_t leftIndex = 0;
    //size_t rightIndex = hist.size() - 1;
    //size_t count = 0;
    //int leftThreshold = 0.05 * totalPixels; // 5% от общего числа пикселей
    //int rightThreshold = 0.95 * totalPixels; // 95% от общего числа пикселей
}
///////////////////////////////////////////////////////////////////////////////

void calculateBlackQuantile(cv::Mat histogram, int& black_quantile, int& white_quantile, double quantile, int hist_size, int total_pixels_count)
{
    double cumulative_sum = 0;
    black_quantile = 0;
    for (int i = 0; i < hist_size; ++i) {
        cumulative_sum += histogram.at<float>(i);   
        if (cumulative_sum / total_pixels_count >= (1-quantile)) {
            black_quantile = i;
            break;
        }
    }
     cumulative_sum = 0;
     white_quantile = 0;
       for (int i = hist_size - 2; i >= 0; --i) {
        cumulative_sum += histogram.at<float>(i);
        if (cumulative_sum / total_pixels_count >= (1 - quantile)) {
            white_quantile = i;
            break;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
void calculateMinMaxFromHistogram(int& c_low, int& c_high, cv::Mat histogram)
{
    c_low = 0;
    int i = 0;
    while (i < histogram.total() && !histogram.at<int>(i))
        i++;
    c_low = i;
    i = histogram.total() - 1;
    while (i >= 0 && !histogram.at<int>(i))
        i--;
    c_high = i;
}
///////////////////////////////////////////////////////////////////////////////
int calculateAutocontrast(int c_low, int c_high, int black_quantile, int white_quantile, int current_value)
{
    int new_value;
    if (current_value <= black_quantile)
        current_value = black_quantile;
    else if (current_value >= white_quantile)
        current_value = white_quantile;
    else
        current_value = black_quantile + (current_value - c_low) * (white_quantile - black_quantile) / (c_high - c_low);
    return current_value;
}
///////////////////////////////////////////////////////////////////////////////
void autoContrastForEachChannel(std::vector<double> c_low, std::vector<double> c_high, std::map<std::string,int> black_quantiles, std::map<std::string, int> white_quantiles, Mat& image)
{

    for (int i = 0; i < image.rows; i++)
    {
        cv::Vec3b* ptr = image.ptr<cv::Vec3b>(i);
        for (int j = 0; j < image.cols; j++)
        {
            ptr[j] = cv::Vec3b(ptr[j][2], ptr[j][1], ptr[j][0]);

            int blue_value = calculateAutocontrast(c_low[2], c_high[2], black_quantiles["blue"], white_quantiles["blue"], ptr[j][2]);
            int green_value = calculateAutocontrast(c_low[1], c_high[1], black_quantiles["green"], white_quantiles["green"], ptr[j][1]);
            int red_value = calculateAutocontrast(c_low[0], c_high[0], black_quantiles["red"], white_quantiles["red"], ptr[j][0]);
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(blue_value, green_value, red_value);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
int main()
{
    int black_quantile, white_quantile;
    int c_low, c_high;
    float quantile = 0.7;
    std::map<std::string, int> black_quantiles = {
        {"blue",0},
        {"green", 0},
        {"red", 0 } };
    std::map<std::string, int> white_quantiles = {
        {"blue",255},
        {"green", 255},
        {"red", 255 } };

    //чтение изображений
    std::filesystem::path executable_path = std::filesystem::current_path();
    std::string color_image_path = "\\test_color.png";
    color_image_path = executable_path.string() + color_image_path;
    

    std::string gray_image_path = "\\test_gray.png";
    gray_image_path = executable_path.string() + color_image_path;


    //вычисляем диапазон для цветного изображения
    cv::Mat color_image = cv::imread(color_image_path, cv::IMREAD_COLOR);
    std::vector<cv::Mat> channels;
    cv::split(color_image, channels);

    std::vector<double> min_color_values = { 0,0,0 };
    std::vector<double> max_color_values = { 0,0,0 };
    minMaxLoc(channels[0], &min_color_values[0], &max_color_values[0]);
    minMaxLoc(channels[1], &min_color_values[1], &max_color_values[1]);
    minMaxLoc(channels[2], &min_color_values[2], &max_color_values[2]);
    double total_min = *min_element(min_color_values.begin(), min_color_values.end());
    double total_max = *max_element(max_color_values.begin(), max_color_values.end());

    cv::Mat gray_image = cv::imread(gray_image_path, cv::IMREAD_GRAYSCALE);
    double min_val_gray, max_val_gray;
    minMaxLoc(gray_image, &min_val_gray, &max_val_gray);

    //вычисляем гистограмму
    std::vector<cv::Mat> histograms_color;
    calcHistogramColor(color_image, histograms_color, total_max, total_max);

    Mat histogram_gray;
//    calcHistogramGray(gray_image, histogram_gray, max_val_gray, max_val_gray);

    //вычисляем квантили
    calculateBlackQuantile(histograms_color[0], black_quantiles["blue"], white_quantiles["blue"], quantile, 256, color_image.cols * color_image.rows);
    calculateBlackQuantile(histograms_color[1], black_quantiles["green"], white_quantiles["green"], quantile, 256, color_image.cols * color_image.rows);
    calculateBlackQuantile(histograms_color[2], black_quantiles["red"], white_quantiles["red"], quantile, 256, color_image.cols * color_image.rows);

    //поканально контрастируем
    autoContrastForEachChannel(min_color_values, max_color_values, black_quantiles, white_quantiles, color_image);

    cv::imshow("Image", color_image);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
