#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

void gammaCorrection(const cv::Mat& src, cv::Mat& dst, const float gamma)
{
    float invGamma = 1 / gamma;

    cv::Mat table(1, 256, CV_8U);
    uchar* p = table.ptr();
    for (int i = 0; i < 256; ++i) {
        p[i] = (uchar)(pow(i / 255.0, invGamma) * 255);
    }

    cv::LUT(src, table, dst);
}
int main(int argc, char* argv[]) {

    int s, h;
    double gamma;
    int max_color = 256;
    std::string output_file = "";
    std::filesystem::path executable_path = std::filesystem::current_path();

    cv::CommandLineParser parser(argc, argv,
        "{s         | 3| size value}"
        "{h         | 20| height value}"
        "{gamma          | 2.4| gamma value}"
        "{@file_name | | file name}");

    s = parser.get<int>("s");
    h = parser.get<int>("h");
    gamma = parser.get<double>("gamma");
    output_file = parser.get<std::string>("@file_name");

   /* if (argc > 1)
        s = std::stoi(argv[1]);
    if (argc > 2)
        h = std::stoi(argv[2]);
    if (argc > 3)
        gamma = std::stof(argv[3]);*/

    int border = 10;
    int image_height = h, image_width = 256 * s;
    cv::Mat image(image_height, image_width, CV_8UC3, cv::Scalar(255, 255, 255));

    int x_pos = 0, y_pos = 0;
    for (int i = 0; i < max_color; ++i) {
        cv::Scalar color(i, i, i);
        cv::Rect strip_rect(x_pos, y_pos, s, h);
        cv::rectangle(image, strip_rect, color, -1);
        x_pos += s;
    }

    cv::Mat gammaImg(image_height, image_width, CV_8UC3, cv::Scalar(255, 255, 255));
    gammaCorrection(image, gammaImg, gamma);
    
    cv::vconcat(image, gammaImg, image);


    if (output_file.size()) {
        std::filesystem::path full_path = executable_path / output_file;
        cv::imwrite(full_path.string(), image);
    }
        cv::imshow("Gradient Image", image);
        cv::waitKey(0);
    
    cv::destroyAllWindows();

}
