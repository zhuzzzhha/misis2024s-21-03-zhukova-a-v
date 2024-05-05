#pragma once

#include <ostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <opencv2/opencv.hpp>


#define GENERATE_FILENAME(name, num) (name + std::to_string(num) + ".png")
#define GENERATE_JSONNAME(name, num) (name + std::to_string(num) + ".json")

#define FILES_ORDER_NAME "files_order.txt"

//--------------------------------------------------------------------------------
struct CircleInfo
{
	double x;
	double y;
	double r;
};
//--------------------------------------------------------------------------------
struct CirclesStatistics
{
	std::vector<CircleInfo> circles;
	int image_width;
	int image_height;
};
cv::Mat addGaussianNoise(cv::Mat& image, double std)
{
	cv::Mat gaussian_noise(image.rows, image.cols, CV_32FC3);
	cv::randn(gaussian_noise, cv::Scalar(0, 0, 0), cv::Scalar(std, std, std));

	cv::Mat noisy_image;
	image.convertTo(noisy_image, CV_32FC3);

	noisy_image += gaussian_noise;

	cv::Mat clipped_noisy_image;
	cv::threshold(noisy_image, clipped_noisy_image, 255, 255, cv::THRESH_TRUNC);
	cv::threshold(clipped_noisy_image, clipped_noisy_image, 0, 0, cv::THRESH_TOZERO);

	clipped_noisy_image.convertTo(clipped_noisy_image, CV_8UC3);
	return clipped_noisy_image;
}

//--------------------------------------------------------------------------------
void WriteCirclesInJSON(const std::string& filename, CirclesStatistics& stats)
{
	std::ostringstream jsonStream;
	jsonStream << "{\n";
	jsonStream << "  \"count_objects\": " << stats.circles.size() << ",\n";
	jsonStream << "  \"objects\": [\n";

	for (size_t i = 0; i < stats.circles.size(); ++i) {
		const CircleInfo& circle = stats.circles[i];
		jsonStream << "    {\n";
		jsonStream << "      \"centre\": {\"x\": " << circle.x << ", \"y\": " << circle.y << "},\n";
		jsonStream << "      \"radius\": " << circle.r << "\n";
		jsonStream << "    }";
		if (i < stats.circles.size() - 1) {
			jsonStream << ",";
		}
		jsonStream << "\n";
	}

	jsonStream << "  ]\n";
	jsonStream << "}\n";
	std::ofstream output_file(filename);

	output_file << jsonStream.str();
	output_file.close();
}
//--------------------------------------------------------------------------------
cv::Mat generateImageWithCircles(CirclesStatistics& img_stat, int count,
	int min_radius, int max_radius, int min_contrast, int max_contrast,
	double std, int blur, int& imageCounter) {
	cv::Mat image(max_radius * 2 * (count + 1), max_radius * 2 * (count + 1), CV_8UC3, cv::Scalar(0, 0, 0));
	int grid_spacing = max_radius * 2;
	int step = (max_radius - min_radius) / count;
	cv::Scalar color(min_contrast, min_contrast, min_contrast);
	int step_color = (max_contrast - min_contrast) / count;

	CirclesStatistics stats;

	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < count; ++j) {
			cv::Point center((j + 1) * grid_spacing, (i + 1) * grid_spacing);
			cv::Scalar color(min_contrast + step_color * j, min_contrast + step_color * j, min_contrast + step_color * j);
			cv::circle(image, center, min_radius + step * i, color, -1);

			CircleInfo circle;
			circle.x = (j + 1) * grid_spacing;
			circle.y = (i + 1) * grid_spacing;
			circle.r = min_radius + step * i;

			stats.circles.push_back(circle);
		}
	}
	stats.image_width = max_radius * 2 * (count + 1);
	stats.image_height = max_radius * 2 * (count + 1);

	cv::Mat new_image = addGaussianNoise(image, std) + image;
	cv::blur(new_image, new_image, cv::Size(blur, blur));

	imwrite(GENERATE_FILENAME("gr_truth_image", imageCounter), new_image);
	++imageCounter;

	return new_image;
}

