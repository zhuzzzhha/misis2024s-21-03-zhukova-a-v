#ifndef _CLASSICMEASURES
#define _CLASSICMEASURES
class ClassicMeasures
{

private:
	ContingencyTable* contingencyTable;
	bool fuzzy;
	double threshold;
public:

	~ClassicMeasures() {

	}

	ClassicMeasures(ContingencyTable* contingencyTable, bool fuzzy, double threshold) {
		this->contingencyTable = contingencyTable;
		this->fuzzy = fuzzy;
		this->threshold = threshold;
	}

	double getTP() {
		return contingencyTable->tp;
	}

	double getTN() {
		return contingencyTable->tn;
	}

	double getFP() {
		return contingencyTable->fp;
	}

	double getFN() {
		return contingencyTable->fn;
	}


	double CalcSensitivity() {
		double tp = contingencyTable->tp;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tn = contingencyTable->tn;
		return tp / (tp + fn);
	}

	double CalcSpecificity() {
		double tp = contingencyTable->tp;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tn = contingencyTable->tn;
		return tn / (tn + fp);
	}


	double CalcFallout() {
		double tp = contingencyTable->tp;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tn = contingencyTable->tn;
		return 1 - CalcSpecificity(); //false positive rate (fallout = 1 - specificity) 
	}


	double CalcAUC() {
		double tp = contingencyTable->tp;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tn = contingencyTable->tn;
		double tpr = CalcSensitivity(); //true positive rate (sensitivity)
		double fpr = CalcFallout(); //false positive rate (fallout)
		return (tpr - fpr + 1) / 2;
	}

	double CalcPrecision() {
		double tp = contingencyTable->tp;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tn = contingencyTable->tn;
		if ((tp + fp) != 0) {
			return tp / (tp + fp);
		}
		return 0;

	}


	double CalcRecall() {
		double tp = contingencyTable->tp;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tn = contingencyTable->tn;
		if ((tp + fn) != 0) {
			return tp / (tp + fn);
		}
		return 0;
	}

	double CalcFMeasure(double beta) {
		double tp = contingencyTable->tp;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tn = contingencyTable->tn;
		double beta_s = beta * beta;
		double precision = CalcPrecision();
		double recall = CalcRecall();
		if ((beta_s * precision + recall) != 0) {
			return (1 + beta_s) * ((precision * recall) / (beta_s * precision + recall));
		}
		return 0;

	}

	double CalcAccuracy() {
		double tp = contingencyTable->tp;
		double fp = contingencyTable->fp;
		double fn = contingencyTable->fn;
		double tn = contingencyTable->tn;
		if ((tp + tn + fp + fn) != 0) {
			return (tp + tn) / (tp + tn + fp + fn);
		}
		return 0;

	}



};
#endif