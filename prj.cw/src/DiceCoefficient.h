#ifndef _DICECOEFFICIENT
#define _DICECOEFFICIENT

#include "ContigencyTable.h"
class ContingencyTable;

class DiceCoefficientMetric
{
private:
  ContingencyTable* contingencyTable;
  bool fuzzy;
  double threshold;
public: 
   	
	~DiceCoefficientMetric(){

	}

	DiceCoefficientMetric(ContingencyTable *contingencyTable, bool fuzzy, double threshold){
		this->contingencyTable= contingencyTable;
		this->fuzzy = fuzzy;
        this->threshold = threshold;
	}


	double CalcDiceCoeff(){
		double tp = contingencyTable->tp;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tn = contingencyTable->tn;

		return 2*tp/(2*tp + fp + fn);

	}

};
#endif