
#include "DiceCoefficient.h"
#include "ContigencyTable.h"
#include "ImageStatistics.h"
#include "ImageReader.h"
#include "MetricConstants.h"
#include "ClassicMeasures.h"
#include "Segmentation.h"
#include "Outputter.h"
#include "CohinKappaMetric.h"
#include "GroundTruthGenerator.h"
#include "ErrorImageGenerator.h"
#include <map>
#include <functional>

using ErrorFunction = std::function<void(ErrorParams, CirclesStatistics&, CirclesStatistics&,
	std::filesystem::path, std::string&, int&)>;

std::unordered_map<ErrorType, ErrorFunction> types_of_errors = { { ErrorType::DILATION, &GenerateDilationImage },
{ ErrorType::EROSION, &GenerateErosionImage }, { ErrorType::OBJ_LOCALIZATION,&GenerateObjLocalizationImage } };


int main()
{
	//инициализируем метрики
	initMetricInfo();
	std::filesystem::path path_to_image = std::filesystem::current_path();

	//генерируем ground truth
	CirclesStatistics gr_truth_stats;
	std::string ground_truth_filename;

	int imageCounter = 0;

	std::ofstream outFile(FILES_ORDER_NAME);
	if (outFile.is_open()) {
		outFile << imageCounter;
		outFile.close();
	}
	else {
		std::cerr << "Unable to open file for writing." << std::endl;
	}

	generateImageWithCircles(gr_truth_stats, 10, 5, 25, path_to_image, ground_truth_filename, imageCounter);
	std::filesystem::path path_to_fixed = path_to_image / ground_truth_filename;

	//генерируем ошибочные сегментации
	while (1) {
		std::cout << "¬ведите тип ошибки: \n"
			<< "0: DILATION\n"
			<< "1: EROSION\n"
			<< "2: OBJ_LOCALIZATION\n";

		CirclesStatistics error_stats;
		std::string error_image_filename;
		ErrorParams params = { 0 };

		int error = 1;
		std::cin >> error;
		switch (static_cast<ErrorType>(error)) {
		case ErrorType::DILATION:
		{
			std::cout << "Enter dilation coefficient\n";
			std::cin >> params.scale_dilation;
			types_of_errors[static_cast<ErrorType>(error)](params, gr_truth_stats, error_stats, path_to_image, error_image_filename, imageCounter);
		} break;
		case ErrorType::EROSION:
		{
			std::cout << "Enter erosion coefficient\n";
			std::cin >> params.scale_erosion;
			types_of_errors[static_cast<ErrorType>(error)](params, gr_truth_stats, error_stats, path_to_image, error_image_filename, imageCounter);
		}break;
		case ErrorType::OBJ_LOCALIZATION:
		{
			std::cout << "Enter bias\n";
			std::cin >> params.obj_localization;
			types_of_errors[static_cast<ErrorType>(error)](params, gr_truth_stats, error_stats, path_to_image, error_image_filename, imageCounter);
		}break;
		default:
			break;
		}

		//сравниваем, выводим значение метрик
		++imageCounter;
		std::ofstream f(FILES_ORDER_NAME);
		if (f.is_open())
			f << imageCounter << '\n';
		f.close();
		std::filesystem::path path_to_moved = path_to_image / error_image_filename;

		ImageReader fixed_proc(path_to_fixed);
		ImageReader moved_proc(path_to_moved);
		ContingencyTable contigency_table(fixed_proc.GetImage(), moved_proc.GetImage(), false, 0);

		validateImage(fixed_proc.GetImage(), moved_proc.GetImage(), false, 0);

		cv::Mat diff_img = DrawDifference(gr_truth_stats, error_stats);
		cv::imshow("Difference", diff_img);
		cv::waitKey(0);
	}


	

	

	
}