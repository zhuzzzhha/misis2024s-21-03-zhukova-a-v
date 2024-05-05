#include <iostream>
#include "ground_truth_generator.h"
#include "ContigencyTable.h"
#include "segmentation.h"

int main()
{
	std::filesystem::path path_to_image = std::filesystem::current_path();
	initMetricInfo();

	//генерируем ground truth
	CirclesStatistics gr_truth_stats;
	std::string ground_truth_filename = "gr_truth_image";
	std::string segmented_filename = "segmented_image";

	int imageCounter = 0;

	std::ofstream outFile(FILES_ORDER_NAME);
	if (outFile.is_open()) {
		outFile << imageCounter;	
		outFile.close();
	}
	else {
		std::cerr << "Unable to open file for writing." << std::endl;
	}

	
	std::filesystem::path path_to_fixed = path_to_image / ground_truth_filename;

	cv::Mat src_image = generateImageWithCircles(gr_truth_stats, 10, 5, 25, 20, 255, 7, 2, imageCounter);
	cv::Mat segmented_image(src_image.size(), src_image.type());

	int radius = 5;
	int diff = 100;

	MeanShiftSegmentation(src_image, segmented_image, radius, diff);

	ContingencyTable contigency_table(src_image, segmented_image, false, 0);

	validateImage(src_image, segmented_image, false, 0);

	cv::imwrite(GENERATE_FILENAME(segmented_filename, imageCounter), segmented_image);
}