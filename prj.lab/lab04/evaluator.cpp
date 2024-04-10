#include "evaluator.h"

//--------------------------------------------------------------------------------
double IoU(const Circle& a, const Circle& b)
{
    double distCenters = std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));

    if (distCenters >= a.r + b.r) return 0.0;

    if (distCenters + std::min(a.r, b.r) <= std::max(a.r, b.r)) {
        return M_PI * std::pow(std::min(a.r, b.r), 2) / M_PI * std::pow(std::max(a.r, b.r), 2);
    }

    double angleA = std::acos((std::pow(a.r, 2) + std::pow(distCenters, 2) - std::pow(b.r, 2)) / (2 * a.r * distCenters));
    double angleB = std::acos((std::pow(b.r, 2) + std::pow(distCenters, 2) - std::pow(a.r, 2)) / (2 * b.r * distCenters));
    double intersectionArea = angleA * std::pow(a.r, 2) + angleB * std::pow(b.r, 2)
        - a.r * distCenters * std::sin(angleA);

    double areaA = M_PI * std::pow(a.r, 2);
    double areaB = M_PI * std::pow(b.r, 2);

    double unionArea = areaA + areaB - intersectionArea;

    return intersectionArea / unionArea;
}
//--------------------------------------------------------------------------------
void evaluateDetections(const std::vector<Circle>& groundTruthCircles,
    const std::vector<Circle>& detectedCircles,
    int& TP, int& FP, int& FN,
    double iouThreshold)
{
    std::vector<bool> detectedFlags(detectedCircles.size(), false);
    for (const auto& gtCircle : groundTruthCircles) {
        bool found = false;

        for (size_t i = 0; i < detectedCircles.size() && !found; ++i) {
            if (IoU(gtCircle, detectedCircles[i]) >= iouThreshold && !detectedFlags[i]) {
                TP++;
                detectedFlags[i] = true;
                found = true;
            }
        }
        if (!found) {
            FN++;
        }
    }

    FP = std::count(detectedFlags.begin(), detectedFlags.end(), false);
}
