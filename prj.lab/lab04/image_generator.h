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

#define GENERATE_FILENAME(name, kernel_size) (name + std::to_string(kernel_size) + ".png")
#define GENERATE_JSONNAME(name, kernel_size) (name + std::to_string(kernel_size) + ".json")

//--------------------------------------------------------------------------------
struct Circle
{
    double x;
    double y;
    double r;
    int brightness;
};
//--------------------------------------------------------------------------------
static int num_file = 1;
//--------------------------------------------------------------------------------
struct Statistics
{
    std::vector<Circle> circles;
    int image_width;
    int image_height;
    int background_color;
    int blur;
    double noise;
};


//--------------------------------------------------------------------------------
struct ImageWithStats {
    cv::Mat image;
    std::optional<Statistics> stats;
};
//--------------------------------------------------------------------------------
void writeInJSON(const std::string& filename, Statistics& stats);
//--------------------------------------------------------------------------------
cv::Mat addGaussianNoise(cv::Mat image, double std);
//--------------------------------------------------------------------------------
void generateImageWithCircles(ImageWithStats& img_stat, int count, int min_radius, int max_radius, int min_contrast, int max_contrast, double std, int blur, bool bSave);
//--------------------------------------------------------------------------------
void on_trackbar(int, void*);
//--------------------------------------------------------------------------------
void CreateTrackbars();
//--------------------------------------------------------------------------------
ImageWithStats GetImage();