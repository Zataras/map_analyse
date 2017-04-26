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
	//	  (  B,  G,  R)
	Vec3b(255,  0,  0), 	//blue
	Vec3b(  0,255,  0), 	//green
	Vec3b(  0,  0,255), 	//red
	Vec3b(  0,  0,  0),	//black
	Vec3b(255,255,255),	//white
	Vec3b(205,205,205),	//grey
	Vec3b(  0,255,255),	//yellow
	Vec3b(  0,153,255),	//orange
	Vec3b(255,153,255),	//pink
	Vec3b(153,100,153)	//violet
};

//this global variable consists of vector of mean values counted for each row in series
//and can be used to visualize supposed position of obstacle's edge
//VectorOfMeanLines: <line1, line2, line3> each line: vector of mean points:<floatX, floatY>

vector<structVecOfMeanLines> VecOfMeanLines;
//int MeanLineIndex

Mat mapaRobocza, color_dst3;
Mat edgesRgbMap; //global temporary to make it avaliable for mouse callback

double resizeFactor = 10.0; 	//global to change it globaly in whole project from one place 
										//when working on different resolutions
string message;

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

//typedef vector<vector<Vec2f>> DataT;

/*
1) Count mean for each line (first index)
(2)) Draw line representing mean value for each line - has to refer to proper coordinate: x OR y (for debug)
*/
void countAndDrawMeanLines(Mat &aRsrcRgbImg)
{
	vector<Vec2f> vecOfMeanVals;
	Vec2f mean;
	//SHOW(VecOfMeanLines.size());
	for(int i=0; i<VecOfMeanLines.size(); i++)
	{
		Vec2f sum = Vec2f(0,0);
		int counter = 0;
		for(int j=0; j<VecOfMeanLines[i].meanPt.size(); j++)
		{
			cout << "VecOfMeanLines["<<i<<"].";
			cout <<"["<<j<<"] = ("<<VecOfMeanLines[i].meanPt[j]<<")\n";
			if(VecOfMeanLines[i].meanPt[j] != Vec2f(0,0))
			{
				sum += VecOfMeanLines[i].meanPt[j]; //can divide here?
				counter++;
			}
		
		}
		//mean = sum / j;
		//SHOW(vecOfMeanVals.size());
		sum = sum / counter;
		vecOfMeanVals.push_back(sum);
	}
	//vecOfMeanVals.shrink_to_fit();
	for(int j=0; j<vecOfMeanVals.size(); j++)
	{
		cout << "vecOfMeanVals["<<j<<"]";
		cout <<" = ("<<vecOfMeanVals[j]<<")\n";			
	}
	//draw mean lines
	//Img.at<Vec3b>(y, x) = dstColor;
	for(int i=0; i<VecOfMeanLines.size(); i++)
	{
		if(!VecOfMeanLines[i].meanPt.empty())
		{
			Point ptToDraw(-1,-1);
			SHOW("");
			Vec2f vBegin = *(VecOfMeanLines[i].meanPt.begin()), vEnd = *(VecOfMeanLines[i].meanPt.rbegin()); 
			SHOW("");
			SHOW(vBegin);
			SHOW(vEnd);
			int jBegin, jEnd;
			if(VecOfMeanLines[i].direction == 1)
			{
				jBegin = vBegin[0];
				jEnd 	 = vEnd[0];				
			}
			if(VecOfMeanLines[i].direction == 2)
			{
				jBegin = vBegin[1];
				jEnd = 	vEnd[1];
			}
			//check if end greater than begin
			if(jBegin > jEnd)
			{
				int jTemp = jBegin;
				jBegin = jEnd;
				jEnd 	 = jTemp;
			}
			for(int j=jBegin; j<=jEnd; j++)
			{
				//check directions
				if(VecOfMeanLines[i].direction == 1)
				{	
					ptToDraw.x = j;
					ptToDraw.y = vecOfMeanVals[i][1];
				}	
				else if(VecOfMeanLines[i].direction == 2)
				{
					ptToDraw.x = vecOfMeanVals[i][0];
					ptToDraw.y = j;
				}
				aRsrcRgbImg.at<Vec3b>(ptToDraw) = COLORS.orange;
				showResized(aRsrcRgbImg, "MeanLines", resizeFactor, 0); //debug
				//cout <<"["<<j<<"] = ("<<VecOfMeanLines[i].meanPt[j]<<")\n";
				/*if(VecOfMeanLines[i].meanPt[j] != Vec2f(0,0))
				{
					sum += VecOfMeanLines[i].meanPt[j]; //can divide here?
				}*/
			}
		}
	}
	
	/*for( vector<vector<Vec2f>>::iterator i = VecOfMeanLines.begin(); i != VecOfMeanLines.end(); i++ )
	{
	 	for( vector<Vec2f>::iterator j = (*i).begin(); j != (*i).end(); j++ )
		{
		 	Vec2f temp = *j;
		 	SHOW(temp);
		}
	}*/
}

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
	//showResized(auxRgbMap, "auxRgbMap", resizeFactor, 0); //debug
	//SHOW("after colorChangeAllRgb");
	//for(int i=1; i<=1; i+=2)
	//{
		//expose edges
	int blur_out = 3;
	lowThreshold = 70;
	
	//SHOW("1");
	
	//edgesRgbMap = auxRgbMap.clone();
	auxRgbMap.copyTo(edgesRgbMap);
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
	
	countAndDrawMeanLines(srcRgbImg);
	//here has to put comparing function with at least two Mat arguments
	
	//callFunctions(auxRgbMap, width, minLenght);
	
	//cvtColor(src, rgbImg, CV_GRAY2RGB);
	
  	/// Create a window
  	//namedWindow( window_name, WINDOW_AUTOSIZE );
  	/// Create a Trackbar for user to enter threshold
	//createTrackbar( "Min Lenght:", window_name, &minLenght, 100, CannyThreshold );
  	/// Show the image
  	//CannyThreshold(0, 0);
	/// Wait until user exit program by pressing a key
  	//waitKey(0);
	
	//destroyAllWindows(); //For a simple program, you do not really have to call these functions because all the resources and windows of the application are closed automatically by the operating system upon exit.
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
