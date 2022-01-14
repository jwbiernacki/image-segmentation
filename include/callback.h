#ifndef MOUSE_HANDLER
#define MOUSE_HANDLER

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "queue.h"
#include "bfs.h"
#include "pathTracer.h"
#include "edmondsKarp.h"
#include "calcAdjInfo.h"
#include "calcStats.h"

using namespace std;
using namespace cv;

void callback(int event, int x, int y, int flags, void* userdata) {
     if  ( event == EVENT_LBUTTONDOWN ) {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

        int srcIndex = (int) ( (y*N) + x + 1 );
        srcNsink[0].push_back(srcIndex);
        nSrc++;

        int intensity = (int) resizedIm.at<uchar>(y,x);
        seedIntensities[0].push_back(intensity);

        cout << "Source pixel index: " << srcIndex << endl;
        }

    if  ( event == EVENT_RBUTTONDOWN ) {
        cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

        int sinkIndex = (int) ( (y*N) + x + 1 );
        srcNsink[1].push_back(sinkIndex);
        nSink++;

        int intensity = (int) resizedIm.at<uchar>(y,x);
        seedIntensities[1].push_back(intensity);

        cout << "Sink pixel index: " << sinkIndex << endl;
        }

    if (  event == EVENT_MBUTTONDOWN  ) { //EVENT_FLAG_SHIFTKEY c
        flag = true;
        //cout << "Flag: " << flag << endl;
        }

     return;
    }

#endif // MOUSE_HANDLER
