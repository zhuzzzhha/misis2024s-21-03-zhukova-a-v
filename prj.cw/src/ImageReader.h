
// Description:
//
// This class performs some preprocessing on the volumes to be compared such as counting
// the voxels and saving them in the memory. The class is used from almost all evaluation algorithms.
//
//f - ground truth, m - data

#ifndef _IMAGEREADER
#define _IMAGEREADER

#include <opencv2/opencv.hpp>
#include <filesystem>
//#include "itkImage.h"
//#include "ImageStatistics.h"

class ImageReader
{
private:
	cv::Mat image;

	public:
		ImageReader(std::filesystem::path path_to_image)
		{
			if (std::filesystem::exists(path_to_image))
			{
				image = cv::imread(path_to_image.string());
				image.convertTo(image,CV_8UC1);
			}
		}
		cv::Mat& GetImage() {
			return image;
		}
};

#endif