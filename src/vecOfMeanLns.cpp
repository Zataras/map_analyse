/*
	W plikach z takim rozszerzeniem umieszczamy tylko 
	i wyłącznie definicje funkcji, czyli nazwę funkcji 
	razem z jej ciałem (czyli blokiem funkcji). 
*/

#include "vecOfMeanLns.h"

//usage e.g. cout << Color::FG_RED << "text";
namespace ColorFonts {
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
}

void createVecOfMeanLines(Mat aSrcRgbImgR, Mat &aAuxRgbMap)
{
	//---------------------------------some variables--------------------------------------
    Point pt(0, 0), diffPt;
	bool minLineLenReached, lookInRevDir;
	int lineLength, direction, currDir, diffSum, dirChangeCount, reachedLen, linesCounter;
	int diffSumLimit = 3; //limit for change of whole coordinate
	int lineLenghtMin = 45; 
	int dirChangeLimit = 4;//limit for changing direction of on pixel after another
	bool allAnalysed = false;
    int strCount = 0, strCountRes = 3;
	structVecOfMeanPts vecOfVec2f;
	vecOfVec2f.direction = -1;
	//SHOW(vecOfVec2f.direction);
	//waitKey(0);
	VecOfMeanPts.push_back(vecOfVec2f);
	
	//----------------------loop until reaches map's last pixel-----------------------------
    while((pt.x  < (aSrcRgbImgR.cols -1) | pt.y < (aSrcRgbImgR.rows -1)) & !allAnalysed)
	{	
		//counting any line
		linesCounter++;
		//---------initializing every cycle-------------
		lineLength = 0; reachedLen = 0;
		dirChangeCount = 0; lineLength = 0; diffSum = 0;
		lookInRevDir = false; minLineLenReached = false;
        currDir = 0;
		//----------------debug-------------------
        //cout << __LINE__ << ": New cycle" <<endl;
		
		//Add new empty element to VecOfMeanPts for edge which will be found next
		
		//SHOW(" ");
		//VecOfMeanPts.push_back(vecOfVec2f);//problem - adding also when line not accepted
		//SHOW(" ");		
		//------Looking for any black pixel to start with-----
        pt = lookForSpecColPxls(aAuxRgbMap, pt, COLORS.black);
		//SHOW(pt);
		if(pt.x == -1)
		{
			allAnalysed = true;//doesnt work
			SHOW(allAnalysed);
		}
		//----------initializing points------------
		Point nextPt = pt, actPt = pt, prevPt = pt;

		if(*(VecOfMeanPts.rbegin()) != vecOfVec2f)
			VecOfMeanPts.push_back(vecOfVec2f);

		//===while there is still any not analysed pixel in current series===
		while( nextPt.x != -1 )
		{
            showResized(aAuxRgbMap, "debug window", resizeFactor, 1); //debug //!! VISU
			//update points values
            if( !lookInRevDir | (lineLength != 0) )
			{
				prevPt = actPt;
				actPt = nextPt;
			}
			else 
			{
				if( lineLength == 0 )
				{
					Point tempPt = nextPt;			
					//nextPt = prevPt; 
					//prevPt = tempPt;
				}			
			}
			//message = "before";SHOW(message);
			nextPt = findNextPixelEdge(aAuxRgbMap, prevPt, actPt, lookInRevDir);
			//message = "after";SHOW(message);
            if((nextPt.x == -1) & ~minLineLenReached)
			{
                // message = "next Pixel on Edge not found";
                //SHOW(message);
			}
            /*if(nextPt.x == -1 & lineLength == 0)
			{
				
			}*/
            //showResized(aAuxRgbMap, "debug window", resizeFactor, 1); //debug	!!
			
			//cout << __LINE__ << "points are[PAN]: " << prevPt << actPt << nextPt << endl;
			
			lineLength++;
			//------------------------------------------
			//check current direction of last 3 pixels
			direction = checkDirection( prevPt, actPt, nextPt );
			//cout << __LINE__ << ": Dir is " << direction << endl;
			
            static Point savedPt;
            //save coodinate after 5 pixels and count diff from this point
            //if(lineLength == 5)
            //    savedPt = actPt;

			//count whole coordinate change if direction is opposite to currently analysed
			//to check if limit is not exceeded
            //if(lineLength<5)
                diffPt = actPt - prevPt;
            //else
            //    diffPt = actPt - savedPt;
            if(actPt != prevPt)
            {
                if( abs(currDir) == 1)
                {
                    diffSum += diffPt.y;
                    if(!diffPt.y)
                        strCount++;
                    else
                        strCount = 0;
                }
                if( abs(currDir) == 2)
                {
                    diffSum += diffPt.x;
                    if(!diffPt.x)
                        strCount++;
                    else
                        strCount = 0;
                }
            }
            //Reset diffSum if line is straight for more than strCountRes pixels
            //if(strCount >= strCountRes)
            //    diffSum = 0;
            //SHOW(diffSum);
			//save first considered currently direction as refernece
            if( currDir == 0 & direction != 0 )
			{
				currDir = direction; 
                /*cout << ColorFonts::FG_BLUE; SHOW(currDir);
                cout << ColorFonts::FG_DEFAULT;*/
			}
            //SHOW(direction);
			//if current direction is not equal to currenly analysed, increase counter value
			//if is reset value
			if( abs(direction) != currDir )
				dirChangeCount++;
			else
				dirChangeCount = 0;
			
			//SHOW(minLineLenReached);
			//set true if reached
            if((lineLength > lineLenghtMin) & (~lookInRevDir) & (~minLineLenReached))
			{
				minLineLenReached = true;
				cout << __LINE__ << ": Line accepted" << endl;
				
			}		
			
			//if sum of direction changes or whole coordinate change limit is exceeded
			//then check if line has reached minimum length
			//is yes it will be further analysed
			//SHOW(dirChangeCount);
			if ( dirChangeCount > dirChangeLimit | abs(diffSum) > diffSumLimit)
			{
				//currDir = -1;
				diffSum = 0;
				if( minLineLenReached & ~lookInRevDir)
				{
					lookInRevDir = true;
					dirChangeCount = -1;
					reachedLen = lineLength;
					message = "Direction changed and min len reached"; 
					SHOW(message);
					SHOW(lineLength);
					
				}
				else if (~minLineLenReached)
				{
					//if not set -1 and this series will be ommited
                    //message = "dirChangeLimit exceeded at length " + to_string(lineLength);
                    //SHOW(message);
					
					nextPt.x = -1;
					//break;
				}
				lineLength = 0;
			}
			
			if( lookInRevDir ) //if accepted to analyse
			{
                int otoczenie = 4; //!! width
                countTrueMean(aAuxRgbMap, aSrcRgbImgR, prevPt, otoczenie, abs(currDir));
			}
			
			//if next point not found
			if (nextPt.x == -1)
			{
				if( minLineLenReached )
				{
					lookInRevDir = true;
					minLineLenReached = false;
					dirChangeCount = -1;
					diffSum = 0;
					reachedLen = lineLength;
					message = "Next point not found, but min len reached";
					SHOW(message); 
					lineLength = 0;
					nextPt.x = -2;//to avoid starting new cycle
					Point tmpPt = prevPt;
					//nextPt == -1;
					prevPt = actPt;
					actPt = tmpPt;
				}
				else
				{
                    //message = "Next point not found!";
                    //SHOW(message);
                    //showResized(aAuxRgbMap, "debug window", resizeFactor, 1); //debug //!! VISU
				}
			}
			
			//If going backwards measured forward length is reached then finish analying this series.
			//Maybe it's bad idea? maybe should follow until doesn't fit previous condiditons
			//like sum of direction changes or whole coordinate change limit is exceed 
            /*if( lookInRevDir & (lineLength >= (reachedLen - 2)) )
			{
				nextPt.x = -1;
				ileDlugich++;
			}*/
				
			
		}
	}
	message = "Out of while loop";
	SHOW(message);	
}

//idea for variable naming convention: [Argument/Local/Static][name][Reference, Pointer]
Point lookForSpecColPxls(Mat &aImgR, Point aPt, Vec3b aColour)
{
    Point maxSize(aImgR.cols - 1, aImgR.rows - 1 );
	bool stop = false;
	Point pt = aPt;
    Vec3b pxColour;
    while((pt.x < maxSize.x | pt.y < maxSize.y) & (stop == false)){
        while(pt.x < maxSize.x & stop == false){
            pxColour = aImgR.at<Vec3b>(pt);
			//SHOW(pt);
			if( pxColour == aColour)
			{
				stop = true;
				//aImgR.at<Vec3b>(pt) = COLORS.green; //for debug
			}

			if( !stop )			
				pt.x++;
		}
        if( !stop & ((pt.y ) < maxSize.y) )
  		{		
			pt.x = 0;
			pt.y++;
		}
	}
	//305: pt = [88, 72] Matrix: 8UC3 89x73
	// if not found return -1,-1
	if(!stop)
		return Point(-1,-1);
	else
		return pt;
}

//should help in checking pixels, //direction 1 - x++; 2 - y++; 3 - x--; 4 - y--
Point checkSpecDirection( Mat &srcImg, Vec3b wantedColor, Point prevPt, Point actPt, int maxGap, bool lookInRevDir )// pt = point
{
	static bool prevRevDir;
	Point foundPt( -1, -1 ), modPt; 

    bool found = false;
	int direction;

	int rad = 1;
    while( (rad <= maxGap) & !found )//every loop increases radius
	{	
		int mG = 1;
        while( (mG <= ( maxGap + 1 )) & !found )//every loop cheks lower priority pixels
		{
			direction = 1;
            while ( (direction <= 4) & !found )
			{
				switch( direction )
				{
					case 1:
						modPt = actPt + Point( +rad, +(mG - 1) );//rad - radius; mG - Max Gap?
					break;
					case 2:
						modPt = actPt + Point( -(mG - 1), +rad );
					break;
					case 3:
						modPt = actPt + Point( -rad, -(mG - 1) );
					break;
					case 4:
						modPt = actPt + Point( +(mG - 1), -rad );
					break;
				}
				//cout << __LINE__ << ": at "<< modPt << " found color is "<< srcImg.at<Vec3b>(modPt) << endl;
				//added black always to allow on rev to analize longer series
				//SHOW(modPt);
                if(((modPt.x < srcImg.cols) & (modPt.y < srcImg.rows)) & ((modPt != prevPt) | (prevRevDir != lookInRevDir)))
				{
                    if((srcImg.at<Vec3b>(modPt) == wantedColor))
					{	
						foundPt = modPt;
						found = true;
					}
				}
				direction++;
			}
			mG++;
		}
		rad++;
	}
	prevRevDir = lookInRevDir;
    if(foundPt == Point(66, 5))
            SHOW(foundPt);
	return foundPt;
}

//szuka kolejnego pixela nalezącego do konturu
Point findNextPixelEdge(Mat &aImgR, Point prevPt, Point actPt, bool lookInRevDir)
{
	Point nextPt(-1,-1);
    static bool prevRevDir;
	//cout << "In findNextPixelEdge" << endl;
    Vec3b wantedColor;
    if(lookInRevDir | ((prevRevDir == 0) & (lookInRevDir == 1)))
    {
        wantedColor = COLORS.red;
        //wantedColor2 = COLORS.black;
    }
    else
    {
        wantedColor = COLORS.black;
        //wantedColor2 = COLORS.black;
    }
    nextPt = checkSpecDirection( aImgR, wantedColor, prevPt, actPt, 2, lookInRevDir );
    if(nextPt == Point(-1,-1))
    {
        nextPt = checkSpecDirection( aImgR, COLORS.black, prevPt, actPt, 2, lookInRevDir);
    }
	//cout << __LINE__ << ": Next point is " << nextPt << endl;

	if( !lookInRevDir /*& nextPt != Point(-1, -1)*/)
	{
		//message = "/*Prev to*/ ["+to_string(prevPt.x)+", "+to_string(prevPt.y)+"] coloured red";
		//SHOW(prevPt);SHOW(actPt);SHOW(nextPt);
		aImgR.at<Vec3b>(prevPt) = COLORS.red; //for debug
	}
	else if(nextPt != Point(-1, -1))
	{
		aImgR.at<Vec3b>(prevPt) = COLORS.grey; //for debug
    }
    showResized(aImgR, "debug window", resizeFactor, 1);
    prevRevDir = lookInRevDir;
	return nextPt;
}

//to check if found point is not neigbour to previous point(and previous point itself)
//returns true if is not
bool checkIfNotNeighbour( Point refPt, Point checkPt )
{
	bool retVal = false;
	Point diffPt = refPt - checkPt;
	if( refPt != checkPt )	
		if( abs(diffPt.x) != 1 )
			if( abs(diffPt.y) != 1 )
				retVal = true;
	return retVal;
}

Point countTrueMeanInt(Mat &aSrcRgbImgR, Point PrevPtMod, int &counterAllOut)
{
	++counterAllOut;
	if(aSrcRgbImgR.at<Vec3b>(PrevPtMod) == COLORS.black )
	{
		return PrevPtMod;
	}
	else 
		return Point(-1, -1);
}



float countTrueMean(Mat &aRgbEdgeMapR, Mat &aSrcRgbImgR, Point &prevPt, int &width, int currDir)
{
	int counterAll = 0, counterAllEdge = 0, counterAllOut = 0;
	int counterFound = 0, counterFoundEdge = 0, counterFoundOut = 0;
	//Point diffPt;
	
    //cout << __LINE__ << ": In countTrueMean with dir " << currDir << endl;

	int pointsArraySize = width * 2 + 1;
	Point pointsArray[pointsArraySize]; //create array to keep checked points's values
	fill_n( pointsArray, pointsArraySize, Point(-1, -1) );//jednak sie przyda, bo nie zawsze uda sie ją później
	//zapełnić przez warunki if zapobiegające wykraczaniu poza mapę
	
	Point PrevPtMod;
	
	int maxWidth = width; //0; //set new width to reduce counting
	
  	//Czy pixel niby usredniony przez filtr jest czarny?:
	if(aSrcRgbImgR.at<Vec3b>(prevPt.x, prevPt.y) == COLORS.black)
	{
		pointsArray[0] = prevPt;
		aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.orange;
		//aSrcRgbImgR.at<Vec3b>(PrevPtMod) = COLORS.orange;
		string message = "should be colored orange";
        //SHOW(message);
		++counterAllOut;
	}
	
	if(currDir == 1)//( diffPt.x != 0 )
	{
		for( int i = 1; i<=width; i++ )
		{
			//check in both vertical directions
			//cout <<"counterAll: " << ++counterAll << endl;
			if( prevPt.y + i < aRgbEdgeMapR.rows ){ // if not exceeds maps size
				PrevPtMod.x = prevPt.x;
				PrevPtMod.y = prevPt.y + i;
				//cout << ColorFonts::FG_DARK_GRAY; SHOW(""); cout << ColorFonts::FG_DEFAULT;
                pointsArray[i] = countTrueMeanInt(aSrcRgbImgR, PrevPtMod, counterAllOut);
				aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.blue;
				aSrcRgbImgR.at<Vec3b>(PrevPtMod) = COLORS.blue;
				
			}
			
			if( prevPt.y - i >= 0 ){ // if not exceeds maps size - 0px
				PrevPtMod.x = prevPt.x;
				PrevPtMod.y = prevPt.y - i;
				//cout << ColorFonts::FG_DARK_GRAY; SHOW(""); cout << ColorFonts::FG_DEFAULT;
                pointsArray[pointsArraySize / 2 + i] = countTrueMeanInt(aSrcRgbImgR, PrevPtMod, counterAllOut);
				aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.blue;
				aSrcRgbImgR.at<Vec3b>(PrevPtMod) = COLORS.blue;
				
			}
		}
		//}
	}
	if(currDir == 2)
	{
			for(int i = 1; i<=width; i++){
				//check in both horizontal directions
				//cout <<"counterAll: " << ++counterAllOut << endl;
				if( prevPt.x + i < aRgbEdgeMapR.cols ){
					PrevPtMod.x = prevPt.x + i;
					PrevPtMod.y = prevPt.y;
					//cout << ColorFonts::FG_DARK_GRAY; SHOW(""); cout << ColorFonts::FG_DEFAULT;
                    pointsArray[i] = countTrueMeanInt(aSrcRgbImgR, PrevPtMod, counterAllOut);
					aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.blue;
					aSrcRgbImgR.at<Vec3b>(PrevPtMod) = COLORS.blue;
					
				}
				if( prevPt.x - i >= 0 ){
					PrevPtMod.x = prevPt.x - i;
					PrevPtMod.y = prevPt.y;
					//cout << ColorFonts::FG_DARK_GRAY; SHOW(""); cout << ColorFonts::FG_DEFAULT;
                    pointsArray[pointsArraySize / 2 + i] = countTrueMeanInt(aSrcRgbImgR, PrevPtMod, counterAllOut);
					aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.blue;
					aSrcRgbImgR.at<Vec3b>(PrevPtMod) = COLORS.blue;
					
				}
			}
		//}
	}

	
	int meanCounter = 0;
	Point meanSum(0,0);
	Point meanPt;
	for(int  i = 0; i <= pointsArraySize - 1; i++)
	{	
		string message = "pointsArray[" + to_string(i) + "] = " + to_string(pointsArray[i].x) + ", " + to_string(pointsArray[i].y);
		//SHOW(message);
		if(pointsArray[i]. x != -1)
		{
			meanCounter++;
			meanSum += pointsArray[i];
		}
	}
	if(meanCounter)//should be float??
	{
		meanPt.x = meanSum.x / meanCounter;
		meanPt.y = meanSum.y / meanCounter;
		float meanPtx = (float) meanSum.x / meanCounter;

		float meanPty = (float) meanSum.y / meanCounter;

		//SHOW(VecOfMeanPts.size());
		VecOfMeanPts[VecOfMeanPts.size()-1].meanPt.push_back(Vec2f(meanPtx, meanPty));
		VecOfMeanPts[VecOfMeanPts.size()-1].direction = currDir;
		/*string message = "whole VecOfMeanPts data below:";
		SHOW(message);
		for(int i=0; i<VecOfMeanPts.size(); i++)
		{
			for(int j=0; j<VecOfMeanPts[i].size(); j++)
			{
				cout << "VecOfMeanPts["<<i<<"].";
				cout <<"["<<j<<"] = ("<<VecOfMeanPts[i][j]<<")\n";
			}
		}*/
	}
	if(meanCounter) // > 1){
	{
        //string message = "!!!!!!!!!!!!Mean point is " + to_string(meanPt.x) + ", " + to_string(meanPt.y);
        //SHOW(message);
		//a.at<Vec3b>(meanPt.x, meanPt.y) = COLORS.green;
		aRgbEdgeMapR.at<Vec3b>(meanPt) = COLORS.pink;//to psuje obliczenia
	}
	else
	{ 
        //string message = "Couldn't count mean at: " + to_string(meanPt.x) + ", " + to_string(meanPt.y);
        //SHOW(message);
		//waitKey(0); //!!
	}
	
	float sumSqrXi = 0; // for counting standard deviation
	
	for(int  i = 0; i <= pointsArraySize - 1; i++)
	{	
		if(pointsArray[i].x != -1)
		{
			
			Point diff = pointsArray[i] - meanPt;
			diff.x = abs(diff.x);
			diff.y = abs(diff.y);
			float dist = sqrt( diff.x^2 + diff.y^2 );
			//cout << "For point [" << i << "] dist is " << dist << endl;
			sumSqrXi = pow(dist, 2) + sumSqrXi;
		}
	}
	//cout << "-2^2 = " << (-2)^2 << endl;
	
	counterAll = counterAllEdge + counterAllOut;
	counterFound = counterFoundEdge + counterFoundOut;
	int percentValue = 0;

	//Pierwiastek estymatora nieobciążonego wariancji:
	//s to wartosc odchylenia, sumSqrXi to suma kwaratów odchyleń pixeli
	//zakładam że śrendnia wypada zawsze w 0
	//pxCounter to ilosc uwzglednionych pixeli
	float s = -1.0;
	if( meanCounter > 1 )
		s = sqrt( sumSqrXi / ( meanCounter - 1 ) );

	//cout << "odchylenie czastkowe = "<< s << endl;

	if( counterAllOut != 0 )
		percentValue = counterFoundOut * 100 / counterAllOut;

	ANALYSED_PX += meanCounter;

	width = maxWidth;

    //showResized(aRgbEdgeMapR, "debug window", resizeFactor, 0); //debug //!! VISU
  
  return s;
	
}
//=======================countTrueMean END=================================

//if previous, actual and next points difference points that they
// may be lying on straight line then take this point into consideration in 
// counting accuracy
//At this time only vertical and horizontal lines are taken under
// consideration
//Returns value meaning direction of line
// 1 - x++(V); 2 - y++(H); -1 - x--(V); -2 - y--(H); cross directions later todo
// 0 - not defined;
int checkDirection(Point prevPt, Point actPt, Point nextPt)
{
	//cout << __LINE__ << ": In checkDirection" << endl;
	int direction = 0;	

	Point diffActPrev = actPt - prevPt;
	Point diffNextAct = nextPt - actPt;
	
	//cout << __LINE__ << ": diffs are: " << diffActPrev << diffNextAct << endl;
 
    if( abs(diffActPrev.x) == 1 & abs(diffNextAct.x) == 1 )
		if( abs(diffActPrev.y) == 0 | abs(diffNextAct.y) == 0 ) //or until diagonal analysed
			direction = diffActPrev.x; // horizontal

    if( abs(diffActPrev.y) == 1 & abs(diffNextAct.y) == 1 )
		if( abs(diffActPrev.x) == 0 | abs(diffNextAct.x) == 0 ) //or until diagonal analysed
			direction = diffActPrev.y*2; // vertical

	return direction;
}
