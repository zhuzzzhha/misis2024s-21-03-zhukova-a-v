#include "image_generator.h"

//--------------------------------------------------------------------------------
double IoU(const Circle& a, const Circle& b);
//--------------------------------------------------------------------------------
void evaluateDetections(const std::vector<Circle>& groundTruthCircles,
    const std::vector<Circle>& detectedCircles,
    int& TP, int& FP, int& FN,
    double iouThreshold = 0.1);
//--------------------------------------------------------------------------------