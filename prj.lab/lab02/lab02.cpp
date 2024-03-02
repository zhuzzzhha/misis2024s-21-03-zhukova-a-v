#include <opencv2/opencv.hpp>
#include <filesystem>

using namespace cv;

///////////////////////////////////////////////////////////////////////////////////////
cv::Mat makeImage(std::vector<unsigned char> brightness, int height_1 = 256, int height_2 = 209, int radius = 83)
{
    Mat image = Mat::zeros(height_1, height_1, CV_32F);
    image.setTo(Scalar(brightness[0], brightness[0], brightness[0]));

    rectangle(image, Point((height_1 - height_2) / 2, (height_1 - height_2) / 2), Point((height_1 - height_2) / 2 + height_2, (height_1 - height_2) / 2 + height_2), Scalar(brightness[1], brightness[1], brightness[1]), -1);

    circle(image, Point(height_1 / 2, height_1 / 2), radius, Scalar(brightness[2], brightness[2], brightness[2]), -1);

    return image;
}
///////////////////////////////////////////////////////////////////////////////////////
cv::Mat makeHistogram(const cv::Mat& image, int max_value = 256, int hist_size = 256)
{
    float range[] = { 0, max_value };
    const float* hist_range[] = { range };
    int channels[] = { 0 };
    bool uniform = true, accumulate = false;
    cv::Mat hist ;
  

    calcHist(&image, 1, channels, Mat(), hist, 1, &hist_size, hist_range);

    double maxVal;
    minMaxLoc(hist, 0, &maxVal);
    hist *= 230 / maxVal;

    int hist_w = 256, hist_h = 256;
    int bin_w = cvRound((double)hist_w / hist_size);
    Mat histImage(hist_h, hist_w, CV_32F, Scalar(230));
    for (int i = 0; i < hist_size; i++) {
        rectangle(histImage, Point(bin_w * i, hist_h),
            Point(bin_w * i + bin_w, hist_h - cvRound(hist.at<float>(i))),
            Scalar(0, 0, 0), -1);
    }
    return histImage;
}
///////////////////////////////////////////////////////////////////////////////////////
cv::Mat addGaussianNoise(cv::Mat image, double std)
{
    cv::Mat gaussian_noise = cv::Mat::zeros(image.rows, image.cols, CV_32F);
    
    cv::randn(gaussian_noise, 0, std);

    return gaussian_noise;
}
///////////////////////////////////////////////////////////////////////////////////////
void makeHconcat(std::vector<Mat>& images)
{
    for (int i = 1; i < images.size(); i++)
    {
        cv::hconcat(images[0], images[i], images[0]);
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void makeVconcat(std::vector<Mat>& images)
{
    for (int i = 1; i < images.size(); i++)
    {
        cv::vconcat(images[0], images[i], images[0]);
    }
}
///////////////////////////////////////////////////////////////////////////////////////
int main() {

    int height_1 = 256, height_2 = 209, radius = 83, point_1 = 0, point_2 = 10, point_3 = 20;

    std::string output_file = "lab_2.png";
    std::filesystem::path executable_path = std::filesystem::current_path();

    std::vector<std::vector<unsigned char>> brightnesses = { {0,127,255},{20,127,235},
        {55,127,235}, {90,127,165} };

    std::vector<Mat> src_images;

    //src images
    for (auto brightness : brightnesses)
    {
        Mat src_image = makeImage(brightness, height_1, height_2, radius);
        src_images.push_back(src_image);
    }

    std::map<int, std::vector<Mat>> noise_images;
    std::vector<int> noise = { 3,7,15 };

    //noise_images
    for (auto value : noise)
    {
        for (auto& image : src_images)
        {
            Mat noisy_image = addGaussianNoise(image, value) + image;
            noise_images[value].push_back(noisy_image);
        }
    }

    //histograms
    std::map<int, std::vector<Mat>> histograms;
    for (auto& [value, images] : noise_images)
    {
        for (auto& image : images)
        {
            Mat hist = makeHistogram(image, 230, 256);
            histograms[value].push_back(hist);
        }
    }
    
    std::vector<Mat> hconcat_images;
    makeHconcat(src_images);
    hconcat_images.push_back(src_images[0]);

    for (auto& [value, images] : noise_images)
         makeHconcat(images);

    for (auto& [value, images] : histograms)
        makeHconcat(images);

    for (auto& [key, value]: noise_images)
    {
        hconcat_images.push_back(noise_images[key][0]);
        hconcat_images.push_back(histograms[key][0]);
    }
    makeVconcat(hconcat_images);

    cv::Mat result_image = hconcat_images[0];



    //noise_images
   /* Mat src_image = makeImage((brightnesses[0]), height_1, height_2, radius);
    
    Mat noisy_image_1 = addGaussianNoise(src_image, 3) + src_image;
    Mat noisy_image_2 = addGaussianNoise(src_image, 7) + src_image;
    Mat noisy_image_3 = addGaussianNoise(src_image, 15) + src_image;

    Mat histogram_1 = makeHistogram(noisy_image_1, 230, 256);
    Mat histogram_2 = makeHistogram(noisy_image_2, 230, 256);
    Mat histogram_3 = makeHistogram(noisy_image_3, 230, 256);
    for (int i = 1; i < 4; ++i)
    {
        Mat image = makeImage((brightnesses[i]), height_1, height_2, radius);
        
        Mat noisy_1 = addGaussianNoise(image, 3) + image;
        Mat noisy_2 = addGaussianNoise(image, 7) + image;
        Mat noisy_3 = addGaussianNoise(image, 15) + image;

        Mat hist_1 = makeHistogram(noisy_1, 230, 256);
        Mat hist_2 = makeHistogram(noisy_2, 230, 256);
        Mat hist_3 = makeHistogram(noisy_3, 230, 256);

        cv::hconcat(src_image, image, src_image);
        cv::hconcat(noisy_image_1, noisy_1, noisy_image_1);
        cv::hconcat(noisy_image_2, noisy_2, noisy_image_2);
        cv::hconcat(noisy_image_3, noisy_3, noisy_image_3);


        cv::hconcat(histogram_1,hist_1, histogram_1);
        cv::hconcat(histogram_2, hist_2, histogram_2);
        cv::hconcat(histogram_3, hist_3, histogram_3);

    }
    cv::Mat result_image;
    cv::vconcat(src_image, noisy_image_1, result_image);
    cv::vconcat(result_image, noisy_image_2, result_image);
    cv::vconcat(result_image, noisy_image_3, result_image);
    cv::vconcat(result_image, histogram_1, result_image);
    cv::vconcat(result_image, histogram_2, result_image);
    cv::vconcat(result_image, histogram_3, result_image);*/


    imshow("", result_image);
    if (output_file.size()) {
        std::filesystem::path full_path = executable_path / output_file;
        cv::imwrite(full_path.string(), result_image);
    }
  
    waitKey(0);
    destroyAllWindows();

    return 0;
}
