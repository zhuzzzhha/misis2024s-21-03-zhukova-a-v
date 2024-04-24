#ifndef _JCCRD_COEFF_METRIC
#define _JCCRD_COEFF_METRIC

#include "ContigencyTable.h"
class ContingencyTable;

class JaccardCoefficientMetric
{

private:
	ContingencyTable* contingencyTable;
	bool fuzzy;
	double threshold;
public:

	~JaccardCoefficientMetric() {

	}

	JaccardCoefficientMetric(ContingencyTable* contingencyTable, bool fuzzy, double threshold) {
		this->contingencyTable = contingencyTable;
		this->fuzzy = fuzzy;
		this->threshold = threshold;
	}

	double CalcJaccardCoeff() {
		double tp = contingencyTable->tp;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tn = contingencyTable->tn;

		return tp / (tp + fp + fn);

	}

};
#endif