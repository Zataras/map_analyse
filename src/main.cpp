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

Mat src, src_gray, gb, src_gray2, src_gray3, src_gray_big;
Mat dst, detected_edges, detected_edges2, color_dst, color_dst2, color_dst3, mapaRobocza;

int edgeThresh = 1;
int lowThreshold = 210, blur_in = 4, blur_out;
//int minLenght = 35; 
int maxGap = 20, thresholdHLP = 15;
int const max_lowThreshold = 300;
int ratio = 3;
int kernel_size = 3;
int kolor = 25;
string str = "Edge Map";//allowed conversion
const char* window_name = str.c_str();

int ANALYSED_PX = 0;

void CannyThreshold(int, void*);

void callFunctions(Mat &aRgbMapR, int width, int minLenght)
{
	
		//blur_out = 1 + blur_in * 2; 
  	/// Reduce noise with a kernel 3x3
  	//blur( src_gray, detected_edges, Size(3,3) );
	//detected_edges_blured = detected_edges.clone();
	//GaussianBlur( src_gray, gb, Size( 1, 1 ), 0, 0 );
		//medianBlur ( src_gray3, src_gray2, blur_out );

	//Point pt = lookForWhitePxls(detected_edges, {0, 0});
	//cout << "ZNALEZNIONO PUNKT: " << pt << endl
	
	//countStdDev(aRgbMapR, aRgbMapR, width);
	//Point pt(0,0);	
	/*while(pt.x  < (aRgbMapR.rows - 1) || pt.y < (aRgbMapR.cols - 1))
	{
		cout << "new cycle"<< endl;	
		//pt = lookForSpecColPxls(aRgbMapR, pt, Vec3b(254, 254, 254));
		aRgbMapR.at<Vec3b>(pt) = Vec3b(0, 255, 0);
	}*/
	
		//Mat detected_edges, greyMap;
		//cvtColor( aRgbMapR, greyMap, CV_BGR2GRAY );


  	//showResized(aRgbMapR, "aRgbMapR", 2.0, 0);

		//cout << "Analysed pixels: " << ANALYSED_PX << endl;
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
	
	createMapOfMeanLines(srcRgbImg, auxRgbMap);
	
	
	
	//showResized(srcRgbImg, "powiekszone", 3.0);
	//int value = srcRgbImg.at<uchar>(50, 50);
	//cout << endl << value << endl;
	//value = srcRgbImg.at<uchar>(1, 1);
	//cout << endl << value << endl;
	

	
	//cvtColor(src, rgbImg, CV_GRAY2RGB);
	
  	/// Create a window
  	//namedWindow( window_name, WINDOW_AUTOSIZE );

  	/// Create a Trackbar for user to enter threshold
  	//createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
	//createTrackbar( "Min Lenght:", window_name, &minLenght, 100, CannyThreshold );

  	/// Show the image
  	//CannyThreshold(0, 0);
	callFunctions(auxRgbMap, width, minLenght);

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
