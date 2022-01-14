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

void CallBackFunc(int event, int x, int y, int flags, void* userdata);

//Global Variables
Mat resizedIm;
vector<vector<int>> srcNsink(2);
vector<vector<int>> seedIntensities(2);
bool flag, success;
int nSrc, nSink, N;

int main()
{
    flag = false;
    nSrc = 0;
    nSink = 0;

    int numBFS = 0; //we use these later
    int * bfsPtr = &numBFS;

    // read in image, image resolution
    Mat srcIm = imread("C:\\Users\\jwbie\\OneDrive\\Attachments\\Desktop\\Biernacki_Image_Segmentation\\Images\\test2.jpg", IMREAD_GRAYSCALE);
    if(!srcIm.data) {
        cout << "Image not found" << endl;
        return -1;
        }
    int height = srcIm.size().height;
    int width = srcIm.size().width;
    cout << "Dimensions of input image: " << height << " x " << width << endl;
    //

    //change resolution based on user input
    cout << "The image will be squared. Input new width: ";
    cin >> N;
    resize(srcIm, resizedIm, Size(N, N));
    cout << "Dimensions of resized image: " << N << " x " << N << endl;
    //

    // create (empty) window to display image
    namedWindow( "Display pic" );

    //give user instructions
    cout << "Left/Right click to add background/foreground pixels. " << endl;
    cout << "Press mouse wheel when finished." << endl;

    //call mouse handler to get source/sink pts from user
    setMouseCallback("Display pic", CallBackFunc, &srcNsink);

    while (flag == false) {
        imshow( "Display pic", resizedIm );// show image
        waitKey(1);
    }

    if (flag == true) {
        destroyAllWindows();
    }

    /////////////////////////////////////////////////////

    int n = N*N;
    int V = n + 2;
    int E = n + n + (4*(n-N));

    vector<double> stats = calcStats(N, resizedIm);
    double var = stats[0];
    double stdDev = stats[1];

    vector<vector<int>> edgeInfo = calcAdjInfo(N, V, E, stdDev, srcNsink, seedIntensities, resizedIm);

    int * foundFinal;
    foundFinal = edmondsKarp(V, E, success, bfsPtr, edgeInfo);

    Mat newIm1;
    cvtColor(resizedIm, newIm1, COLOR_GRAY2RGB);

    vector<int> FV = edgeInfo[0];
    vector<int> edges = edgeInfo[1];
    vector<int> space = edgeInfo[4];

    for (int j = 1; j <= n; j++) {
        int xyA = j-1;
        for(int i = FV[j]; i < FV[j+1]; i++)
            {
            if( foundFinal[i] = 1 )
                {
                int xyB = edges[i] - 1;
                int x1 = xyA % N;
                int y1 = (xyA - x1) / N;
                int x2 = xyB % N;
                int y2 = (xyB - x2) / N;
                Point p1(x1, y1);
                Point p2(x2, y2);
                newIm1.at<Vec3b>(p1.x,p1.y) = Vec3b(255, 0, 0);
                newIm1.at<Vec3b>(p2.x,p2.y) = Vec3b(0, 255, 0);
                }
            }
        }

    ////////////////////////////////
    /*
    Queue * Q = createQueue(V);
    int source = 0;
    int * Found = new int[V];
    fill_n(Found, V, -1);
    Enqueue(Q, source); //push source into queue and set found.
    Found[source] = 1;
    int node;

    while(true) {
        if (Q->size == 0) {
            cout << "---Oops! Q is empty. Terminating BFS.---" << endl;
            break;
        }
        node = Dequeue(Q);
        if (node == (V-1)) {
            break;
        }
        for(int i = FV[node]; i < FV[node +1]; i++) {
            if((Found[edges[i]] == -1) && (space[i] > 0))
                {
                Enqueue(Q,edges[i]);
                Found[edges[i]] = 1;
                }
            else if (space[i] <= 0)
                {
                int index1 = node - 1;
                int x1 = index1 % N;
                int y1 = (index1 - x1) / N;
                int index2 = edges[i] - 1;
                int x2 = index2 % N;
                int y2 = (index2 - x2) / N;
                Point p1(x1, y1);
                Point p2(x2, y2);
                newIm1.at<Vec3b>(p1.x,p1.y) = Vec3b(255, 0, 0);
                newIm1.at<Vec3b>(p2.x,p2.y) = Vec3b(0, 255, 0);
                }
            }
        }*/
    ////////////////////////////////


    namedWindow("newImg1", WINDOW_AUTOSIZE);
    imshow("newImg1", newIm1);
    waitKey(0);
    //destroyAllWindows();





return 0;

}


void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
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

    if (  event == EVENT_MBUTTONDOWN  ) {
        flag = true;
        }

     return;
    }
