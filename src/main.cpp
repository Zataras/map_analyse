/* file main.cpp
Analyses maps created with ROS
Aim of this project is to define accuracy and recurrence of used mapping method
*/	

#include "main.h"

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

	int width = 6;
	if( argc > 2 )
	{
		width = atoi(argv[2]);
		cout << "width = " << width << endl;
	}

	string ty =  type2str( srcRgbImg.type() );
	message = "Matrix: " + ty + " " + to_string(srcRgbImg.cols) + "x" + to_string(srcRgbImg.rows);
	SHOW(message);

	Mat auxRgbMap = srcRgbImg.clone();
	
	//Gray pixels to black:
	colorChangeAllRgb(auxRgbMap, COLORS.black, COLORS.grey);

	int blur_out = 3;
	lowThreshold = 70;
	
	auxRgbMap.copyTo(edgesRgbMap);

	medianBlur ( auxRgbMap, auxRgbMap, blur_out );
	Mat greyMap;
	cvtColor(auxRgbMap, greyMap, CV_RGB2GRAY);
	Canny( greyMap, edgesRgbMap, lowThreshold, lowThreshold*ratio, kernel_size );
	
	//that map will be compared with oryginal
	bitwise_not ( edgesRgbMap, edgesRgbMap );
	cvtColor(edgesRgbMap, edgesRgbMap, CV_GRAY2RGB);
	
	namedWindow("debug window", WINDOW_AUTOSIZE);//WINDOW_AUTOSIZE);
	setMouseCallback("debug window", onMouse, NULL);
	
	createVecOfMeanLines(srcRgbImg, edgesRgbMap);
	
	countAndDrawMeanLines(srcRgbImg);
	
	Point startPt, endPt;	
	float stdDev = countStdDev(srcRgbImg, 10, startPt, endPt);
	
	message = "at the end";
	SHOW(message);//debug
	
	destroyAllWindows(); //For a simple program, you do not really have to call these functions because all the resources and windows of the application are closed automatically by the operating system upon exit.
  	return 0;
}


/// Create a window
//namedWindow( window_name, WINDOW_AUTOSIZE );
/// Create a Trackbar for user to enter threshold
//createTrackbar( "Min Lenght:", window_name, &minLenght, 100, CannyThreshold );
/// Show the image
//CannyThreshold(0, 0);
/// Wait until user exit program by pressing a key
//waitKey(0);

/*==OLD==:

void callFunctions(Mat &aRgbMapR, int width, int minLenght)
{
	
  	/// Reduce noise with a kernel 3x3
  	//blur( src_gray, detected_edges, Size(3,3) );
	//GaussianBlur( src_gray, gb, Size( 1, 1 ), 0, 0 );
	//medianBlur ( src_gray3, src_gray2, blur_out );
}
*/
/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
/*void CannyThreshold(int, void*)
{		
	//valuesChange.flip();
}*/

