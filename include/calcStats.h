#ifndef STATS
#define STATS

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "printHistogram.h"


using namespace std;
using namespace cv;

vector<double> calcStats(int N, Mat resizedIm) {

double newN = (double) N;
int n = N * N;
double sum = 0;
double temp = 0;
double mean, var, stdDev;
bool even = false;
int nQ;
int counter = 0;

if (n % 2 == 0) {
	even = true;
	}

if (even) {
	nQ = n / 2;
	}
else {
	nQ = ((n - 1) / 2);
	}

vector<double> Q1, Q3;
vector<int> pixelIntensities, intensityLabels;

for ( int y = 0; y < N ; y++) {
	for (int x = 0; x < N; x++) {
		counter++;
		double pixelIntensity = (double) resizedIm.at<uchar>(y,x);
		int pixInt = (int) resizedIm.at<uchar>(y,x);
		if (counter <= nQ) {
			Q1.push_back(pixelIntensity);
			}
		else if (counter > (n - nQ)) {
			Q3.push_back(pixelIntensity);
			}
        pixelIntensities.push_back(pixInt);
        intensityLabels.push_back(pixInt);
		sum = sum + pixelIntensity;
		}
	}

mean = sum / (newN * newN);

sort(pixelIntensities.begin(), pixelIntensities.end());
sort(intensityLabels.begin(), intensityLabels.end());
vector<int>::iterator myIt;
myIt = unique(intensityLabels.begin(), intensityLabels.end());
intensityLabels.resize(distance(intensityLabels.begin(),myIt));

vector<int> freq(256);
vector<int> labels2(256);

for (int i = 0; i < 256; i++) {
    labels2[i] = i;
    freq[i] = 0;
}

int tempCtr = 0;
int currentFreq = 0;
for (int i = 0; i < 256; i++) {
    while (pixelIntensities[tempCtr] == i) {
        currentFreq++;
        tempCtr++;
    }
    freq[i] = currentFreq;
    currentFreq = 0;
}

ofstream myFile;
    myFile.open("histogram.dat");

    for (int i = 0; i < 256; i++) {
        myFile << labels2[i] << " " << freq[i] << endl;
    }
    myFile.close();

double medQ1, medQ3, IQR;
size_t varSize = sizeof(Q1[0]);
size_t nQ1 = Q1.size() / 2;
size_t nQ3 = Q3.size() / 2;
nth_element(Q1.begin(), (Q1.begin()+nQ1), Q1.end());
nth_element(Q1.begin(), (Q1.begin()+nQ1+varSize), Q1.end());
nth_element(Q3.begin(), (Q3.begin()+nQ3), Q3.end());
nth_element(Q3.begin(), (Q3.begin()+nQ3+varSize), Q3.end());

medQ1 = (Q1[nQ1] + Q1[nQ1+varSize]) / 2;
medQ3 = (Q3[nQ3] + Q3[nQ3+varSize]) / 2;

IQR = medQ3 - medQ1;
cout << "IQR: " << IQR << endl;

double binWidth = (2 * IQR) / pow(n,(1.0/3)); //Freedman-Diaconis rule
cout << "binWidth: " << binWidth << endl;

for ( int y = 0; y < N ; y++) {
	for (int x = 0; x < N; x++) {
		double pixelIntensity = (double) resizedIm.at<uchar>(y,x);
		temp = temp + pow((pixelIntensity - mean),2);
		}
	}

var = temp / (n);
stdDev = sqrt(var);

int * intensityArr = &pixelIntensities[0];

//printHistogram(intensityArr, n);



vector<double> outVec(2);
outVec[0] = var;
outVec[1] = stdDev;

return outVec;

}


#endif
