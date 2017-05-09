/* File usefulFun.cpp  */

#include "usefulFun.h"

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

void showResized(const Mat &srcImg, const string& winname, double factor, int timeMs)
{
	Mat bigImg;
	//INTER_NEAREST do not fuzz pixels
	resize(srcImg, bigImg, Size(), factor, factor, INTER_NEAREST);SHOW("");

	namedWindow(winname, WINDOW_AUTOSIZE);SHOW("");
	imshow(winname, bigImg);SHOW("");
	waitKey(timeMs);SHOW("");
}
