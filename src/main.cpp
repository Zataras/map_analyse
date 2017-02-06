#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
//#include "opencv2/imgcodecs/imgcodecs.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <algorithm> // In C++, to set them all to -1, you can use something like std::fill_n (from <algorithm>): 
#include "strLines.h"
							// std::fill_n(array, 100, -1);
//#include <unistd.h>//usleep

using namespace cv;
using namespace std;

/// Global variables

Mat src, src_gray, gb, src_gray2, src_gray3, src_gray_big;
Mat dst, detected_edges, detected_edges2, color_dst, color_dst2, color_dst3, mapaRobocza;

int edgeThresh = 1;
int lowThreshold = 210, blur_in = 4, blur_out;
int minLenght = 35, maxGap = 20, thresholdHLP = 15;
int const max_lowThreshold = 300;
int ratio = 3;
int kernel_size = 3;
int kolor = 25;
char* window_name = "Edge Map";
int ANALYSED_PX = 0;

void callFunctions(int width)
{
	blur_out = 1 + blur_in * 2; 
  /// Reduce noise with a kernel 3x3
  //blur( src_gray, detected_edges, Size(3,3) );
	//detected_edges_blured = detected_edges.clone();
	//GaussianBlur( src_gray, gb, Size( 1, 1 ), 0, 0 );
	medianBlur ( src_gray3, src_gray2, blur_out );
  /// Canny detector
  Canny( src_gray2, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
	Canny( src_gray2, detected_edges2, lowThreshold, lowThreshold*ratio, kernel_size );
	//imshow( "Canny", detected_edges );
	//medianBlur ( detected_edges, detected_edges, blu );
	//GaussianBlur( detected_edges, detected_edges, Size( blu, blu ), 0, 0 );

	//test
	//Point pt = lookForWhitePxls(detected_edges, {0, 0});
	//cout << "ZNALEZNIONO PUNKT: " << pt << endl;
	countStdDev(detected_edges2, src_gray, width);

	cout << "Analysed pixels: " << ANALYSED_PX << endl;
}


/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
	blur_out = 1 + blur_in * 2; 
  /// Reduce noise with a kernel 3x3
  //blur( src_gray, detected_edges, Size(3,3) );
	//detected_edges_blured = detected_edges.clone();
	//GaussianBlur( src_gray, gb, Size( 1, 1 ), 0, 0 );
	medianBlur ( src_gray3, src_gray2, blur_out );
  /// Canny detector
  Canny( src_gray2, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
	Canny( src_gray2, detected_edges2, lowThreshold, lowThreshold*ratio, kernel_size );
	//imshow( "Canny", detected_edges );
	//medianBlur ( detected_edges, detected_edges, blu );
	//GaussianBlur( detected_edges, detected_edges, Size( blu, blu ), 0, 0 );

	//test
	//Point pt = lookForWhitePxls(detected_edges, {0, 0});
	//cout << "ZNALEZNIONO PUNKT: " << pt << endl;
	//countStdDev(detected_edges2, src_gray, 6);
	
	cvtColor( detected_edges, color_dst, CV_GRAY2BGR );

	cvtColor( detected_edges2, color_dst2, CV_GRAY2BGR );	
	/*
	vector<Vec4i> lines;
//threshold[, lines[, minLineLength[, maxLineGap]]]
  HoughLinesP( detected_edges, lines, 1, CV_PI/180, minLenght, maxGap, thresholdHLP );
  for( size_t i = 0; i < lines.size(); i++ )
  {
			cout << "Linia numer: " << i << endl;
			cout << "Dane linii: " << Point(lines[i][0], lines[i][1]) << Point(lines[i][2], lines[i][3]) << endl;
			//cout << "Rówananie prostej: " << i << endl;
      line( color_dst, Point(lines[i][0], lines[i][1]),
          Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 2, 8 );
  }
*/
	//namedWindow( "Source", 1 );
  //imshow( "Source", src );

  //namedWindow( "Detected Lines", WINDOW_NORMAL );
	//resize(color_dst, color_dst, Size(), 1.4, 1.4, INTER_CUBIC);
  //imshow( "Detected Lines", color_dst );
	//namedWindow( "pixels", 1 );
	//resize(color_dst2, color_dst2, Size(), 1.4, 1.4, INTER_CUBIC);
  //imshow( "pixels", color_dst2 );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);
	
  src.copyTo( dst, detected_edges);
	
	resize(dst, dst, Size(), 1.5, 1.5, INTER_CUBIC);
  imshow( window_name, src_gray2 );
 }


/** @function main */
int main( int argc, char* argv[] )
{
  /// Load an image
  src = imread( argv[1] );

  if( !src.data )
  { return -1; }

	int width = 6;
	if( argc > 2 )
	{
		width = atoi(argv[2]);
		cout << "width = " << width << endl;
	}

  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );

	//GaussianBlur( src, src, Size( 3, 3 ), 0, 0 );
	//medianBlur ( src_gray, src_gray, 5 );
  /// Convert the image to grayscale
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
	resize(src_gray, src_gray_big, Size(), 1.4, 1.4, INTER_CUBIC);
	//imshow( "Oryginal", src_gray_big );
	//szary na czarny:
	Gr2Bl(src_gray, src_gray3, 205);
	//sz na cz koniec

  /// Create a window
  /*namedWindow( window_name, WINDOW_AUTOSIZE );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
	createTrackbar( "Min Lenght:", window_name, &minLenght, 100, CannyThreshold );
	createTrackbar( "Max Gap:", window_name, &maxGap, 100, CannyThreshold );
	createTrackbar( "Threshold HLP:", window_name, &thresholdHLP, 200, CannyThreshold );
	createTrackbar( "Blur:", window_name, &blur_in, 50, CannyThreshold );*/

  /// Show the image
  //CannyThreshold(0, 0);
	callFunctions(width);

  /// Wait until user exit program by pressing a key
  //waitKey(0);

  return 0;
  }
