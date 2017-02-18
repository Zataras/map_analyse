#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
//#include "opencv2/imgcodecs/imgcodecs.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <algorithm> // In C++, to set them all to -1, you can use something like std::fill_n (from <algorithm>): 
#include "strLines.h"
#include <bitset>
							// std::fill_n(array, 100, -1);
//#include <unistd.h>//usleep

using namespace cv;
using namespace std;

/// Global variables

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

//bitset<1> valuesChange;

void CannyThreshold(int, void*);

void callFunctions(Mat &aRgbMapR, int width, int minLenght)
{
	
		//blur_out = 1 + blur_in * 2; 
  	/// Reduce noise with a kernel 3x3
  	//blur( src_gray, detected_edges, Size(3,3) );
	//detected_edges_blured = detected_edges.clone();
	//GaussianBlur( src_gray, gb, Size( 1, 1 ), 0, 0 );
		//medianBlur ( src_gray3, src_gray2, blur_out );

	//test
	//Point pt = lookForWhitePxls(detected_edges, {0, 0});
	//cout << "ZNALEZNIONO PUNKT: " << pt << endl;
		//countStdDev(aRgbMapR, aRgbMapR, width);
	Point pt(0,0);	
	/*while(pt.x  < (aRgbMapR.rows - 1) || pt.y < (aRgbMapR.cols - 1))
	{
		cout << "new cycle"<< endl;	
		//pt = lookForSpecColPxls(aRgbMapR, pt, Vec3b(254, 254, 254));
		aRgbMapR.at<Vec3b>(pt) = Vec3b(0, 255, 0);
	}*/
	
	Mat detected_edges, greyMap;
	cvtColor( aRgbMapR, greyMap, CV_BGR2GRAY );
	Canny( greyMap, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
	vector<Vec4i> lines;
	//threshold[, lines[, minLineLength[, maxLineGap]]]
	//int minLenght = 10;
	int maxGap = 10;
	int thresholdHLP = 5;
	bitset<1> valuesChangeSaved;
	//valuesChangeSaved.set();	

  	showResized(aRgbMapR, "aRgbMapR", 2.0, 0);

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

  	/// Create a matrix of the same type and size as src (for dst)
  	//dst.create( src.size(), src.type() );

  	/// Convert the image to grayscale
  	//Needed???
  	//cvtColor( src, src_gray, COLOR_BGR2GRAY );
	
	//Gray pixels to black:
	//Gr2Bl(src_gray, src_gray3, 205);

	//testing:
	Mat auxRgbMap = srcRgbImg.clone();
	
	srcRgbImg.at<Vec3b>(50, 50) = Vec3b(0, 255, 0);
	
	//showResized(srcRgbImg, "powiekszone", 3.0);
	//int value = srcRgbImg.at<uchar>(50, 50);
	//cout << endl << value << endl;
	//value = srcRgbImg.at<uchar>(1, 1);
	//cout << endl << value << endl;
	
	string ty =  type2str( srcRgbImg.type() );
	printf("Matrix: %s %dx%d \n", ty.c_str(), srcRgbImg.cols, srcRgbImg.rows );
	
	//cvtColor(src, rgbImg, CV_GRAY2RGB);
	
	Vec3b intensity = srcRgbImg.at<Vec3b>(50, 50);
	/*uchar blue = intensity.val[0];
	uchar green = intensity.val[1];
	uchar red = intensity.val[2];*/
	cout  << intensity << endl;
	intensity = srcRgbImg.at<Vec3b>(1, 1);
	cout  << intensity << endl;
	
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
