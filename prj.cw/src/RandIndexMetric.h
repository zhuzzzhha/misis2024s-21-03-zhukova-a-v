#ifndef _RAND_INDEX_METRIC
#define _RAND_INDEX_METRIC

class ContingencyTable;
class RandIndexMetric
{

private:
  ContingencyTable *contingencyTable;
  bool fuzzy;
  double threshold;
	double a;
	double b;
	double c;
	double d;

public: 
   	
	~RandIndexMetric(){

	}

	RandIndexMetric(ContingencyTable *contingencyTable, bool fuzzy, double threshold){
		this->contingencyTable= contingencyTable;
		this->fuzzy = fuzzy;
        this->threshold = threshold;

		this->a = contingencyTable->a;
		this->b = contingencyTable->b;
		this->c = contingencyTable->c;
		this->d = contingencyTable->d;

	}


	double CalcRandIndex(){
		

		return (a + d)/(a + b + c + d);

	}

	double CalcAdjustedRandIndex(){

		double x1 = a - ((a+c)*(a+b)/(a+b+c+d));
		double x2 = ( (a+c) + (a+b))/2.0;
		double x3 = ( (a+c)*(a+b))/(a+b+c+d);

		if(x2!=x3)
			return x1/(x2-x3);
		else
			return 0;
	}

};
#endif