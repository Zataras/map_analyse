/* file main.cpp
Analyses maps created with ROS
Aim of this project is to define accuracy and recurrence of used mapping method
*/	

#include "main.h"

/** @function main */
int main( int argc, char* argv[] )
{
	//cout << "Press 'c' key to continue";
	
    //int minLenght = 35;
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

	//for debug:
	//namedWindow("debug window", WINDOW_AUTOSIZE);

	Mat auxRgbMap = srcRgbImg.clone();
	Mat srcRgbImgCloned = srcRgbImg.clone();
	
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
	
	createVecOfMeanLines(srcRgbImgCloned, edgesRgbMap);
	
	//remove last element of VecOfMeanPts
	//because it's being added "in case"
	VecOfMeanPts.pop_back();
	
	countAndDrawMeanLines(srcRgbImg);
	message = "after draw";
	SHOW(message);
	Point startPt, endPt;
	SHOW(VecOfMeanPts.size());
	Vec2f sumStdDevVsCount;
    for(int i=0; i<static_cast<int>(VecOfMeanPts.size()); i++)
	{
		SHOW("");
		startPt =  static_cast<Point>(*(VecOfMeanPts[i].meanPt.begin()));
		endPt = static_cast<Point>(*(VecOfMeanPts[i].meanPt.rbegin()));
		//write mean value
		if(VecOfMeanPts[i].direction == 1)
		{
			startPt.y = vecOfMeanVals[i][1];
			endPt.y   = vecOfMeanVals[i][1];
			//check if end greater than begin
			if(startPt.x > endPt.x)
			{
				int temp = startPt.x;
				startPt.x = endPt.x;
				endPt.x 	 = temp;
			}
		}
		else if(VecOfMeanPts[i].direction == 2)
		{
            startPt.x = static_cast<int>(vecOfMeanVals[i][0]);
            endPt.x   = static_cast<int>(vecOfMeanVals[i][0]);
			//check if end greater than begin
			if(startPt.y > endPt.y)
			{
				int temp = startPt.y;
				startPt.y = endPt.y;
				endPt.y 	 = temp;
			}
		}
		
		float stdDev = countStdDev(srcRgbImg, 10, startPt, endPt);
		SHOW(stdDev);
		sumStdDevVsCount[0] += stdDev;
		sumStdDevVsCount[1]++;
	}
	float finalStdDev = sumStdDevVsCount[0] / sumStdDevVsCount[1];
	
	SHOW(finalStdDev);
	
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

/*115: message = at the end
*** Error in `/home/mitat/mag/map_analyse/cmake-Debug-x86_64-gnu-linux/launcher': free(): invalid next size (normal): 0x0000000001456570 ***
======= Backtrace: =========
/lib64/libc.so.6(+0x791fb)[0x7fc4983c61fb]
/lib64/libc.so.6(+0x8288a)[0x7fc4983cf88a]
/lib64/libc.so.6(cfree+0x4c)[0x7fc4983d32bc]
/lib64/libopencv_core.so.3.1(_ZNK2cv12MatAllocator5unmapEPNS_8UMatDataE+0x99)[0x7fc4990eda79]
/lib64/libopencv_core.so.3.1(_ZN2cv3Mat10deallocateEv+0x1c)[0x7fc4990ee65c]
/home/mitat/mag/map_analyse/cmake-Debug-x86_64-gnu-linux/launcher(_ZN2cv3Mat7releaseEv+0x4f)[0x40b1b7]
/home/mitat/mag/map_analyse/cmake-Debug-x86_64-gnu-linux/launcher(_ZN2cv3MatD1Ev+0x18)[0x40b0be]
/lib64/libc.so.6(+0x3a410)[0x7fc498387410]
/lib64/libc.so.6(+0x3a46a)[0x7fc49838746a]
/lib64/libc.so.6(__libc_start_main+0xf8)[0x7fc49836d408]
/home/mitat/mag/map_analyse/cmake-Debug-x86_64-gnu-linux/launcher(_start+0x2a)[0x409baa]
======= Memory map: ========
00400000-00417000 r-xp 00000000 fd:02 15862347                           /home/mitat/mag/map_analyse/cmake-Debug-x86_64-gnu-linux/launcher
00616000-00617000 r--p 00016000 fd:02 15862347                           /home/mitat/mag/map_analyse/cmake-Debug-x86_64-gnu-linux/launcher
00617000-00618000 rw-p 00017000 fd:02 15862347                           /home/mitat/mag/map_analyse/cmake-Debug-x86_64-gnu-linux/launcher
010cb000-01d79000 rw-p 00000000 00:00 0                                  [heap]
7fc41c000000-7fc41c021000 rw-p 00000000 00:00 0
7fc41c021000-7fc420000000 ---p 00000000 00:00 0
7fc4235ed000-7fc4235f7000 r-xp 00000000 fd:00 532513                     /usr/lib64/libnss_files-2.24.so
7fc4235f7000-7fc4237f7000 ---p 0000a000 fd:00 532513                     /usr/lib64/libnss_files-2.24.so
7fc4237f7000-7fc4237f8000 r--p 0000a000 fd:00 532513                     /usr/lib64/libnss_files-2.24.so
7fc4237f8000-7fc4237f9000 rw-p 0000b000 fd:00 532513                     /usr/lib64/libnss_files-2.24.so
7fc4237f9000-7fc4237ff000 rw-p 00000000 00:00 0
7fc4237ff000-7fc423800000 ---p 00000000 00:00 0
7fc423800000-7fc424000000 rw-p 00000000 00:00 */
