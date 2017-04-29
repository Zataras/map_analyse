/*
Functions called in main function
Code file mainFun.cpp
*/

#include "mainFun.h"

float countStdDev(Mat &srcImg, int width, Point startPt, Point endPt)
{
	/*if(currDir == 1)//( diffPt.x != 0 )
	{
		for( int i = 1; i<=width; i++ )
		{
			if( prevPt.y + i < aRgbEdgeMapR.rows ){ // if not exceeds maps size
				PrevPtMod.x = prevPt.x;
				PrevPtMod.y = prevPt.y + i;
				cout << ColorFonts::FG_DARK_GRAY; SHOW(""); cout << ColorFonts::FG_DEFAULT;
				aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.blue;
				pointsArray[i] = countTrueMeanInt(aSrcRgbImgR, aRgbEdgeMapR, pointsArraySize, PrevPtMod, counterAllOut, i, maxWidth);
			}
		
			if( prevPt.y - i >= 0 ){ // if not exceeds maps size - 0px
				PrevPtMod.x = prevPt.x;
				PrevPtMod.y = prevPt.y - i;
				cout << ColorFonts::FG_DARK_GRAY; SHOW(""); cout << ColorFonts::FG_DEFAULT;
				aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.blue;
				pointsArray[pointsArraySize / 2 + i] = countTrueMeanInt(aSrcRgbImgR, aRgbEdgeMapR, pointsArraySize, PrevPtMod, counterAllOut, i, maxWidth);
			}
		}
	}
	if(currDir == 2)
	{
		for(int i = 1; i<=width; i++){
			if( prevPt.x + i < aRgbEdgeMapR.cols ){
				PrevPtMod.x = prevPt.x + i;
				PrevPtMod.y = prevPt.y;
				cout << ColorFonts::FG_DARK_GRAY; SHOW(""); cout << ColorFonts::FG_DEFAULT;
				aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.blue;
				pointsArray[i] = countTrueMeanInt(aSrcRgbImgR, aRgbEdgeMapR, pointsArraySize, PrevPtMod, counterAllOut, i, maxWidth);
			}
			if( prevPt.x - i >= 0 ){
				PrevPtMod.x = prevPt.x - i;
				PrevPtMod.y = prevPt.y;
				cout << ColorFonts::FG_DARK_GRAY; SHOW(""); cout << ColorFonts::FG_DEFAULT;
				aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.blue;
				pointsArray[pointsArraySize / 2 + i] = countTrueMeanInt(aSrcRgbImgR, aRgbEdgeMapR, pointsArraySize, PrevPtMod, counterAllOut, i, maxWidth);
			}
		}
	}*/
}
