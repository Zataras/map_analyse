/* File strLines.  */
#ifndef FILE_STRLINES_SEEN
#define FILE_STRLINES_SEEN
/*
	deklaracje typów
	deklaracje funkcji
	deklaracje struktur
	deklaracje klas
	deklarację ewentualnych zmiennych globalnych 
	
	Dodatkowo dołączamy do niego niezbędne pliki
	nagłówkowe, jakie będą wykorzystywane przez daną
	bibliotekę. W pliku nagłówkowym nie umieszczamy
   natomiast bloków funkcji. Można powiedzieć po
 	prostu, że w pliku nagłówkowym umieszczamy
  	wszystko oprócz bloków funkcji. 
*/

	/*====SCIĄGA MAT SIZE====:
	cv:Mat mat;
	int rows = mat.rows;
	int cols = mat.cols;

	cv::Size s = mat.size();
	rows = s.height;
	cols = s.width;
	*/

//#include "opencv2/imgproc.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgcodecs/imgcodecs.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <algorithm> // In C++, to set them all to -1, you can use something like std::fill_n (from <algorithm>): 
							// std::fill_n(array, 100, -1);
//#include <unistd.h>//usleep
#include <string> //to_string
//#include <ostream>//font colors
#include "usefulFun.h"


using namespace cv;
using namespace std;


struct structVecOfMeanLines
{
	vector<Vec2f> meanPt;
	int direction;
	bool operator!= (const structVecOfMeanLines s1)
	{
		if(meanPt == s1.meanPt && direction == s1.direction)
			return 0;
		else
			return 1;
	}
	
};

/// Global variables
const ColorsName COLORS;

//it should contain points coordinates sorted by lines and containing information about direction
extern vector<structVecOfMeanLines> VecOfMeanLines;

extern string message;

extern Mat mapaRobocza, color_dst3;
extern int ANALYSED_PX;
extern double resizeFactor;
extern Mat edgesRgbMap;

void onMouse(int evt, int x, int y, int flags, void* param);

//changes one color pixels to another to avoid double parallel lines:
//takes references and changes to dst colour
void colorChangeAllRgb(Mat &Img, Vec3b srcColor, Vec3b dstColor);

//Takes pixels similar to lines and counts mean of them and draw at auxRgbMap
void createMapOfMeanLines(Mat &srcRgbImgR, Mat &auxRgbMap);

//idea for variable naming convention: [Const][Argument/Local/Static][name][Reference, Pointer]
//function returns coordinates of first pixel found in specified color on specified image
//starting from specified point in horizontal direction
Point lookForSpecColPxls(Mat &aImgR, Point aPt, Vec3b aColour);

//to check if found point is not neigbour to previous point(and previous point itself)
//returns true if is not
bool checkIfNotNeighbour( const Mat &srcImg, Point refPt, Point checkPt );//not used??

//should help in checking pixels, //direction 1 - x++; 2 - y++; 3 - x--; 4 - y--
//looks in 4 directions, returns found point
Point checkSpecDirection( Mat &srcImg, Point actPt, Point prevPt, int maxGap, bool lookInRevDir );

//szuka kolejnego pixela nalezącego do konturu
//looking for next pixel on edge, returns its coordinates
Point findNextPixelEdge(Mat &srcImg, Point prevPt, Point actPt, bool lookInRevDir);


Point countTrueMeanInt(Mat &pixImg, Mat &aImgRgbEdgeR, /*Point (&pointsArray)[],*/ int pointsArraySize, Point PrevPtMod, int &counterAllOut, /*bool fOrS,*/ int currWidth, int &maxWidth);

float countTrueMean(Mat &aRgbEdgeMapR, Mat &aSrcRgbImgR, Point &prevPt, Point &actPt, int &width, int currDir);
//=======================countTrueMean END=================================

//if previous, actual and next points difference points that they
// may be lying on straight line then take this point into consideration in 
// counting accuracy
//At this time only vertical and horizontal lines are taken under
// consideration
//Returns value meaning direction of line
// 1 - x(V); 2 - y(H); cross directions later todo
// 0 - not defined;
int checkDirection(Point prevPt, Point actPt, Point nextPt);

//Poprawic sposob obliczania dokladnosci!!!!
//dokladnosc to odchylenie danego pomiaru od spodziewanej wartosci referencyjnej
//czyli moze dla kazdego pixela osobno nalezy wyznaczyc dokladnosc?

//zwraca dokladnosc w procentach
//okresla ile pixeli w najblizszym otoczeniu linii nie nalezy do linii
//okreslic jako linie tylko dlugie proste i tylko je analizowac
//wyszlo probnie ze odchylenie st. = 0.0878308 
int countStdDev(Mat &edgeImg, Mat &pixImg, int otoczenie);

#endif /* !FILE_STRLINES_SEEN */
