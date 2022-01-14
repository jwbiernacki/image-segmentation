#ifndef ADJ_INFO
#define ADJ_INFO

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "calcCapacity.h"
#include "kMeans.h"

using namespace std;
using namespace cv;

vector<vector<int>> calcAdjInfo(int N, int V, int E, double var, vector<vector<int>>& srcNsink, vector<vector<int>>& seedIntensities, Mat resizedIm) {

int n = N*N;
int nSrc = (srcNsink[0]).size();
int edgeCtr = n;
int maxCap = 0;
double intensitySum = 0;

vector<int>::iterator itSrc;
vector<int>::iterator itSink;
vector<int> FV(V);
vector<int> edges(E+1);
vector<int> flow(E+1);
vector<int> capacity(E+1);
vector<int> space(E+1);

vector<int> pixelIntensities(N*N);

int * foundFV = new int[V];
fill_n(foundFV, V, -1);

foundFV[0] = 1;
foundFV[V-1] = 1;
FV[0] = 0;
FV[V-1] = E;
edges[E] = -1;
flow[E] = -1;
space[E] = -1;
capacity[E] = -1;

for ( int y = 0; y < N ; y++) {
	for (int x = 0; x < N; x++) {

		//calculate pixel index
		int index = (int) ( (y*N) + x + 1 );
		int intensity1 = (int) resizedIm.at<uchar>(y,x);

        int xL = x - 1;
        int xR = x + 1;
        int yU = y - 1;
        int yD = y + 1;

		int UP = (int) ( (yU*N) + x + 1 );
		int LEFT = (int) ( (y*N) + xL + 1 );
		int RIGHT = (int) ( (y*N) + xR + 1 );
		int DOWN = (int) ( (yD*N) + x + 1);

		if ( (yU>=0) && (yU<N) ) {
			if (foundFV[index] == -1) {
				FV[index] = edgeCtr;
				foundFV[index] = 1;
				}
			edges[edgeCtr] = UP;
            int intensity2 = (int) resizedIm.at<uchar>(yU,x);
            flow[edgeCtr] = 0;
            capacity[edgeCtr] = calcCapacity(intensity1, intensity2, var);
            if (capacity[edgeCtr] > maxCap) {
                maxCap = capacity[edgeCtr];
                }
            space[edgeCtr] = capacity[edgeCtr];
			edgeCtr++;
			}

		if ( (xL>=0) && (xL<N) ) {
			if (foundFV[index] == -1) {
				FV[index] = edgeCtr;
				foundFV[index] = 1;
			}
			edges[edgeCtr] = LEFT;
            int intensity2 = (int) resizedIm.at<uchar>(y,xL);
            flow[edgeCtr] = 0;
            capacity[edgeCtr] = calcCapacity(intensity1, intensity2, var);
            if (capacity[edgeCtr] > maxCap) {
                maxCap = capacity[edgeCtr];
                }
            space[edgeCtr] = capacity[edgeCtr];
			edgeCtr++;
			}

		if ( (xR>=0) && (xR<N) ) {
			if (foundFV[index] == -1) {
				FV[index] = edgeCtr;
				foundFV[index] = 1;
			}
			edges[edgeCtr] = RIGHT;
            int intensity2 = (int) resizedIm.at<uchar>(y,xR);
            flow[edgeCtr] = 0;
            capacity[edgeCtr] = calcCapacity(intensity1, intensity2, var);
            if (capacity[edgeCtr] > maxCap) {
                maxCap = capacity[edgeCtr];
                }
            space[edgeCtr] = capacity[edgeCtr];
			edgeCtr++;
			}

		if ( (yD>=0) && (yD<N) ) {
			if (foundFV[index] == -1) {
				FV[index] = edgeCtr;
				foundFV[index] = 1;
			}
			edges[edgeCtr] = DOWN;
            int intensity2 = (int) resizedIm.at<uchar>(yD,x);
            flow[edgeCtr] = 0;
            capacity[edgeCtr] = calcCapacity(intensity1, intensity2, var);
            if (capacity[edgeCtr] > maxCap) {
                maxCap = capacity[edgeCtr];
                }
            space[edgeCtr] = capacity[edgeCtr];
			edgeCtr++;
			}

		itSink = find((srcNsink[1]).begin(), (srcNsink[1]).end(), index);
        itSrc = find((srcNsink[0]).begin(), (srcNsink[0]).end(), index);

        edges[edgeCtr] = V - 1;
        flow[edgeCtr] = 0;
        if (itSink != (srcNsink[1]).end()){
            capacity[edgeCtr] = -9999;
            space[edgeCtr] = -9999;
        }
        else if (itSrc != (srcNsink[0]).end()) {
            capacity[edgeCtr] = 0;
            space[edgeCtr] = 0;
        }
        else {
            capacity[edgeCtr] = kMeansSingle(pixelIntensities[(index-1)], seedIntensities, srcNsink);
            space[edgeCtr] = capacity[edgeCtr];
        }

        edgeCtr++;
        }
    }

    for (int i = 0; i < n ; i++) {
            int x = i % N;
            int y = (i - x) / N;
            int index = i + 1;
            int intensity1 = (int) resizedIm.at<uchar>(y,x);
            edges[i] = index;
            flow[i] = 0;
            itSrc = find((srcNsink[0]).begin(), (srcNsink[0]).end(), index);
            itSink = find((srcNsink[1]).begin(), (srcNsink[1]).end(), index);
            if (itSrc != (srcNsink[0]).end()){
                capacity[i] = maxCap + 100;
                space[i] = maxCap + 100;
            }
            else if (itSink != (srcNsink[1]).end()) {
                capacity[i] = 0;
                space[i] = 0;
            }
            else {
                capacity[i] = 100 - kMeansSingle(intensity1, seedIntensities, srcNsink);
                space[i] = capacity[i];
            }
        }

    for (int i = 0; i < capacity.size(); i++){
            if (capacity[i] == -9999) {
                capacity[i] = maxCap + 100;
                space[i] = maxCap + 100;
                }
            cout << "Edge " << i << ": " << capacity[i] << endl;
        }

	vector<vector<int>> output(5);
	output[0] = FV;
	output[1] = edges;
	output[2] = flow;
    output[3] = capacity;
    output[4] = space;

    return output;
}

#endif
