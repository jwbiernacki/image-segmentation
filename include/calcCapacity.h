#ifndef CAPACITY_FUNC
#define CAPACITY_FUNC

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

int calcCapacity(int i1, int i2, double stdDev) {

    double I1 = (double) i1;
    double I2 = (double) i2;


    double expTerm = ( -pow((I1 - I2),2) / (2 * stdDev) );

    int CAPACITY = (int) ceil((100 * exp(expTerm)));

    //cout << "stdDev: " << stdDev << ", I1: " << I1 << ", I2: " << I2 << ", cap: " << CAPACITY << endl;

    return CAPACITY;
    }

int calcCapacity2(int i1, int i2, double stdDev) {

    double I1 = (double) i1;
    double I2 = (double) i2;


    double expTerm = ( -pow((I1 - I2),2) / (2 * stdDev) );

    int CAPACITY = (int) (( 100 * exp(expTerm) ) + 1);

    //cout << "stdDev: " << stdDev << ", I1: " << I1 << ", I2: " << I2 << ", cap: " << CAPACITY << endl;

    return CAPACITY;
    }

#endif
