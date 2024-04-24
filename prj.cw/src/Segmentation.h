
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
#include <string>
#include <sstream>


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

	//--------------------------------------------------------------------------------

	metricId = DICE;
	DiceCoefficientMetric* diceCoefficient = new DiceCoefficientMetric(contingenceTable, fuzzy, threshold);
	value = diceCoefficient->CalcDiceCoeff();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = JACRD;
	JaccardCoefficientMetric* jaccardCoefficient = new JaccardCoefficientMetric(contingenceTable, fuzzy, threshold);
	value = jaccardCoefficient->CalcJaccardCoeff();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

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

	std::cout << std::endl;
	//--------------------------------------------------------------------------------

	metricId = GCOERR;
	GlobalConsistencyError* globalConsistencyError = new GlobalConsistencyError(contingenceTable, fuzzy, threshold);
	value = globalConsistencyError->CalcGlobalConsistencyError();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = RNDIND;
	RandIndexMetric* randIndex = new RandIndexMetric(contingenceTable, fuzzy, threshold);
	value = randIndex->CalcRandIndex();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = ADJRIND;
	value = randIndex->CalcAdjustedRandIndex();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = KAPPA;
	CohinKappaMetric* cohinKappa = new CohinKappaMetric(contingenceTable, fuzzy, threshold);
	value = cohinKappa->CalcCohenKappa();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------

	metricId = AUC;
	value = classicMeasures->CalcAUC();
	pushValue(metricId, value);

	//--------------------------------------------------------------------------------



}
#endif