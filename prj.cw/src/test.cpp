
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

using ErrorFunction = std::function<void(ErrorParams, JSONData&, JSONData&, std::filesystem::path,
	std::string&, int&)>;

std::unordered_map<ErrorType, ErrorFunction> types_of_errors = { /*{ ErrorType::DILATION, &GenerateDilationImage },*/
	/*{ ErrorType::EROSION, &GenerateErosionJSON },*/ { ErrorType::OBJ_LOCALIZATION,&GenerateObjLocalizationJSON },{ ErrorType::BNDRY_APPROXIMATION,&GenerateBoundaryApproxJSON},
		{ErrorType::BNDRY_LOCALIZATION, &GenerateBoundaryLocalJSON }};



int main(int argc, char* argv[])
{

		cv::CommandLineParser parser(argc, argv,	
			"{@input_dir         | | input directory}");

	std::string input_dir = parser.get<std::string>("@input_dir");

	std::filesystem::path output_metrics_dir = std::filesystem::current_path()/input_dir;
	std::filesystem::path output_images_dir = std::filesystem::current_path() / input_dir;

	if (std::filesystem::create_directory(output_metrics_dir / "metrics"))
		output_metrics_dir /= "metrics";

	//                      
		initMetricInfo();
	std::filesystem::path path_to_image = input_dir;

	//           ground truth
	CirclesStatistics gr_truth_stats;
	std::string ground_truth_filename;

	int imageCounter = 0;


	std::filesystem::path path_to_fixed = path_to_image / ground_truth_filename;

	std::filesystem::path path_to_json = "test.json";
	JSONData data;
	GetDataFromJSON(path_to_json, data);

	//                                
	while (1) {
		std::cout << "Enter error type: \n"
			<< "0: DILATION\n"
			<< "1: EROSION\n"
			<< "2: OBJ_LOCALIZATION\n"
			<< "3: BNDRY_APPROXIMATION\n"
			<< "4: BNDRY_LOCALIZATION\n";

		std::string error_image_filename;
		ErrorParams params = { 0 };
		JSONData error_data;

		//          ,                        
		++imageCounter;
		cv::Mat ground_truth = GenerateImageFromJSON(data);
		cv::imwrite(output_images_dir.string() + "\\" + "ground_truth.png", ground_truth);
		cv::Mat error_image;

		int error = 1;
		std::cin >> error;
		switch (static_cast<ErrorType>(error)) {
		case ErrorType::DILATION:
		{
			std::cout << "Enter dilation coefficient\n";
			std::cin >> params.scale_dilation;
			std::string error_type = "dilation";
			GenerateDilationFromImage(params, ground_truth, error_image, output_images_dir, error_image_filename, imageCounter);
			ValidateErrorImage(data, error_type, output_metrics_dir, error_image, ground_truth, params.scale_dilation);
		} break;
		case ErrorType::EROSION:
		{
			std::cout << "Enter erosion coefficient\n";
			std::cin >> params.scale_erosion;
			std::string error_type = "erosion";
			GenerateErosionFromImage(params, ground_truth, error_image, output_images_dir, error_image_filename, imageCounter);
			ValidateErrorImage(data, error_type, output_metrics_dir, error_image, ground_truth, params.scale_erosion);
		}break;
		case ErrorType::OBJ_LOCALIZATION:
		{
			std::cout << "Enter bias\n";
			std::cin >> params.obj_localization;
			types_of_errors[static_cast<ErrorType>(error)](params, data, error_data, input_dir, error_image_filename, imageCounter);
			error_image = GenerateImageFromJSON(error_data);
			std::string error_type = "obj_localization";

			stringstream ss;
			ss << fixed << setprecision(1) << params.obj_localization;

			std::string img_param = ss.str();

			cv::imwrite(output_images_dir.string() + "\\" + error_type + img_param + ".png", error_image);
			ValidateErrorImage(data, error_type, output_metrics_dir, error_image, ground_truth, params.obj_localization);
		}break;
		case ErrorType::BNDRY_APPROXIMATION:
		{
			std::cout << "Enter boundary approximation coefficient\n";
			std::cin >> params.boundary_approximation;
			types_of_errors[static_cast<ErrorType>(error)](params, data, error_data, input_dir, error_image_filename, imageCounter);
			error_image = GenerateImageFromJSON(error_data);
			std::string error_type = "obj_approximation";
			stringstream ss;
			ss << fixed << setprecision(1) << params.boundary_approximation;

			std::string img_param = ss.str();

			cv::imwrite(output_images_dir.string() + "\\" + error_type + img_param + ".png", error_image);
			ValidateErrorImage(data, error_type, output_metrics_dir, error_image, ground_truth, params.boundary_approximation);
		}break;
		case ErrorType::BNDRY_LOCALIZATION:
		{
			std::cout << "Enter boundary localization coefficient\n";
			std::cin >> params.boundary_localization;
			types_of_errors[static_cast<ErrorType>(error)](params, data, error_data, input_dir, error_image_filename, imageCounter);
			error_image = GenerateImageFromJSON(error_data);
			std::string error_type = "bndry_localization";
			stringstream ss;
			ss << fixed << setprecision(1) << params.boundary_approximation;

			std::string img_param = ss.str();
			cv::imwrite(output_images_dir.string() + "\\" + error_type + img_param + ".png", error_image);
			ValidateErrorImage(data, error_type, output_metrics_dir, error_image, ground_truth, params.boundary_localization);
		}break;
		default:
			break;
		}

		cv::waitKey(0);
	}


	

	

	
}
