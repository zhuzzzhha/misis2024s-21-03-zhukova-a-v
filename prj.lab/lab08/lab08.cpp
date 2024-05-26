#include <fstream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include <random>


struct Point {
    double x, y, z;
};

Point projectToPlane(const Point& p) {
    double A = 0.5, B = 0.5, C = -1;
    double coeff = A * p.x + B * p.y + C * p.z;
    return { p.x - A * coeff, p.y - B * coeff, p.z - C * coeff };
}

struct Parameters {
    double a, b;
};

Parameters parametersFor(const Point& M) {
    Point V1 = { 2, 0, 1 };
    Point V2 = { 0, 2, 1 };

    double determinant = V1.x * V2.y - V1.y * V2.x;
    double a = (M.x * V2.y - V1.y * M.y) / determinant;
    double b = (V1.x * M.y - M.x * V1.y) / determinant;

    return { a, b };
}

std::vector<Point> generateRandomPoints(int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);

    std::vector<Point> points;
    for (int i = 0; i < n; i++) {
        points.push_back({ static_cast<double>(dis(gen)), static_cast<double>(dis(gen)), static_cast<double>(dis(gen)) });
    }
    return points;
}

double normalize(int value) {
    return value / 255.0;
}


void normalizePoints(vector<Point>& points) {
    for (auto& point : points) {
        point.x = normalize(point.x);
        point.y = normalize(point.y);
        point.z = normalize(point.z);
    }
}
int main(int argc, char* argv[]) {

    cv::CommandLineParser parser(argc, argv,
        "{@input_file+ | | file name}");

    std::string input_put_file = parser.get<std::string>("@+");
    std::ofstream outputFile("params.txt");

    cv::Mat img = cv::imread(input_put_file);

    std::vector<Point> points;
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            cv::Vec3b intensity = img.at<cv::Vec3b>(i, j);
            int blue = intensity.val[0];
            int green = intensity.val[1];
            int red = intensity.val[2];
            Point point;
            point.x = red;
            point.y = green;
            point.z = blue;
            points.push_back(point);
        }
    }
    normalizePoints(points);

    for (auto& p : points)
    {
        Point p_prime = projectToPlane(p);
        //cout << "Projection of point (" << p.x << ", " << p.y << ", "<< p.z << ") is: (" << p_prime.x << ", " << p_prime.y << ", " << p_prime.z << ")"<< endl;
        Parameters params = parametersFor(p_prime);

        // Writing parameters of projection and RGB color to the file
        outputFile << params.a << " " << params.b << " " << p.x << " " << p.y << " " << p.z << '\n';
    }

    return 0;
}