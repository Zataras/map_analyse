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

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
//#include "opencv2/imgcodecs/imgcodecs.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <algorithm> // In C++, to set them all to -1, you can use something like std::fill_n (from <algorithm>): 
							// std::fill_n(array, 100, -1);
//#include <unistd.h>//usleep

using namespace cv;
using namespace std;

/// Global variables
extern Mat src, src_gray, gb, src_gray2, src_gray3, src_gray_big;
extern Mat dst, detected_edges, detected_edges2, color_dst, color_dst2, color_dst3, mapaRobocza;
extern int kolor;
extern int ANALYSED_PX;

//szary na czarny:
//changes grey pixels to black to avoid double parallel lines:
//takes references and changes dst colours
//third arg is colour of grey
void Gr2Bl(Mat &srcImg, Mat &dstImg, int grey);

//podaj początkowy punkt, poszukiwanie inkrementuje x i y
//pass start point, searches and increments x and y coordinates
//returns found white point coordinates
Point lookForWhitePxls(Mat &srcImg, Point inPt);

//to check if found point is not neigbour to previous point(and previous point itself)
//returns true if is not
bool checkIfNotNeighbour( const Mat &srcImg, Point refPt, Point checkPt );//not used??

//should help in checking pixels, //direction 1 - x++; 2 - y++; 3 - x--; 4 - y--
//looks in 4 directions, returns found point
Point checkSpecDirection( Mat &srcImg, Point actPt, Point prevPt, int maxGap, bool lookInRevDir );

//szuka kolejnego pixela nalezącego do konturu
//looking for next pixel on edge, returns its coordinates
Point findNextPixelEdge(Mat &srcImg, Mat &pixImg, Point prevPt, Point actPt, bool lookInRevDir);


Point countTrueMeanInt(Mat &pixImg, /*Point (&pointsArray)[],*/ int pointsArraySize, Point PrevPtMod, int &counterAllOut, /*bool fOrS,*/ int currWidth, int &maxWidth);

float countTrueMean(Mat &pixImg, Point &prevPt, Point &actPt, int &width, int currDir);
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
	
