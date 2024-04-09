#pragma once

#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <fstream>
#include <optional>
#include <vector>
#include <random>
#include <optional>
#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/features2d.hpp"

using namespace cv;
Mat LoG(Mat& image_src);