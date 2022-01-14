#ifndef KMEANS
#define KMEANS

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "calcCapacity.h"

using namespace std;
using namespace cv;

vector<double> kMeans(vector<int>& pixelIntensities, vector<vector<int>>& srcNsink) {

    int nPix = pixelIntensities.size();
    vector<double> E1(nPix);

    int nSrc = srcNsink[0].size();
    int nSink = srcNsink[1].size();

    for (int i = 1; i <= nPix; i++)
    {
        double minDistSrc = 999999;
        double minDistSink = 999999;
        double pixIntensity = pixelIntensities[i-1];
        for (int j = 0; j < nSrc; j++)
            {
            double tempSrc = pow((pixIntensity - pixelIntensities[((srcNsink[0])[j] - 1)]),2);
            if (tempSrc < minDistSrc) {
                minDistSrc = tempSrc;
                }
            }
        for (int j = 0; j < nSink; j++)
            {
            double tempSink = pow((pixIntensity - pixelIntensities[((srcNsink[0])[j] - 1)]),2);
            if (tempSink < minDistSink) {
                minDistSink = tempSink;
                }
            }
        double E1src = (minDistSrc / (minDistSrc + minDistSink));
        double E1sink = (minDistSink / (minDistSrc + minDistSink));
        /*if (E1src < E1sink) {
            E1[i-1] = E1src;
            }
        else {
            E1[i-1] = E1sink;
            }*/
        E1[i-1] = E1src;
        cout << E1[i-1] << endl;
    }

    return E1;
}

int kMeansSingle(int pixIntensity, vector<vector<int>>& seedIntensities, vector<vector<int>>& srcNsink) {

    int nSrc = (srcNsink[0]).size();
    int nSink = (srcNsink[1]).size();

    double minDistSrc = 999999;
    double minDistSink = 999999;
        for (int j = 0; j < nSrc; j++)
            {
            double tempSrc = pow((pixIntensity - seedIntensities[0][j]),2);
            if (tempSrc < minDistSrc) {
                minDistSrc = tempSrc;
                }
            }
        for (int j = 0; j < nSink; j++)
            {
            double tempSink = pow((pixIntensity - seedIntensities[1][j]),2);
            if (tempSink < minDistSink) {
                minDistSink = tempSink;
                }
            }

    int Rsrc = (int) ceil((100 * (minDistSrc / (minDistSrc + minDistSink))));
    //cout << Rsrc << endl;
    return Rsrc;
}


#endif
