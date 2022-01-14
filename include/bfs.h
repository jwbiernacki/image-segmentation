#ifndef BFS_
#define BFS_

#include<iomanip>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "queue.h"

using namespace std;

int * BFS(vector<int> FV, vector<int> edges, vector<int>& space, int V, int E, bool * success);

int * BFS(vector<int> FV, vector<int> edges, vector<int>& space, int V, int E, bool * success) {
    cout << "---Starting BFS---" << endl;

    Queue * Q = createQueue(V);
    int source = 0;

    int * Found = new int[V];
    fill_n(Found, V, -1);

    int * fringe = new int[E];
    fill_n(fringe, V, -1);

    Enqueue(Q, source); //push source into quene and set found.
    Found[source] = 1;

    int node;

    int * parent = new int[V];
    fill_n(parent, V, -1);

    while(true) {
        if (Q->size == 0) {
            cout << "---Oops! Q is empty. Terminating BFS.---" << endl;
            return fringe;
        }
        node = Dequeue(Q);
        //printQ(Q);
        //cout << "node = " << node << ", Found: " << Found[node] << endl;;
        if (node == (V-1)) {
            *success = true;
            cout << "---Found Sink, terminating BFS.---" << endl;
            return parent;
        }
        for(int i = FV[node]; i < FV[node +1]; i++)
            {
            if((Found[edges[i]] == -1) && (space[i] > 0))
                {
                Enqueue(Q,edges[i]);
                Found[edges[i]] = 1;
                fringe[edges[i]] = -1;
                parent[edges[i]] = node;
                }
            else {
                fringe[i] = 1;
                }
            }
    }
    *success = false;
    return fringe;
}

#endif // BFS
