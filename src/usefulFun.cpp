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
//! bla2
void showResized(const Mat &srcImg, const string& winname, double factor, int timeMs)
{
    //int rows = srcImg.rows*factor;
    Mat bigImg;//(rows,srcImg.cols, CV_8UC3, Scalar(0,0,0));
	//INTER_NEAREST do not fuzz pixels
    resize(srcImg, bigImg, Size(), factor, factor, INTER_NEAREST);

    namedWindow(winname, WINDOW_AUTOSIZE);
    imshow(winname, bigImg);
    waitKey(timeMs);
}

