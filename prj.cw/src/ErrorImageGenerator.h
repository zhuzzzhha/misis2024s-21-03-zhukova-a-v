#pragma once
#include "GroundTruthGenerator.h"

//--------------------------------------------------------------------------------
struct CircleInfo;
//--------------------------------------------------------------------------------
enum class ErrorType
{
	DILATION,
	EROSION,
	OBJ_LOCALIZATION,
	BNDRY_LOCALIZATION,
	BNDRY_APPROXAIMATION,
	INNER_MASK
};
//--------------------------------------------------------------------------------
struct ErrorParams
{
	double scale_dilation; //коэффициент уменьшения радиуса круга 
	double scale_erosion; //коэффициент увеличения радиуса круга
	double boundary_localization; //
	double obj_localization; //смещение от центра по OX
	double boundary_approximation;
	double inner_mask;
};
//--------------------------------------------------------------------------------
void GenerateDilationImage(ErrorParams params, CirclesStatistics& stats,
	CirclesStatistics& error_stats, std::filesystem::path file_path,
	std::string& file_name, int& imageCounter)
{
	cv::Mat image(stats.image_width, stats.image_height, CV_32F, cv::Scalar(0, 0, 0));
	cv::Scalar color(255, 255, 255);

	for (int i = 0; i < stats.circles.size(); ++i) {
		

			cv::Point center(stats.circles[i].x, stats.circles[i].y);
			cv::circle(image, center, stats.circles[i].r/params.scale_dilation, color, -1);

			CircleInfo circle;
			circle.x = stats.circles[i].x;
			circle.y = stats.circles[i].y;
			circle.r = stats.circles[i].r/ params.scale_dilation;

			error_stats.circles.push_back(circle);

	}

	error_stats.image_width = stats.image_width;
	error_stats.image_height = stats.image_height;

	auto image_path = file_path / GENERATE_FILENAME("dilation_image_circles", imageCounter);
	auto json_path = file_path / GENERATE_JSONNAME("dilation_json_circles", imageCounter);
	file_name = image_path.string();

	bool f = cv::imwrite(image_path.string(), image);
	WriteCirclesInJSON(json_path.string(), stats);
}
//--------------------------------------------------------------------------------
void GenerateErosionImage(ErrorParams params, CirclesStatistics& stats,
	CirclesStatistics& error_stats, std::filesystem::path file_path,
	std::string& file_name, int& imageCounter)
{
	cv::Mat image(stats.image_width, stats.image_height, CV_32F, cv::Scalar(0, 0, 0));
	cv::Scalar color(255, 255, 255);

	for (int i = 0; i < stats.circles.size(); ++i) {


		cv::Point center(stats.circles[i].x, stats.circles[i].y);
		cv::circle(image, center, stats.circles[i].r * params.scale_erosion, color, -1);

		CircleInfo circle;
		circle.x = stats.circles[i].x;
		circle.y = stats.circles[i].y;
		circle.r = stats.circles[i].r * params.scale_erosion;

		error_stats.circles.push_back(circle);

	}

	error_stats.image_width = stats.image_width;
	error_stats.image_height = stats.image_height;

	auto image_path = file_path / GENERATE_FILENAME("erosion_image_circles", imageCounter);
	auto json_path = file_path / GENERATE_JSONNAME("erosion_json_circles", imageCounter);
	file_name = image_path.string();

	bool f = cv::imwrite(image_path.string(), image);
	WriteCirclesInJSON(json_path.string(), stats);
}
//--------------------------------------------------------------------------------
void GenerateObjLocalizationImage(ErrorParams params, CirclesStatistics& stats,
	CirclesStatistics& error_stats, std::filesystem::path file_path,
	std::string& file_name, int& imageCounter)
{
	cv::Mat image(stats.image_width, stats.image_height, CV_32F, cv::Scalar(0, 0, 0));
	cv::Scalar color(255, 255, 255);

	for (int i = 0; i < stats.circles.size(); ++i) {


		cv::Point center(stats.circles[i].x+ params.obj_localization, stats.circles[i].y);
		cv::circle(image, center, stats.circles[i].r, color, -1);

		CircleInfo circle;
		circle.x = stats.circles[i].x + params.obj_localization;
		circle.y = stats.circles[i].y;
		circle.r = stats.circles[i].r;

		error_stats.circles.push_back(circle);

	}

	error_stats.image_width = stats.image_width;
	error_stats.image_height = stats.image_height;

	auto image_path = file_path / GENERATE_FILENAME("obj_locallization_image_circles", imageCounter);
	auto json_path = file_path / GENERATE_JSONNAME("obj_locallization_json_circles", imageCounter);
	file_name = image_path.string();

	bool f = cv::imwrite(image_path.string(), image);
	WriteCirclesInJSON(json_path.string(), stats);
}
//--------------------------------------------------------------------------------
void GenerateObjLocalizationImage(ErrorParams params, CirclesStatistics& stats,
	CirclesStatistics& error_stats, std::filesystem::path file_path,
	std::string& file_name, int& imageCounter)
{
	cv::Mat image(stats.image_width, stats.image_height, CV_32F, cv::Scalar(0, 0, 0));
	cv::Scalar color(255, 255, 255);

	for (int i = 0; i < stats.circles.size(); ++i) {


		cv::Point center(stats.circles[i].x + params.obj_localization, stats.circles[i].y);
		cv::circle(image, center, stats.circles[i].r, color, -1);

		CircleInfo circle;
		circle.x = stats.circles[i].x + params.obj_localization;
		circle.y = stats.circles[i].y;
		circle.r = stats.circles[i].r;

		error_stats.circles.push_back(circle);

	}

	error_stats.image_width = stats.image_width;
	error_stats.image_height = stats.image_height;

	auto image_path = file_path / GENERATE_FILENAME("obj_locallization_image_circles", imageCounter);
	auto json_path = file_path / GENERATE_JSONNAME("obj_locallization_json_circles", imageCounter);
	file_name = image_path.string();

	bool f = cv::imwrite(image_path.string(), image);
	WriteCirclesInJSON(json_path.string(), stats);
}