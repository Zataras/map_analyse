#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
//#include "opencv2/imgcodecs/imgcodecs.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <algorithm> // In C++, to set them all to -1, you can use something like std::fill_n (from <algorithm>): 
#include "strLines.h"
//#include <bitset> //bitset<1> valuesChange;
//#include <unistd.h>//usleep

using namespace cv;
using namespace std;

/// Global variables


const colors COLORS = 
{
	Vec3b(0,0,0),
	Vec3b(0,255,0)
};

Mat detected_edges, detected_edges2, mapaRobocza, color_dst3;

int edgeThresh = 1;
int lowThreshold = 210, blur_in = 4, blur_out;
//int minLenght = 35; 
int maxGap = 20, thresholdHLP = 15;
int const max_lowThreshold = 300;
int ratio = 3;
int kernel_size = 3;
string str = "Edge Map";//allowed conversion
const char* window_name = str.c_str();

int ANALYSED_PX = 0;

void CannyThreshold(int, void*);

void callFunctions(Mat &aRgbMapR, int width, int minLenght)
{
	
  	/// Reduce noise with a kernel 3x3
  	//blur( src_gray, detected_edges, Size(3,3) );
	//GaussianBlur( src_gray, gb, Size( 1, 1 ), 0, 0 );
		//medianBlur ( src_gray3, src_gray2, blur_out );
	
	//countStdDev(aRgbMapR, aRgbMapR, width);
}

/** @function main */
int main( int argc, char* argv[] )
{

	int minLenght = 35; 
	
	/// Load an image
 	Mat srcRgbImg = imread( argv[1] );

  	if( !srcRgbImg.data )
  		return -1;

	int width = 6;
	if( argc > 2 )
	{
		width = atoi(argv[2]);
		cout << "width = " << width << endl;
	}

	string ty =  type2str( srcRgbImg.type() );
	printf("Matrix: %s %dx%d \n", ty.c_str(), srcRgbImg.cols, srcRgbImg.rows );
	
	//cout << srcRgbImg.rows << " x " << srcRgbImg.cols << endl;
	//srcRgbImg.at<Vec3b>(Point(373, 403)) = COLORS.green;

	//testing:
	Mat auxRgbMap = srcRgbImg.clone();
	
	//Gray pixels to black:
	colorChangeAllRgb(auxRgbMap, Vec3b(205, 205, 205), Vec3b(255, 255, 255));
	
	//srcRgbImg.at<Vec3b>(50, 50) = Vec3b(0, 255, 0);
	
	//that map will be compared with oryginal
	createMapOfMeanLines(srcRgbImg, auxRgbMap);
	
	//here has to put comparing function with at least two Mat arguments
	
	callFunctions(auxRgbMap, width, minLenght);
	
	//cvtColor(src, rgbImg, CV_GRAY2RGB);
	
  	/// Create a window
  	//namedWindow( window_name, WINDOW_AUTOSIZE );
  	/// Create a Trackbar for user to enter threshold
	//createTrackbar( "Min Lenght:", window_name, &minLenght, 100, CannyThreshold );
  	/// Show the image
  	//CannyThreshold(0, 0);
	/// Wait until user exit program by pressing a key
  	//waitKey(0);
	
	destroyAllWindows();
  	return 0;
}

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{		
	//valuesChange.flip();
}
