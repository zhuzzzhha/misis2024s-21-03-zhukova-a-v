#include <opencv2/opencv.hpp>
#include <filesystem>
#include <vector>
using namespace cv;
using namespace std;
///////////////////////////////////////////////////////////////////////////////////////
cv::Mat makeImage(Scalar brightness_rect, Scalar brightness_circle, int height = 99, int radius = 25)
{
	Mat image = Mat::zeros(height, height, CV_32F);
	//image.setTo(brightness_rect);

	rectangle(image, Point(0, 0), Point(height, height), brightness_rect, -1);

	circle(image, Point(height / 2, height / 2), radius, brightness_circle, -1);

	return image;
}
///////////////////////////////////////////////////////////////////////////////////////
void makeHconcat(std::vector<Mat>& images)
{
	for (int i = 1; i < images.size(); i++)
	{
		cv::hconcat(images[0], images[i], images[0]);
	}
}
///////////////////////////////////////////////////////////////////////////////////////
void makeVconcat(std::vector<Mat>& images)
{
	for (int i = 1; i < images.size(); i++)
	{
		cv::vconcat(images[0], images[i], images[0]);
	}
}
///////////////////////////////////////////////////////////////////////////////////////
Mat applyConvolution(const Mat& image, const Mat& kernel) {
	Mat result;
	filter2D(image, result, -1, kernel);
	return result;
}
///////////////////////////////////////////////////////////////////////////////////////
Mat calculateSquare(const Mat& image_1, const Mat& image_2) {
	Mat squared_1, squared_2;
	cv::pow(image_1, 2, squared_1);
	cv::pow(image_2, 2, squared_2);

	Mat sum;
	cv::add(squared_1, squared_2, sum);

	Mat result;
	cv::sqrt(sum, result);

	return result;
}
///////////////////////////////////////////////////////////////////////////////////////
Mat mergeChannels(const Mat& channel1, const Mat& channel2, const Mat& channel3) {
	Mat result;
	vector<Mat> channels = { channel1, channel2, channel3 };
	cv::merge(channels, result);
	return result;
}
///////////////////////////////////////////////////////////////////////////////////////
int main()
{
	Scalar brightness_rect = Scalar(0, 0, 0);
	Scalar brightness_circle = Scalar(127, 127, 127);
	vector<Scalar> brightness = { Scalar(0, 0, 0), Scalar(127, 127, 127), Scalar(255, 255, 255) };

	Mat result_image;
	vector<Mat> images;

	/*for (int i = 0; i < brightness.size(); i++)
	{

		for (int j = i + 1; j < brightness.size(); j++)
		{
			auto image = makeImage(brightness[i], brightness[j]);
			images.push_back(image);
		}
		
	}*/
	auto init_image = makeImage(brightness[0], brightness[1]);
	auto image = makeImage(brightness[0], brightness[2]);
	cv::hconcat(init_image, image, init_image);

	image = makeImage(brightness[1], brightness[0]);
	cv::hconcat(init_image, image, init_image);

	auto init_image_2 = makeImage(brightness[1], brightness[2]);
	image = makeImage(brightness[2], brightness[0]);
	cv::hconcat(init_image_2, image, init_image_2);

	image = makeImage(brightness[2], brightness[1]);
	cv::hconcat(init_image_2, image, init_image_2);

	cv::vconcat(init_image_2, init_image, init_image);

	bool res = imwrite("result.png", init_image);

	Mat kernel_1 = (Mat_<float>(2, 2) << 1, 0, 0, -1);
	Mat kernel_2 = (Mat_<float>(2, 2) << 0, 1, -1, 0);

	init_image.convertTo(init_image, CV_32F);


	Mat I_1 = applyConvolution(init_image, kernel_1);
	Mat I_2 = applyConvolution(init_image, kernel_2);

	Mat I_3 = calculateSquare(I_1, I_2);

	Mat mergedImage = mergeChannels(I_1, I_2, I_3);


	imshow("Image_1", I_1);
	imshow("Image_2", I_2);
	imshow("Image_3", I_3);
	imshow("Result image", mergedImage);
	imwrite("merged.png", init_image);
	waitKey(0);
	destroyAllWindows();

}