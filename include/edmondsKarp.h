#ifndef EK
#define EK

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "queue.h"
#include "bfs.h"
#include "pathTracer.h"

using namespace std;
using namespace cv;

int * edmondsKarp(int V, int E, bool success, int * bfsPtr, vector<vector<int>>& edgeInfo) {

    vector<int> FV = edgeInfo[0];
    vector<int> edges = edgeInfo[1];
    vector<int> flow = edgeInfo[2];
    vector<int> capacity = edgeInfo[3];
    vector<int> space = edgeInfo[4];

	vector<int> tempFlow;
	vector<int> tempCap;
	vector<int> tempSpace;
	vector<vector<int>> pathInfo;
	vector<int> edgesOut;

	int minSpace;

	//while possible, run BFS and augment paths (edmonds-karp)//
	while (true) {
        int * result;
        success = false;
        result = BFS(FV, edges, space, V, E, &success);
		if (success == true) {
    			(*bfsPtr)++;
    			cout << "---BFS " << (*bfsPtr) << " successful---" << endl;
			}
        else {
            (*bfsPtr)++;
            cout << "---BFS " << (*bfsPtr) << " failed---" << endl;
            return result;
            }
		pathInfo = backtrace((V-1), result);
		edgesOut = edgePath(pathInfo[0], FV, edges);
		for(int i=0; i < edgesOut.size(); i++) {
            tempFlow.push_back(flow[edgesOut[i]]);
            tempCap.push_back(capacity[edgesOut[i]]);
            tempSpace.push_back(space[edgesOut[i]]);
            }

		minSpace =*min_element(tempSpace.begin(), tempSpace.end());

		for(int i=0; i < edgesOut.size(); i++) {
            flow[edgesOut[i]] = flow[edgesOut[i]] + minSpace;
            space[edgesOut[i]] = space[edgesOut[i]] - minSpace;
            }

		tempFlow.clear();
		tempCap.clear();
		tempSpace.clear();
		pathInfo.clear();
		edgesOut.clear();

        edgeInfo[2] = flow;
        edgeInfo[4] = space;
		}
}

#endif
