#ifndef _COHINKAPPA
#define _COHINKAPPA

#include "ContigencyTable.h"
class ContingencyTable;

class CohinKappaMetric
{
private:
	ContingencyTable* contingencyTable;
	bool fuzzy;
	double threshold;
public:

	~CohinKappaMetric() {

	}

	CohinKappaMetric(ContingencyTable* contingencyTable, bool fuzzy, double threshold) {
		this->contingencyTable = contingencyTable;
		this->fuzzy = fuzzy;
		this->threshold = threshold;
	}

	double CalcCohenKappa() {
		double tp = contingencyTable->tp;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tn = contingencyTable->tn;

		double agreement = tp + tn;
		double chance_0 = (tn + fn) * (tn + fp);
		double chance_1 = (fp + tp) * (fn + tp);
		double chance = chance_0 + chance_1;
		double sum = (tn + fn + fp + tp);
		chance = chance / sum;
		double kappa = (agreement - chance) / (sum - chance);
		return kappa;
	}

};
#endif