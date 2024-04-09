#pragma once

#include "image_generator.h"
#include "log_detector.h"

using namespace cv;
using std::cout;
//--------------------------------------------------------------------------------
void writeInJSON(const std::string& filename, Statistics& stats)
{
    std::ostringstream jsonStream;
    jsonStream << "{";
    jsonStream << "\"background\":{";
    jsonStream << "\"size\":[" << stats.image_width << "," << stats.image_height << "],";
    jsonStream << "\"color\":" << stats.background_color << ",";
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
//--------------------------------------------------------------------------------
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
    clipped_noisy_image.convertTo(clipped_noisy_image, CV_32F);

    return clipped_noisy_image;
}
//--------------------------------------------------------------------------------
void generateImageWithCircles(ImageWithStats& img_stat, int count, int min_radius, int max_radius, int min_contrast, int max_contrast, double std, int blur, bool bSave) {
    cv::Mat image(max_radius * 2 * (count + 1), max_radius * 2 * (count + 1), CV_32F, cv::Scalar(0, 0, 0));
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
    img_stat.image = new_image;
    img_stat.stats = stats;
    if (bSave)
    {
        imwrite(GENERATE_FILENAME("generated_image_", num_file), new_image);
        num_file += 1;

    }

}
