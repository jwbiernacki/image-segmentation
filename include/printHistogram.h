#ifndef HIST
#define HIST

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

void printHistogram(int arr[], int n)
{
    int maxEle = *max_element(arr, arr + n);

    for (int i = maxEle; i >= 0; i--) {
        cout.width(2);
        cout << right << i << " | ";
        for (int j = 0; j < n; j++) {

            // if array of element is greater
            // then array it print x
            if (arr[j] >= i)
                cout << " x ";

            // else print blank spaces
            else
                cout << "   ";
        }
        cout << "\n";
    }

    // print last line denoted by ----
    for (int i = 0; i < n + 3; i++)
        cout << "---";

    cout << "\n";
    cout << "     ";

    for (int i = 0; i < n; i++) {
        cout.width(2); // width for a number
        cout << right << arr[i] << " ";
    }
}

#endif
