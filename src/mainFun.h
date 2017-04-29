/*
Functions called in main function
Header file mainFun.h
*/

#ifndef FILE_MAINFUN_SEEN
#define FILE_MAINFUN_SEEN

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

//returns value of standard deviation in pixels
//arguments: Mat with only black and white pixels
//		width - how far from line to check
//Start and end point of mean straight line
float countStdDev(Mat &srcImg, int width, Point startPt, Point endPt);

#endif /* !FILE_MAINFUN_SEEN */
