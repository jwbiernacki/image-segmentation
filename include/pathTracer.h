#ifndef PATH_TRACE
#define PATH_TRACE

#include<iomanip>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<vector<int>> backtrace(int node, int * parent) {
    //cout << "---Starting backtrace---" << endl;
    vector<vector<int>> pL;
    vector<int> path;
    vector<int> pathLength;

	int iter2 = 1;
	int tempNode = node;

	while(parent[tempNode] != 0) {
		tempNode = parent[tempNode];
		iter2++;
		}

    pathLength.push_back(iter2 + 1);

	int newNode = node;
	path.push_back(newNode);

	while(true) {
        if (parent[newNode] == 0) {
            //cout << "Parent of " << newNode << " is " << parent[newNode] << " << here it is!!!" << endl;
            break;
        }
	    //cout << "Parent of " << newNode << " is " << parent[newNode] << endl;
		newNode = parent[newNode];
		path.push_back(newNode);
		}
    path.push_back(0);
    pL.push_back(path);
    pL.push_back(pathLength);
    return pL;
}

vector<int> edgePath(vector<int>& pathInfo, vector<int>& FV, vector<int>& edges) {
    vector<int> edgesOut;
    for (int i = (pathInfo.size() - 1); i > 0 ; i--) {
        int j = pathInfo[i];
        for (int k = FV[j]; k < FV[j+1]; k++) {
            if (edges[k] == pathInfo[i-1])
                {
                edgesOut.push_back(k);
                break;
                }
            }
        }
    return edgesOut;
    }

#endif // PATH_TRACE
