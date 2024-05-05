
// Description:
//
// This class counts some statistics like number of pixels etc.
//

#ifndef _IMAGESTATISTICS
#define _IMAGESTATISTICS
#include <opencv2/opencv.hpp>
//#include "itkImage.h"

#define PIXEL_VALUE_RANGE_MAX 255
#define PIXEL_VALUE_RANGE_MIN 0
class ImageStatistics
{
public:
	int numberElements_f;
	int numberElements_m;
	int num_nonzero_points_f;
	int num_nonzero_points_m;
	int num_intersection;
	int max_f;
	int max_m;

	~ImageStatistics() {

	}

	ImageStatistics(cv::Mat& fixedImage, cv::Mat& movingImage, bool fuzzy, double threshold) {

		double thd = 0;
		if (!fuzzy && threshold != -1) {
			thd = threshold * PIXEL_VALUE_RANGE_MAX;
		}

		numberElements_f = 0;
		num_nonzero_points_f = 0;

		max_f = 0;
		max_m = 0;

		for (int y = 0; y < fixedImage.rows; ++y) {
			for (int x = 0; x < fixedImage.cols; ++x) {
				if (fixedImage.at<uchar>(y, x) > thd) {
					num_nonzero_points_f++;
				}
				if (fixedImage.at<uchar>(y, x) > max_f)
					max_f = fixedImage.at<uchar>(y, x);
			}
		}
		

		numberElements_m = 0;
		num_nonzero_points_m = 0;
		for (int y = 0; y < movingImage.rows; ++y) {
			for (int x = 0; x < movingImage.cols; ++x) {
				if (movingImage.at<uchar>(y, x) > thd) {
					num_nonzero_points_m++;
				}
				if (movingImage.at<uchar>(y, x) > max_m)
					max_m = movingImage.at<uchar>(y, x);
			}
		}

		for (int y = 0; y < movingImage.rows; ++y) {
			for (int x = 0; x < movingImage.cols; ++x) {
				if (movingImage.at<uchar>(y, x) > thd && fixedImage.at<uchar>(y, x) > thd) {
					num_intersection++;
				}
			}
		}
		
	}

};

#endif