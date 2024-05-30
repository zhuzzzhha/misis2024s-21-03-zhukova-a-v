#pragma once
#include "GroundTruthGenerator.h"
#include <random>

//--------------------------------------------------------------------------------
struct CircleInfo;
//--------------------------------------------------------------------------------
enum class ErrorType
{
	DILATION,
	EROSION,
	OBJ_LOCALIZATION,
	BNDRY_APPROXIMATION,
	BNDRY_LOCALIZATION,
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
void addNoise(Polygon& polygon, int noiseLevel) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist(-noiseLevel, noiseLevel);

	for (cv::Point& point : polygon.points) {
		point.x += dist(gen);
		point.y += dist(gen);
	}
}
//--------------------------------------------------------------------------------
void addComplexity(Polygon& polygon, int numNewPoints) {
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<double> dist(0.0, 1.0);

	for (int i = 0; i < numNewPoints; ++i) {
		int index = dist(gen) * (polygon.points.size() - 1);
		cv::Point newPoint((polygon.points[index].x + polygon.points[index + 1].x) / 2, (polygon.points[index].y + polygon.points[index + 1].y) / 2);
		polygon.points.insert(polygon.points.begin() + index + 1, newPoint);
	}
}
//--------------------------------------------------------------------------------
void GenerateDilationJSON(ErrorParams params, JSONData& ground_truth_data,
	JSONData& error_data, std::filesystem::path file_path,
	std::string& file_name, int& imageCounter)
{
	json data;
	data["filename"] = ground_truth_data.filename;
	data["regions"] = json::array();
	error_data.filename = ground_truth_data.filename;



	for (auto& polygon : ground_truth_data.polygons)
	{
		json region1;
		std::vector<cv::Point> points;

		Polygon erosion_polygon;
		cv::approxPolyDP(polygon.points, erosion_polygon.points, params.scale_erosion, true); // Применяем алгоритм декимации

		for (auto& point : erosion_polygon.points)
		{
			points.push_back(cv::Point(point.x, 0));
			region1["shape_attributes"]["all_points_x"].push_back(point.x);
		}
		for (int j = 0; j < erosion_polygon.points.size(); ++j)
		{
			points[j].y = erosion_polygon.points[j].y;
			region1["shape_attributes"]["all_points_y"].push_back(erosion_polygon.points[j].y);
		}
		data["regions"].push_back(region1);
		error_data.polygons.push_back(erosion_polygon);
	}

	ground_truth_data.image_width = ground_truth_data.image_width;
	ground_truth_data.image_height = ground_truth_data.image_height;

	auto json_path = file_path / GENERATE_JSONNAME("obj_locallization_json_circles", imageCounter);
	std::ofstream outputFile(json_path);
	outputFile << std::setw(4) << data << std::endl;

	outputFile.close();
}
//--------------------------------------------------------------------------------
void GenerateBoundaryApproxJSON(ErrorParams params, JSONData& ground_truth_data,
	JSONData& error_data, std::filesystem::path file_path,
	std::string& file_name, int& imageCounter)
{
	json data;
	data["filename"] = ground_truth_data.filename;
	data["regions"] = json::array();
	error_data.filename = ground_truth_data.filename;

	for (auto& polygon : ground_truth_data.polygons)
	{
		json region1;
		std::vector<cv::Point> points;

		Polygon approx_polygon;
		cv::approxPolyDP(polygon.points, approx_polygon.points, params.boundary_approximation, true); // Применяем алгоритм декимации

		for (auto& point : approx_polygon.points)
		{
			points.push_back(cv::Point(point.x, 0));
			region1["shape_attributes"]["all_points_x"].push_back(point.x);
		}
		for (int j = 0; j < approx_polygon.points.size(); ++j)
		{
			points[j].y = approx_polygon.points[j].y;
			region1["shape_attributes"]["all_points_y"].push_back(approx_polygon.points[j].y);
		}
		data["regions"].push_back(region1);
		error_data.polygons.push_back(approx_polygon);
	}

	ground_truth_data.image_width = ground_truth_data.image_width;
	ground_truth_data.image_height = ground_truth_data.image_height;

	auto json_path = file_path / GENERATE_JSONNAME("boundary_approx", imageCounter);
	std::ofstream outputFile(json_path);
	outputFile << std::setw(4) << data << std::endl;

	outputFile.close();
}
//--------------------------------------------------------------------------------
void GenerateObjLocalizationJSON(ErrorParams params, JSONData& ground_truth_data,
	JSONData& error_data, std::filesystem::path file_path,
	std::string& file_name, int& imageCounter)
{
	json data;
	data["filename"] = ground_truth_data.filename;
	data["regions"] = json::array();
	error_data.filename = ground_truth_data.filename;
	

	for (auto& polygon : ground_truth_data.polygons)
	{
		json region1;
		std::vector<cv::Point> points;
		Polygon error_polygon;
		for (auto& point : polygon.points)
		{
			points.push_back(cv::Point(point.x + params.obj_localization, 0));
			region1["shape_attributes"]["all_points_x"].push_back(point.x + params.obj_localization);
			error_polygon.points.push_back(cv::Point(point.x + params.obj_localization, 0));
		}
		for (int j = 0; j < polygon.points.size(); ++j)
		{
			points[j].y = polygon.points[j].y;
			region1["shape_attributes"]["all_points_y"].push_back(polygon.points[j].y);
			error_polygon.points[j].y = polygon.points[j].y;
		}
		data["regions"].push_back(region1);
		error_data.polygons.push_back(error_polygon);
	}

	ground_truth_data.image_width = ground_truth_data.image_width;
	ground_truth_data.image_height = ground_truth_data.image_height;

	auto json_path = file_path / GENERATE_JSONNAME("obj_locallization_", imageCounter);
	std::ofstream outputFile(json_path);
	outputFile << std::setw(4) << data << std::endl;

	outputFile.close();

}
//--------------------------------------------------------------------------------
void GenerateBoundaryLocalJSON(ErrorParams params, JSONData& ground_truth_data,
	JSONData& error_data, std::filesystem::path file_path,
	std::string& file_name, int& imageCounter)
{
	json data;
	data["filename"] = ground_truth_data.filename;
	data["regions"] = json::array();
	error_data.filename = ground_truth_data.filename;


	for (auto& polygon : ground_truth_data.polygons)
	{
		json region1;
		std::vector<cv::Point> points;
		addNoise(polygon, params.boundary_localization);

		// Добавление новых точек для усложнения контура
		addComplexity(polygon, polygon.points.size());
		data["regions"].push_back(region1);
		error_data.polygons.push_back(polygon);

		for (auto& point : polygon.points)
		{
			region1["shape_attributes"]["all_points_x"].push_back(point.x);
			region1["shape_attributes"]["all_points_y"].push_back(point.y);
		}

	}

	ground_truth_data.image_width = ground_truth_data.image_width;
	ground_truth_data.image_height = ground_truth_data.image_height;

	auto json_path = file_path / GENERATE_JSONNAME("obj_locallization_", imageCounter);
	std::ofstream outputFile(json_path);
	outputFile << std::setw(4) << data << std::endl;

	outputFile.close();

}
//--------------------------------------------------------------------------------
void GenerateErosionFromImage(ErrorParams params, cv::Mat& ground_truth_img,
	cv::Mat& eroded_img,
	std::filesystem::path file_path,
	std::string& file_name, int& imageCounter)
{
	int kernelSize = params.scale_erosion;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,
		cv::Size(kernelSize, kernelSize));

	// Применение эрозии
	cv::erode(ground_truth_img, eroded_img, kernel,cv::Point(-1,-1), 3);

	stringstream ss;
	ss << fixed << setprecision(1) << params.scale_erosion;

	std::string img_param = ss.str();

	cv::imwrite(file_path.string() + "\\" + "erosion" + img_param + ".png", eroded_img);
}
//--------------------------------------------------------------------------------
void GenerateDilationFromImage(ErrorParams params, cv::Mat& ground_truth_img,
	cv::Mat& dilate_img,
	std::filesystem::path file_path,
	std::string& file_name, int& imageCounter)
{
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * params.scale_dilation + 1, 2 * params.scale_dilation + 1));
	cv::dilate(ground_truth_img, dilate_img, element);

	stringstream ss;
	ss << fixed << setprecision(1) << params.scale_dilation;

	std::string img_param = ss.str();

	cv::imwrite(file_path.string() + "\\" + "dilation" + img_param + ".png", dilate_img);
}
//--------------------------------------------------------------------------------
void ValidateErrorImage(JSONData& data, std::string& error_type, std::filesystem::path output_dir, cv::Mat& error_image, cv::Mat& ground_truth,  double image_params)
{
	error_image.convertTo(error_image, CV_8UC3);

	cv::imshow("Ground truth", ground_truth);
	cv::imshow("Error", error_image);

	cv::Mat result_image = cv::Mat::zeros(ground_truth.size(), CV_8UC3);

	// Зеленый канал для ground_truth, красный для error_image
	for (int i = 0; i < ground_truth.rows; ++i) {
		for (int j = 0; j < ground_truth.cols; ++j) {
			if(ground_truth.at<uchar>(i, j) == 255 || error_image.at<uchar>(i, j) == 255)
				result_image.at<cv::Vec3b>(i, j) = cv::Vec3b(0, ground_truth.at<uchar>(i, j), error_image.at<uchar>(i, j)); // Зеленый цвет

		}
	}
	for (auto polygon : data.polygons) {
		polylines(result_image, polygon.points, true, cv::Scalar(255, 255, 255), 1);
	}
	
	imshow("Result", result_image);
	cv::imwrite("result.png", result_image);

	//cv::bitwise_not(ground_truth, ground_truth);
	//cv::bitwise_not(error_image, error_image);
	ContingencyTable contigency_table(ground_truth, error_image, false, 0);

	validateImage(ground_truth, error_image, output_dir, error_type, false, 0, image_params);
}
//--------------------------------------------------------------------------------