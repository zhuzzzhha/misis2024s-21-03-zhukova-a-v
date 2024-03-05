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
    float range[] = { 0, max_value };
    const float* hist_range[] = { range };
    int channels[] = { 0 };
    bool uniform = true, accumulate = false;
    calcHist(&image, 1, channels, Mat(), histogram, 1, &hist_size, hist_range);

}
///////////////////////////////////////////////////////////////////////////////
void makeVconcat(std::vector<Mat>& images)
{
    for (int i = 1; i < images.size(); i++)
    {
        cv::vconcat(images[0], images[i], images[0]);
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void calculateQuantile(cv::Mat histogram, int& black_quantile, int& white_quantile, double quantile, int hist_size, int total_pixels_count)
{
    double cumulative_sum = 0;
    black_quantile = 0;
    for (int i = 0; i < hist_size; ++i) {
        cumulative_sum += histogram.at<float>(i);   
        if (cumulative_sum / total_pixels_count >= (quantile)) {
            black_quantile = i;
            break;
        }
    }
     cumulative_sum = 0;
     white_quantile = 0;
       for (int i = hist_size - 2; i >= 0; --i) {
        cumulative_sum += histogram.at<float>(i);
        if (cumulative_sum / total_pixels_count >= (quantile)) {
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
        //current_value = black_quantile + (current_value - c_low) * (white_quantile - black_quantile) / (c_high - c_low);
        current_value = c_low + (current_value - black_quantile) * (c_high - c_low) / (white_quantile - black_quantile);
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
void autoContrastForGray(double c_low, double c_high, int black_quantile, int white_quantile, Mat& image)
{

    for (int i = 0; i < image.rows; i++) 
    {
        uchar* ptr = image.ptr<uchar>(i);
        for (int j = 0; j < image.cols; j++)
        {
            int gray_value = calculateAutocontrast(c_low, c_high, black_quantile, white_quantile, ptr[j]);
            image.at<uchar>(i, j) = static_cast<uchar>(gray_value);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
void autoContrastForAllChannels(std::vector<double> c_low, std::vector<double> c_high, std::map<std::string, int> black_quantiles, std::map<std::string, int> white_quantiles, Mat& image)
{
    int black_quantile = 0;
    for (auto& [color, value] : black_quantiles)
        black_quantile = std::max(value, black_quantile);

    int white_quantile = 0;
    for (auto& [color, value] : white_quantiles)
        white_quantile = std::max(value, white_quantile);

    for (int i = 0; i < image.rows; i++)
    {
        cv::Vec3b* ptr = image.ptr<cv::Vec3b>(i);
        for (int j = 0; j < image.cols; j++)
        {
            ptr[j] = cv::Vec3b(ptr[j][2], ptr[j][1], ptr[j][0]);

            int blue_value = calculateAutocontrast(c_low[2], c_high[2], black_quantile, white_quantile, ptr[j][2]);
            int green_value = calculateAutocontrast(c_low[1], c_high[1], black_quantile, white_quantile, ptr[j][1]);
            int red_value = calculateAutocontrast(c_low[0], c_high[0], black_quantile, white_quantile, ptr[j][0]);
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(blue_value, green_value, red_value);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    int black_quantile_gray, white_quantile_gray;
    int c_low, c_high;
    double quantile = 0.05;

    cv::CommandLineParser parser(argc, argv,
        "{q         | 0.05| quantile}");

    quantile = parser.get<double>("q");
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
    gray_image_path = executable_path.string() + gray_image_path;


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
    calcHistogramGray(gray_image, histogram_gray, max_val_gray, max_val_gray);
    calculateQuantile(histogram_gray, black_quantile_gray, white_quantile_gray, quantile, max_val_gray, gray_image.cols * gray_image.rows);

    //вычисляем квантили
    calculateQuantile(histograms_color[0], black_quantiles["blue"], white_quantiles["blue"], quantile, 256, color_image.cols * color_image.rows);
    calculateQuantile(histograms_color[1], black_quantiles["green"], white_quantiles["green"], quantile, 256, color_image.cols * color_image.rows);
    calculateQuantile(histograms_color[2], black_quantiles["red"], white_quantiles["red"], quantile, 256, color_image.cols * color_image.rows);

    //поканально контрастируем
    cv::Mat contrast_each_cahnnels_color_image;
    cv::Mat contrast_all_cahnnels_color_image;
    contrast_each_cahnnels_color_image = color_image.clone();
    contrast_all_cahnnels_color_image = color_image.clone();

    autoContrastForEachChannel(min_color_values, max_color_values, black_quantiles, white_quantiles, contrast_each_cahnnels_color_image);
    autoContrastForAllChannels(min_color_values, max_color_values, black_quantiles, white_quantiles, contrast_all_cahnnels_color_image);
    autoContrastForGray(min_val_gray, max_val_gray, black_quantile_gray, white_quantile_gray, gray_image);

    //std::vector<Mat> images = { contrast_all_cahnnels_color_image, contrast_each_cahnnels_color_image };
    //makeVconcat(images);

    std::string output_file_gray = "gray_image.png";
    std::string output_file_color_all_channels = "color_image_all_channels.png";
    std::string output_file_color_each_channels = "color_image_each_channels.png";
    std::filesystem::path output_file_gray_full = executable_path / output_file_gray;
    std::filesystem::path output_file_color_all = executable_path / output_file_color_all_channels;
    std::filesystem::path output_file_color_each = executable_path / output_file_color_each_channels;

    imwrite(output_file_color_all.string(), contrast_all_cahnnels_color_image);
    imwrite(output_file_color_each.string(), contrast_each_cahnnels_color_image);
    imwrite(output_file_gray_full.string(), gray_image);
    cv::imshow("Image", gray_image);
    cv::waitKey(0);
    cv::destroyAllWindows();
}
