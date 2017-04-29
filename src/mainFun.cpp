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

/*
1) Count mean for each line (first index)
(2)) Draw line representing mean value for each line - has to refer to proper coordinate: x OR y (for debug)
*/
void countAndDrawMeanLines(Mat &aRsrcRgbImg)
{
	vector<Vec2f> vecOfMeanVals;
	Vec2f mean;
	for(int i=0; i<VecOfMeanPts.size(); i++)
	{
		Vec2f sum = Vec2f(0,0);
		int counter = 0;
		for(int j=0; j<VecOfMeanPts[i].meanPt.size(); j++)
		{
			cout << "VecOfMeanPts["<<i<<"].";
			cout <<"["<<j<<"] = ("<<VecOfMeanPts[i].meanPt[j]<<"); dir = "<<VecOfMeanPts[i].direction<< "\n";
			if(VecOfMeanPts[i].meanPt[j] != Vec2f(0,0))
			{
				sum += VecOfMeanPts[i].meanPt[j]; //can divide here?
				counter++;
			}
		
		}

		if(counter)
		{
			sum = sum / counter;
			vecOfMeanVals.push_back(sum);
		}
	}
	//vecOfMeanVals.shrink_to_fit();
	for(int j=0; j<vecOfMeanVals.size(); j++)
	{
		cout << "vecOfMeanVals["<<j<<"]";
		cout <<" = ("<<vecOfMeanVals[j]<<")\n";			
	}
	//draw mean lines
	for(int i=0; i<VecOfMeanPts.size(); i++)
	{
		if(!VecOfMeanPts[i].meanPt.empty())
		{
			Point ptToDraw(-1,-1);
			SHOW("");
			Vec2f vBegin = *(VecOfMeanPts[i].meanPt.begin()), vEnd = *(VecOfMeanPts[i].meanPt.rbegin()); 
			SHOW("");
			SHOW(vBegin);
			SHOW(vEnd);
			int jBegin, jEnd;
			if(VecOfMeanPts[i].direction == 1)
			{
				jBegin = vBegin[0];
				jEnd 	 = vEnd[0];				
			}
			if(VecOfMeanPts[i].direction == 2)
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
			SHOW(i);
			for(int j=jBegin; j<=jEnd; j++)
			{
				//check directions
				if(VecOfMeanPts[i].direction == 1)
				{	
					ptToDraw.x = j;
					ptToDraw.y = vecOfMeanVals[i][1]; //SHOW(ptToDraw.y);
				}	
				else if(VecOfMeanPts[i].direction == 2)
				{
					ptToDraw.x = vecOfMeanVals[i][0]; //SHOW(ptToDraw.x);
					ptToDraw.y = j;
				}
				aRsrcRgbImg.at<Vec3b>(ptToDraw) = COLORS.orange;
				showResized(aRsrcRgbImg, "MeanLines", resizeFactor, 1); //debug
			}
		}
	}
	//!! TODO:
	//Zmodyfikowac juz istniejace funkcje badajace otoczenie linii, tak zeby mozna je  bylo
	//ponownie wykonac wzdluz wyznaczonej sredniej linii i policzyc odchylenie pixeli
	
	/*for( vector<vector<Vec2f>>::iterator i = VecOfMeanPts.begin(); i != VecOfMeanPts.end(); i++ )
	{
	 	for( vector<Vec2f>::iterator j = (*i).begin(); j != (*i).end(); j++ )
		{
		 	Vec2f temp = *j;
		 	SHOW(temp);
		}
	}*/
}

void onMouse(int evt, int x, int y, int flags, void* param) {
    if(evt == EVENT_LBUTTONDOWN) {
        //Point* ptPtr = (Point*)param;
        Point Pt = Point(x,y);
        Pt.x = round(Pt.x / resizeFactor);
        Pt.y = round(Pt.y / resizeFactor);
        Vec3b keptColor = edgesRgbMap.at<Vec3b>(Pt);
        edgesRgbMap.at<Vec3b>(Pt) = COLORS.violet;
        showResized(edgesRgbMap, "debug window", resizeFactor, 300);
        edgesRgbMap.at<Vec3b>(Pt) = keptColor;
        showResized(edgesRgbMap, "debug window", resizeFactor, 1);
        
        cout << "Clicked on " << Pt << " coloured " << keptColor << "\n";
    }
}

//change grey pixels to black to avoid double parallel lines:
//takes references and changes dst colours
//third arg is colour of grey
void colorChangeAllRgb(Mat &Img, Vec3b srcColor, Vec3b dstColor)
{
	for(int x = 0; x < Img.cols; x++){
		//SHOW(x);
		for(int y = 0; y < Img.rows; y++){
			//SHOW(y);
			//Vec3b pxColor = Img.at<Vec3b>(x, y);
			if( Img.at<Vec3b>(y, x) == srcColor)//!!
				Img.at<Vec3b>(y, x) = dstColor;	//!!
			//showResized(Img, "Img", resizeFactor, 1);
		}
	}
}
