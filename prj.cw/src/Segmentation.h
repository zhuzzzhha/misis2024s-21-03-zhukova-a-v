
// Description:
//
// This file is responsible for evaluating two segmentations. It links the suitable evaluation 
// algorithm to perform the evaluation and then it links the program part responsible for 
// displaying the values or writting them in an xml file according to the set options.
//

#ifndef _SEGMENTATION
#define _SEGMENTATION 

#include <exception>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>


#include "MetricConstants.h"
#include "ImageStatistics.h"
#include "Outputter.h"
#include "DiceCoefficient.h"
#include "CohinKappaMetric.h"
#include "GlobalConsistencyError.h"
#include "JaccardCoefficientMetric.h"
#include "RandIndexMetric.h"
#include "ClassicMeasures.h"


using namespace std;
using json = nlohmann::json;

#define GENERATE_FILENAME(name, num) (name + to_string(num) + ".png")
#define GENERATE_JSONNAME(name, num) (name +"_" + num + ".json")

static void validateImage(const cv::Mat& fixedImage, const cv::Mat& movingImage, std::filesystem::path output_dir, std::string& error_type, bool fuzzy, double threshold, double image_param)
{

	ContingencyTable* contingenceTable = new ContingencyTable(fixedImage, movingImage, fuzzy, threshold);
	
	stringstream ss;
	ss << fixed << setprecision(1) << image_param;

	std::string img_param = ss.str();
	std::string json_path = output_dir.string() +"\\" + GENERATE_JSONNAME(error_type, img_param);
	std::ofstream json_file(json_path);
	json metrics_json;	

	double value = 0;
	//--------------------------------------------------------------------------------
	MetricId metricId = TP;
	ClassicMeasures* classicMeasures = new ClassicMeasures(contingenceTable, fuzzy, threshold);
	value = classicMeasures->getTP();
	pushValue(metricId, value);
	// Добавление новой метрики
	std::string metric_name = "TP";
	double metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	// Запись обновленного JSON в файл
	std::ofstream updated_json_file(json_path);


	//--------------------------------------------------------------------------------

	metricId = FP;
	value = classicMeasures->getFP();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "FP";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;


	//--------------------------------------------------------------------------------

	metricId = TN;
	value = classicMeasures->getTN();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "TN";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	//--------------------------------------------------------------------------------

	metricId = FN;
	value = classicMeasures->getFN();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "FN";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	//--------------------------------------------------------------------------------

	std::cout << std::endl;

	//--------------------------------------------------------------------------------

	metricId = DICE;
	DiceCoefficientMetric* diceCoefficient = new DiceCoefficientMetric(contingenceTable, fuzzy, threshold);
	value = diceCoefficient->CalcDiceCoeff();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "DICE";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	//--------------------------------------------------------------------------------

	metricId = JACRD;
	JaccardCoefficientMetric* jaccardCoefficient = new JaccardCoefficientMetric(contingenceTable, fuzzy, threshold);
	value = jaccardCoefficient->CalcJaccardCoeff();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "JACCARD";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	//--------------------------------------------------------------------------------

	metricId = SNSVTY;
	value = classicMeasures->CalcSensitivity();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "SENSITIVITY";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	//--------------------------------------------------------------------------------

	metricId = SPCFTY;
	value = classicMeasures->CalcSpecificity();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "SPECIFICITY";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	//--------------------------------------------------------------------------------

	metricId = FALLOUT;
	value = classicMeasures->CalcFallout();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "FALLOUT";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	//--------------------------------------------------------------------------------

	metricId = PRCISON;
	value = classicMeasures->CalcPrecision();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "PRECISION";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;
	//--------------------------------------------------------------------------------

	metricId = ACURCY;
	value = classicMeasures->CalcAccuracy();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "ACCURACY";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	//--------------------------------------------------------------------------------

	std::cout << std::endl;
	//--------------------------------------------------------------------------------

	metricId = GCOERR;
	GlobalConsistencyError* globalConsistencyError = new GlobalConsistencyError(contingenceTable, fuzzy, threshold);
	value = globalConsistencyError->CalcGlobalConsistencyError();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "GCOERROR";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	//--------------------------------------------------------------------------------

	metricId = RNDIND;
	RandIndexMetric* randIndex = new RandIndexMetric(contingenceTable, fuzzy, threshold);
	value = randIndex->CalcRandIndex();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "RAND INDEX";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	//--------------------------------------------------------------------------------

	metricId = ADJRIND;
	value = randIndex->CalcAdjustedRandIndex();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "ADJUST RAND";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	//--------------------------------------------------------------------------------

	metricId = KAPPA;
	CohinKappaMetric* cohinKappa = new CohinKappaMetric(contingenceTable, fuzzy, threshold);
	value = cohinKappa->CalcCohenKappa();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "COHEN KAPPA";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;
	
	//--------------------------------------------------------------------------------

	metricId = AUC;
	value = classicMeasures->CalcAUC();
	pushValue(metricId, value);
	// Добавление новой метрики
	metric_name = "AUC";
	metric_value = value;
	metrics_json["metrics"][metric_name] = metric_value;

	// Запись обновленного JSON в файл
	updated_json_file << metrics_json.dump(4);  // 4 - количество отступов для удобства чтения
	updated_json_file.close();

	//--------------------------------------------------------------------------------



}
#endif