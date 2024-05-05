#ifndef _OUTPUTTER
#define _OUTPUTTER

#include "MetricConstants.h"



using namespace std;


static void pushValue(MetricId id, double value) {

	std::cout << metricInfo[id].help << " = " <<std::fixed<<std::setprecision(3)<<value<<std::endl;
}



#endif