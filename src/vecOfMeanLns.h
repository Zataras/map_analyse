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
#include "mainFun.h"

using namespace cv;
using namespace std;

/// Global variables

//it should contain points coordinates sorted by lines and containing information about direction
extern vector<structVecOfMeanPts> VecOfMeanPts;
extern string message;
extern Mat mapaRobocza, color_dst3;
extern int ANALYSED_PX;
extern double resizeFactor;
extern Mat edgesRgbMap;

//Takes pixels similar to lines and counts mean of them and draw at auxRgbMap
void createVecOfMeanLines(Mat srcRgbImgR, Mat &auxRgbMap);

//idea for variable naming convention: [Const][Argument/Local/Static][name][Reference, Pointer]
//function returns coordinates of first pixel found in specified color on specified image
//starting from specified point in horizontal direction
Point lookForSpecColPxls(Mat &aImgR, Point aPt, Vec3b aColour);

//to check if found point is not neigbour to previous point(and previous point itself)
//returns true if is not
bool checkIfNotNeighbour( Point refPt, Point checkPt );//not used??

//should help in checking pixels, //direction 1 - x++; 2 - y++; 3 - x--; 4 - y--
//looks in 4 directions, returns found point
Point checkSpecDirection( Mat &srcImg, Point actPt, Point prevPt, int maxGap, bool lookInRevDir );

//szuka kolejnego pixela nalezącego do konturu
//looking for next pixel on edge, returns its coordinates
Point findNextPixelEdge(Mat &srcImg, Point prevPt, Point actPt, bool lookInRevDir);

Point countTrueMeanInt(Mat &pixImg, Point PrevPtMod, int &counterAllOut);

float countTrueMean(Mat &aRgbEdgeMapR, Mat &aSrcRgbImgR, Point &prevPt, int &width, int currDir);
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

	/*====SCIĄGA MAT SIZE====:
	cv:Mat mat;
	int rows = mat.rows;
	int cols = mat.cols;

	cv::Size s = mat.size();
	rows = s.height;
	cols = s.width;
	*/

#endif /* !FILE_STRLINES_SEEN */
