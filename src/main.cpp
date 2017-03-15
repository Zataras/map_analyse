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

//BGR
const colors COLORS = 
{
	Vec3b(255,0,0),
	Vec3b(0,255,0),
	Vec3b(0,0,255),
	Vec3b(0,0,0),
	Vec3b(255,255,255),
	Vec3b(205,205,205),
	Vec3b(255,255,0),
	Vec3b(255,153,0)
};



Mat detected_edges, detected_edges2, mapaRobocza, color_dst3;
Mat edgesRgbMap;

double resizeFactor = 6.0;

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

//void coutLine(int line)

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
	//cout << "Press 'c' key to continue";
	
	int minLenght = 35; 
	string message;
	/// Load an image
	if (argc < 1) {
		cout << "Name of image not specified";
      return -1;
   }
 	Mat srcRgbImg = imread( argv[1] );

  	if( !srcRgbImg.data )
  		return -1;

	//showResized(srcRgbImg, "srcRgbImg", 2.5, 0); //debug

	int width = 6;
	if( argc > 2 )
	{
		width = atoi(argv[2]);
		cout << "width = " << width << endl;
	}

	string ty =  type2str( srcRgbImg.type() );
	message = "Matrix: " + ty + " " + to_string(srcRgbImg.cols) + "x" + to_string(srcRgbImg.rows);
	SHOW(message);
	//printf("Matrix: %s %dx%d \n", ty.c_str(), srcRgbImg.cols, srcRgbImg.rows );

	//testing:
	Mat auxRgbMap = srcRgbImg.clone();
	
	//showResized(auxRgbMap, "auxRgbMap", 2.5, 0); //debug
	
	//Gray pixels to black:
	colorChangeAllRgb(auxRgbMap, COLORS.black, COLORS.grey);
	
	//for(int i=1; i<=1; i+=2)
	//{
		//expose edges
	int blur_out = 3;
	lowThreshold = 70;
	
	//SHOW("1");
	
	edgesRgbMap = auxRgbMap.clone();
	//SHOW("2");
	medianBlur ( auxRgbMap, auxRgbMap, blur_out );
	Mat greyMap;
	cvtColor(auxRgbMap, greyMap, CV_RGB2GRAY);
	Canny( greyMap, edgesRgbMap, lowThreshold, lowThreshold*ratio, kernel_size );
		//SHOW(i); 
		//showResized(edgesRgbMap, "testMap", 2.5, 1); //debug
	//}
	
	//srcRgbImg.at<Vec3b>(50, 50) = Vec3b(0, 255, 0);
	
	//that map will be compared with oryginal
	bitwise_not ( edgesRgbMap, edgesRgbMap );
	cvtColor(edgesRgbMap, edgesRgbMap, CV_GRAY2RGB);
	
	namedWindow("debug window", WINDOW_AUTOSIZE);//WINDOW_AUTOSIZE);
	setMouseCallback("debug window", onMouse, NULL);
	
	createMapOfMeanLines(srcRgbImg, edgesRgbMap);
	
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
