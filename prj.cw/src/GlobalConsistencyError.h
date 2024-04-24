#ifndef _GLOBALCONSISTENCYERROR
#define _GLOBALCONSISTENCYERROR
#include "ContigencyTable.h"

class ContingencyTable;

class GlobalConsistencyError
{

private:
	ContingencyTable* contingencyTable;
	bool fuzzy;
	double threshold;
public:

	~GlobalConsistencyError() {

	}

	GlobalConsistencyError(ContingencyTable* contingencyTable, bool fuzzy, double threshold) {
		this->contingencyTable = contingencyTable;
		this->fuzzy = fuzzy;
		this->threshold = threshold;
	}

	double CalcGlobalConsistencyError() {
		double tn = contingencyTable->tn;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tp = contingencyTable->tp;
		double num1 = contingencyTable->numberElements_f;
		double num2 = contingencyTable->numberElements_m;

		double n = tn + fp + fn + tp;
		double e1 = (fn * (fn + 2 * tp) / (tp + fn) + fp * (fp + 2 * tn) / (tn + fp)) / n;
		double e2 = (fp * (fp + 2 * tp) / (tp + fp) + fn * (fn + 2 * tn) / (tn + fn)) / n;

		return std::min(e1, e2);
	}


};
#endif