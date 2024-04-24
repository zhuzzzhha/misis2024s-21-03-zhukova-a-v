
// Description:
//
// This class builds a contingency table holding statistic and information about the
// compared images like overlapping voxels and the parameter a,b,c,d, etc. which are
// used by evaluation algorithm.
//

#ifndef _CONTINGENCYTABLE
#define _CONTINGENCYTABLE
//
//#include "itkImage.h"
//#include "itkImageRegionIterator.h"
#include <algorithm>
#include "ImageStatistics.h"

class ImageStatistics;
class ContingencyTable
{


private:
	ImageStatistics* imageStat;

public:
	int numberElements_f;
	int numberElements_m;
	double tn; //TN
	double fn; //FN
	double fp; //FP
	double tp; //TP
	double a;
	double b;
	double c;
	double d;
	double n;
	~ContingencyTable() {

	}

	ContingencyTable(const cv::Mat& fixedImage, const cv::Mat& movingImage, bool fuzzy, double threshold) {
	
		double sum_f = 0;
		double sum_m = 0;
		tn = 0;
		fp = 0;
		fn = 0;
		tp = 0;
		a = 0;
		b = 0;
		c = 0;
		d = 0;
		for (int y = 0; y < movingImage.rows; ++y) {
			for (int x = 0; x < movingImage.cols; ++x) {
					tp += (movingImage.at<uchar>(y, x) && fixedImage.at<uchar>(y, x)) ? 1 : 0;
					tn += (!movingImage.at<uchar>(y, x) && !fixedImage.at<uchar>(y, x)) ? 1 : 0;
					fp += (movingImage.at<uchar>(y, x) && !fixedImage.at<uchar>(y, x)) ? 1 : 0;
					fn+= (!movingImage.at<uchar>(y, x) && fixedImage.at<uchar>(y, x)) ? 1 : 0;
			}
		}

		//-------------------
		n = std::min(numberElements_f, numberElements_m);
		double coltot1 = tn + fp;
		double coltot2 = fn + tp;
		double rowtot1 = tn + fn;
		double rowtot2 = fp + tp;
		double nis = rowtot1 * rowtot1 + rowtot2 * rowtot2;
		double njs = coltot1 * coltot1 + coltot2 * coltot2;
		double s = tp * tp + tn * tn + fp * fp + fn * fn;
		a = (binomial(tn) + binomial(fn) + binomial(fp) + binomial(tp)) / 2.0;
		b = (njs - s) / 2.0;
		c = (nis - s) / 2.0;
		d = (n * n + s - nis - njs) / 2.0;
		double t1 = (n * (n - 1)) / 2.0;
	}

	static long double binomial(double val) {
		return  val * (val - 1);
	}

};

#endif