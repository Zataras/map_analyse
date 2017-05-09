/* File main.h  */
#ifndef FILE_MAIN_SEEN
#define FILE_MAIN_SEEN

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <algorithm> // In C++, to set them all to -1, you can use something like std::fill_n (from <algorithm>):
//#include <bitset> //bitset<1> valuesChange;
//#include <unistd.h>//usleep

#include "mainFun.h"
#include "vecOfMeanLns.h"

using namespace cv;
using namespace std;

/// Global variables

//this global variable consists of vector of mean values counted for each row in series
//and can be used to visualize supposed position of obstacle's edge
//VectorOfMeanLines: <line1, line2, line3> each line: vector of mean points:<floatX, floatY>
vector<structVecOfMeanPts> VecOfMeanPts;
vector<Vec2f> vecOfMeanVals;

Mat edgesRgbMap; //global temporary to make it avaliable for mouse callback
double resizeFactor = 4.0; 	//global to change it globaly in whole project from one place 
										//when working on different resolutions
string message;

int edgeThresh = 1;
int lowThreshold = 210, blur_in = 4, blur_out;
int maxGap = 20, thresholdHLP = 15;
int const max_lowThreshold = 300;
int ratio = 3;
int kernel_size = 3;
string str = "Edge Map";//allowed conversion
const char* window_name = str.c_str();

int ANALYSED_PX = 0;

void CannyThreshold(int, void*);


#endif /* !FILE_MAIN_SEEN */
