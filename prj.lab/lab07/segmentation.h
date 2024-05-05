#pragma once

#include <opencv2/opencv.hpp>

#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#include "MetricConstants.h"
#include "ImageStatistics.h"
#include "Outputter.h"
#include "ClassicMeasures.h"


using namespace std;

void MeanShiftSegmentation(cv::Mat& src_image, cv::Mat& segmented_image, int radius, int min_diff)
{
	cv::pyrMeanShiftFiltering(src_image, segmented_image, radius, min_diff);
	cv::imshow("Initial Image", src_image);
	cv::imshow("Segmented Image", segmented_image);
	cv::waitKey(0);
}



static void validateImage(const cv::Mat& fixedImage, const cv::Mat& movingImage, bool fuzzy, double threshold)
{

	ContingencyTable* contingenceTable = new ContingencyTable(fixedImage, movingImage, fuzzy, threshold);


	double value = 0;
	//--------------------------------------------------------------------------------
	MetricId metricId = TP;
	ClassicMeasures* classicMeasures = new ClassicMeasures(contingenceTable, fuzzy, threshold);
	value = classicMeasures->getTP();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = FP;
	value = classicMeasures->getFP();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = TN;
	value = classicMeasures->getTN();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = FN;
	value = classicMeasures->getFN();
	pushValue(metricId, value);
	//--------------------------------------------------------------------------------

	std::cout << std::endl;


	metricId = SNSVTY;
	value = classicMeasures->CalcSensitivity();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = SPCFTY;
	value = classicMeasures->CalcSpecificity();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = FALLOUT;
	value = classicMeasures->CalcFallout();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = PRCISON;
	value = classicMeasures->CalcPrecision();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = ACURCY;
	value = classicMeasures->CalcAccuracy();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = AUC;
	value = classicMeasures->CalcAUC();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------



}
