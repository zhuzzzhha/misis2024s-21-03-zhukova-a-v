#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <iostream>

void ConcatenateJpegExt(std::string& sFileName) {
    if (sFileName.size() > 4)
    {
        if (sFileName.substr(sFileName.length() - 5) != ".jpg")
            sFileName += ".jpg";
    }
    else  sFileName += ".jpg";
}
int main(int argc, char* argv[]) {

    int s = 3, h = 20;
    float gamma = 2.4;
    int max_color = 256;

    if (argc > 1)
        s = std::stoi(argv[1]);
    if (argc > 2)
        h = std::stoi(argv[2]);
    if (argc > 3)
        gamma = std::stoi(argv[3]);

    int border = 10;
    cv::Mat image(h*256 + border, s*256 + border, CV_8UC3, cv::Scalar(255, 255, 255));

    int x_pos = 5, y_pos = 5;
    for (int i = 0; i < max_color; ++i) {
        cv::Scalar color(i, i, i);
        cv::Rect strip_rect(x_pos, y_pos, s, h);
        cv::rectangle(image, strip_rect, color,-1);
        y_pos += h;
    }
    /*cv::pow(image / 255.0, gamma, image);
    image *= 255;*/
    
    if (argc > 4) {
        std::string output_file = argv[4];
        ConcatenateJpegExt(output_file);
        cv::imwrite(output_file, image);
    }
        cv::imshow("Gradient Image", image);
        cv::waitKey(0);
        cv::destroyAllWindows();

}
