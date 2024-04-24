#pragma once

#include <ostream>
#include <string>
#include <filesystem>
#include <opencv2/opencv.hpp>


#define GENERATE_FILENAME(name, num) (name + to_string(num) + ".png")
#define GENERATE_JSONNAME(name, num) (name + to_string(num) + ".json")

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
//--------------------------------------------------------------------------------
std::string getCurrentDateTimeAsString() {
	auto currentTime = std::chrono::system_clock::now();

	std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

	std::tm* currentTime_tm = std::localtime(&currentTime_t);

	std::ostringstream oss;
	oss << (currentTime_tm->tm_min * 60 + currentTime_tm->tm_sec);
	return oss.str();
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
void generateImageWithCircles(CirclesStatistics& stats, int count, int min_radius, int max_radius,
	std::filesystem::path file_path, std::string& file_name, int& imageCounter) {
	cv::Mat image(max_radius * 2 * (count + 1), max_radius * 2 * (count + 1), CV_32F, cv::Scalar(0, 0, 0));
	int grid_spacing = max_radius * 2;
	int step = (max_radius - min_radius) / count;
	cv::Scalar color(255, 255, 255);

	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < count; ++j) {


			cv::Point center((j + 1) * grid_spacing, (i + 1) * grid_spacing);
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

	auto image_path = file_path / GENERATE_FILENAME("image_circles", imageCounter);
	file_name = image_path.string();
	auto json_path = file_path / GENERATE_JSONNAME("json_circles", imageCounter);

	++imageCounter;
	std::ofstream f(FILES_ORDER_NAME);
	if (f.is_open())
		f << imageCounter << '\n';
	f.close();

	cv::imwrite(image_path.string(), image);
	WriteCirclesInJSON(json_path.string(), stats);

}
//--------------------------------------------------------------------------------
cv::Mat DrawDifference(CirclesStatistics& ground_truth, CirclesStatistics& error_img)
{
	cv::Mat image(ground_truth.image_width, ground_truth.image_height, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Scalar color_gr_truth(0, 255, 0);
	cv::Scalar color_error(0, 0, 255);
	for (int i = 0; i < ground_truth.circles.size(); ++i) {


		cv::Point center_gr_truth(ground_truth.circles[i].x, ground_truth.circles[i].y);
		cv::circle(image, center_gr_truth, ground_truth.circles[i].r, color_gr_truth, 1);

		cv::Point center_error(error_img.circles[i].x, error_img.circles[i].y);
		cv::circle(image, center_error, error_img.circles[i].r, color_error, 1);
	}
	return image;

}
