/*
Functions called in main function
Header file mainFun.h
*/

#ifndef FILE_MAINFUN_SEEN
#define FILE_MAINFUN_SEEN

#include <iostream> //cout

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "usefulFun.h"

using namespace cv;
using namespace std;

struct structVecOfMeanPts
{
	vector<Vec2f> meanPt;
	int direction;
	bool operator!= (const structVecOfMeanPts s1)
	{
		if(meanPt == s1.meanPt && direction == s1.direction)
			return 0;
		else
			return 1;
	}
};

extern double resizeFactor;
extern Mat edgesRgbMap;

extern const ColorsName COLORS;
extern vector<structVecOfMeanPts> VecOfMeanPts;



//returns value of standard deviation in pixels
//arguments: Mat with only black and white pixels
//		width - how far from line to check
//Start and end point of mean straight line
float countStdDev(Mat &srcImg, int width, Point startPt, Point endPt);

/*
1) Count mean for each line (first index)
(2)) Draw line representing mean value for each line - has to refer to proper coordinate: x OR y (for debug)
*/
void countAndDrawMeanLines(Mat &aRsrcRgbImg);

void onMouse(int evt, int x, int y, int flags, void* param);

//changes one color pixels to another to avoid double parallel lines:
//takes references and changes to dst colour
void colorChangeAllRgb(Mat &Img, Vec3b srcColor, Vec3b dstColor);


#endif /* !FILE_MAINFUN_SEEN */
