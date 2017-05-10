/*
Functions called in main function
Code file mainFun.cpp
*/

#include "mainFun.h"

//usage e.g. cout << Color::FG_RED << "text";
/*namespace ColorFonts {
    enum Code {
        FG_RED      		= 31,
        FG_GREEN    		= 32,
        FG_BLUE     		= 34,
        FG_BLACK 	  		= 30,
        FG_YELLOW 		= 33,
        FG_MAGENTA 		= 35,
        FG_CYAN 			= 36,
        FG_LIGHT_GRAY 	= 37, 
        FG_DARK_GRAY 	= 90, 
        FG_LIGHT_RED 	= 91, 
        FG_LIGHT_GREEN 	= 92, 
        FG_LIGHT_YELLOW = 93, 
        FG_LIGHT_BLUE 	= 94, 
        FG_LIGHT_MAGENTA= 95, 
        FG_LIGHT_CYAN 	= 96, 
        FG_WHITE 			= 97,
        FG_DEFAULT  		= 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    std::ostream& operator<<(std::ostream& os, Code code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }
}*/

float countStdDev(Mat &srcImg, int width, Point startPt, Point endPt)
{
	SHOW(startPt);
	SHOW(endPt);
	int currDir = 0;
	Point diffPt = endPt - startPt;
	SHOW(diffPt);
	if(diffPt.x == 0)
		currDir = 2;
	else if (diffPt.y == 0)
		currDir = 1;
	SHOW(currDir);
	Point currPt = startPt;
	Point currPtMod;
	Point devSumVsCounter = Point(0,0); //x is sum, y is counter
	
	do
	{
		if(currDir == 1)
		{
			for( int i = 1; i<=width; i++ )
			{
				if( currPt.y + i < srcImg.rows ){ // if not exceeds maps size
					currPtMod.x = currPt.x;
					currPtMod.y = currPt.y + i;

					//srcImg.at<Vec3b>(currPtMod) = COLORS.blue;
					if(srcImg.at<Vec3b>(currPtMod) == COLORS.black)
					{
						devSumVsCounter.x += i;
						devSumVsCounter.y++;
					}
				}
		
				if( currPt.y - i >= 0 ){ // if not exceeds maps size - 0px
					currPtMod.x = currPt.x;
					currPtMod.y = currPt.y - i;
					
					//srcImg.at<Vec3b>(currPtMod) = COLORS.blue;
					if(srcImg.at<Vec3b>(currPtMod) == COLORS.black)
					{
						devSumVsCounter.x += i;
						devSumVsCounter.y++;
					}
				}
			}
			currPt.x++;
		}
		if(currDir == 2)
		{
            for(int i = 1; i<=width; i++){
				if( currPt.x + i < srcImg.cols ){
					currPtMod.x = currPt.x + i;
					currPtMod.y = currPt.y;
					
					//srcImg.at<Vec3b>(currPtMod) = COLORS.blue;
					if(srcImg.at<Vec3b>(currPtMod) == COLORS.black)
					{
						devSumVsCounter.x += i;
						devSumVsCounter.y++;
					}
				}
				if( currPt.x - i >= 0 ){
					currPtMod.x = currPt.x - i;
					currPtMod.y = currPt.y;
					
					//srcImg.at<Vec3b>(currPtMod) = COLORS.blue;
					if(srcImg.at<Vec3b>(currPtMod) == COLORS.black)
					{
						devSumVsCounter.x += i;
						devSumVsCounter.y++;
					}
				}
			}
			currPt.y++;
		}
		//SHOW(currPt);
		//SHOW(currDir);
	}while(currPt != endPt);
	SHOW(devSumVsCounter);
	float retVal = static_cast<float>(devSumVsCounter.x) / static_cast<float>(devSumVsCounter.y);
	return retVal;
}

/*
1) Count mean for each line (first index)
(2)) Draw line representing mean value for each line - has to refer to proper coordinate: x OR y (for debug)
*/
void countAndDrawMeanLines(Mat &aRsrcRgbImg)
{
	//vector<Vec2f> vecOfMeanVals;
	Vec2f mean;
    for(unsigned int i=0; i<VecOfMeanPts.size(); i++)
	{
		Vec2f sum = Vec2f(0,0);
		int counter = 0;
        for(unsigned int j=0; j<VecOfMeanPts[i].meanPt.size(); j++)
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
    for(unsigned int j=0; j<vecOfMeanVals.size(); j++)
	{
		cout << "vecOfMeanVals["<<j<<"]";
		cout <<" = ("<<vecOfMeanVals[j]<<")\n";			
	}
	//draw mean lines
    for(unsigned int i=0; i<VecOfMeanPts.size(); i++)
	{
		SHOW("");
		if(!VecOfMeanPts[i].meanPt.empty())
		{
			Point ptToDraw(-1,-1);
			//SHOW("");
			Vec2f vBegin = *(VecOfMeanPts[i].meanPt.begin()), vEnd = *(VecOfMeanPts[i].meanPt.rbegin()); 
			//SHOW("");
			//SHOW(vBegin);
			//SHOW(vEnd);
			int jBegin, jEnd;
			if(abs(VecOfMeanPts[i].direction == 1))
			{
				jBegin = vBegin[0];
				jEnd 	 = vEnd[0];				
			}
			if(abs(VecOfMeanPts[i].direction == 2))
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
				//SHOW(j);
				//check directions
				if(abs(VecOfMeanPts[i].direction) == 1)
				{	
					ptToDraw.x = j;
					ptToDraw.y = vecOfMeanVals[i][1]; //SHOW(ptToDraw.y);
				}	
				else if(abs(VecOfMeanPts[i].direction == 2))
				{
					ptToDraw.x = vecOfMeanVals[i][0]; //SHOW(ptToDraw.x);
					ptToDraw.y = j;
				}
				aRsrcRgbImg.at<Vec3b>(ptToDraw) = COLORS.orange;
                showResized(aRsrcRgbImg, "MeanLines", resizeFactor, 10); //debug VISU
			}
		}
	}
	
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
