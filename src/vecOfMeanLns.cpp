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

void createVecOfMeanLines(Mat &aSrcRgbImgR, Mat &aAuxRgbMap)
{
	//---------------------------------some variables--------------------------------------
	Point pt(0,0), diffPt;
	bool minLineLenReached, lookInRevDir;
	int lineLength, direction, currDir, diffSum, dirChangeCount, reachedLen, linesCounter;
	int diffSumLimit = 5, lineLenghtMin = 20, dirChangeLimit = 3;
	bool allAnalysed = false;
	structVecOfMeanPts vecOfVec2f;
	vecOfVec2f.direction = -1;
	//SHOW(vecOfVec2f.direction);
	//waitKey(0);
	VecOfMeanPts.push_back(vecOfVec2f);
	
	//----------------------loop until reaches map's last pixel-----------------------------
	while((pt.x  < (aSrcRgbImgR.cols) || pt.y < (aSrcRgbImgR.rows)) && !allAnalysed)
	{	
		//counting any line
		linesCounter++;
		//---------initializing every cycle-------------
		lineLength = 0; reachedLen = 0;
		dirChangeCount = 0; lineLength = 0; diffSum = 0;
		lookInRevDir = false; minLineLenReached = false;
		currDir = -1;
		//----------------debug-------------------
		cout << __LINE__ << ": New cycle" <<endl;
		
		//Add new empty element to VecOfMeanPts for edge which will be found next
		
		//SHOW(" ");
		//VecOfMeanPts.push_back(vecOfVec2f);//problem - adding also when line not accepted
		//SHOW(" ");		
		//------Looking for any black pixel to start with-----
		pt = lookForSpecColPxls(aAuxRgbMap, pt, COLORS.black);
		//SHOW(pt);
		if(pt.x == -1){
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
			
			//update points values
			if( !lookInRevDir || lineLength != 0 )
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
			
			nextPt = findNextPixelEdge(aAuxRgbMap, prevPt, actPt, lookInRevDir);
			/*if(nextPt.x == -1 && lineLength == 0)
			{
				
			}*/
			//showResized(aAuxRgbMap, "debug window", resizeFactor, 1); //debug	!!		
			
			//cout << __LINE__ << "points are[PAN]: " << prevPt << actPt << nextPt << endl;
			
			lineLength++;
			//------------------------------------------
			//check current direction of last 3 pixels
			direction = checkDirection( prevPt, actPt, nextPt );
			//cout << __LINE__ << ": Dir is " << direction << endl;
			
			//count whole coordinate change if direction is opposite to currently analysed
			//to check if limit is not exceeded
			diffPt = actPt - prevPt;
			if( currDir == 1)
				diffSum += diffPt.y;
			if( currDir == 2)
				diffSum += diffPt.x;

			//save first considered currently direction as refernece
			if( currDir == -1 && direction != 0 )
				currDir = direction;

			//if current direction is not equal to currenly analysed, increase counter value
			//if is reset value
			if( direction != currDir )
				dirChangeCount++;
			else
				dirChangeCount = 0;
			
			//set true if reached
			if( lineLength > lineLenghtMin && ~lookInRevDir )
			{
				minLineLenReached = true;
				cout << __LINE__ << ": Line accepted" << endl;
				
			}		
			
			//if sum of direction changes or whole coordinate change limit is exceed 
			//then check if line has reched minimum length
			//is yes it will be further analysed
			
			if ( dirChangeCount > dirChangeLimit || diffSum > diffSumLimit )
			{
				//currDir = -1;
				diffSum = 0;
				if( minLineLenReached )
				{
					lookInRevDir = true;
					dirChangeCount = -1;
					reachedLen = lineLength;
					SHOW("Direction changed and min len reached");
					SHOW(lineLength);
					lineLength = 0;
					
				}
				else
				{
					//if not set -1 and this series will be ommited
					SHOW(""); cout << "	dir change limit exceeded" << endl;
					nextPt.x = -1;
				}
			}
			
			if( lookInRevDir ) //if accepted to analyse
			{
				
				int otoczenie = 5;
				float dokladnosc = countTrueMean(aAuxRgbMap, aSrcRgbImgR, prevPt, actPt, otoczenie, currDir);
				//if( dokladnosc >= 0 ){
					//sum2 = sum2 + dokladnosc;
					//sum = sum + dokladnosc;
					//ile++;
					//ile2++;
				//}
				//cout << "licze dokladnosc" << endl;
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
					SHOW(lineLength); cout << "	Next point not found, but min len reached" << endl;
					lineLength = 0;
					nextPt.x = -2;//to avoid starting new cycle
					Point tmpPt = prevPt;
					//nextPt == -1;
					prevPt = actPt;
					actPt = tmpPt;
				}
				else
				{
					//if not leave -1 and this series will be ommited
					SHOW(nextPt.x);
				}
			}
			
			//If going backwards measured forward length is reached then finish analying this series.
			//Maybe it's bad idea? maybe should follow until doesn't fit previous condiditons
			//like sum of direction changes or whole coordinate change limit is exceed 
			/*if( lookInRevDir && (lineLength >= (reachedLen - 2)) )
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
	bool stop = false;
	Point pt = aPt;
	
	while((pt.x < (aImgR.cols) || pt.y < (aImgR.rows)) && stop == false){
		while(pt.x < (aImgR.cols) && stop == false){
			Vec3b pxColour = aImgR.at<Vec3b>(pt);
			//SHOW(pt);
			if( pxColour == aColour)
			{
				stop = true;
				//aImgR.at<Vec3b>(pt) = COLORS.green; //for debug
			}

			if( !stop )			
				pt.x++;
		}
  		if( !stop && ((pt.y ) < (aImgR.rows)) )
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
Point checkSpecDirection( Mat &srcImg, Point prevPt, Point actPt, int maxGap, bool lookInRevDir )// pt = point
{
	Vec3b wantedColor;	
	Point foundPt( -1, -1 ), modPt; 
	if( lookInRevDir )
		wantedColor = COLORS.red;
	else
		wantedColor = COLORS.black;
	bool found = false;
	int direction;

	int rad = 1;
	while( rad <= maxGap && !found )//every loop increases radius
	{	
		int mG = 1;
		while( mG <= ( maxGap + 1 ) && !found )//every loop cheks lower priority pixels
		{
			direction = 1;
			while ( direction <= 4 && !found )
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
				if(modPt.x < srcImg.cols && modPt.y < srcImg.rows)
				{
					//SHOW(modPt);
					if( (srcImg.at<Vec3b>(modPt) == wantedColor || srcImg.at<Vec3b>(modPt) == COLORS.black) && modPt != prevPt )
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
	return foundPt;
}

//szuka kolejnego pixela nalezącego do konturu
Point findNextPixelEdge(Mat &aImgR, Point prevPt, Point actPt, bool lookInRevDir){
	Point nextPt(-1,-1);

	//cout << "In findNextPixelEdge" << endl;
	nextPt = checkSpecDirection( aImgR, prevPt, actPt, 3, lookInRevDir );
	//cout << __LINE__ << ": Next point is " << nextPt << endl;

	if( !lookInRevDir )
	{
		//message = "/*Prev to*/ ["+to_string(prevPt.x)+", "+to_string(prevPt.y)+"] coloured red";
		//SHOW(prevPt);SHOW(actPt);SHOW(nextPt);
		aImgR.at<Vec3b>(prevPt) = COLORS.red; //for debug
	}
	else
		aImgR.at<Vec3b>(prevPt) = COLORS.grey; //for debug
	
	return nextPt;
}

//to check if found point is not neigbour to previous point(and previous point itself)
//returns true if is not
bool checkIfNotNeighbour( const Mat &srcImg, Point refPt, Point checkPt )
{
	bool retVal = false;
	Point diffPt = refPt - checkPt;
	if( refPt != checkPt )	
		if( abs(diffPt.x) != 1 )
			if( abs(diffPt.y) != 1 )
				retVal = true;
	return retVal;
}

Point countTrueMeanInt(Mat &aSrcRgbImgR, Mat &aImgRgbEdgeR,/*Point (&pointsArray)[],*/ int pointsArraySize, Point PrevPtMod, int &counterAllOut, /*bool fOrS,*/ int currWidth, int &maxWidth)
{
	//static Mat clonedSrcRgbImg = aSrcRgbImgR.clone();
	SHOW(PrevPtMod);
	cout << ColorFonts::FG_BLUE << "   :Checking above point in countTrueMeanInt. Its colour is ";
	cout << aSrcRgbImgR.at<Vec3b>(PrevPtMod) << ColorFonts::FG_DEFAULT << endl;
	
	++counterAllOut;
	//waitKey(0);
	
	if(aSrcRgbImgR.at<Vec3b>(PrevPtMod) == COLORS.black )
	{

		//string message = "Coloured yellow";
		//SHOW(message);
		//aSrcRgbImgR.at<Vec3b>(PrevPtMod) = COLORS.yellow; //zaznacza na zolty uwzglednione pixelec
		//aImgRgbEdgeR.at<Vec3b>(PrevPtMod) = COLORS.yellow;
		return PrevPtMod;
	}
	else 
		return Point(-1, -1);
	
	
	
}



float countTrueMean(Mat &aRgbEdgeMapR, Mat &aSrcRgbImgR, Point &prevPt, Point &actPt/*NU*/, int &width, int currDir)
{
	int counterAll = 0, counterAllEdge = 0, counterAllOut = 0;
	int counterFound = 0, counterFoundEdge = 0, counterFoundOut = 0;
	uchar black = 0;
	//Point diffPt;
	
	cout << __LINE__ << ": In countTrueMean with dir " << currDir << endl;
	
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
		SHOW(message);
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

		SHOW(VecOfMeanPts.size());
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
		string message = "!!!!!!!!!!!!Mean point is " + to_string(meanPt.x) + ", " + to_string(meanPt.y);
		SHOW(message);
		//a.at<Vec3b>(meanPt.x, meanPt.y) = COLORS.green;
		aRgbEdgeMapR.at<Vec3b>(meanPt) = COLORS.pink;//to psuje obliczenia
	}
	else
	{ 
		string message = "Couldn't count mean at: " + to_string(meanPt.x) + ", " + to_string(meanPt.y);
		SHOW(message);
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

	int pxCounter = counterFoundOut;
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

	showResized(aRgbEdgeMapR, "debug window", resizeFactor, 1); //debug //!!
  
  return s;
	
}
//=======================countTrueMean END=================================

//if previous, actual and next points difference points that they
// may be lying on straight line then take this point into consideration in 
// counting accuracy
//At this time only vertical and horizontal lines are taken under
// consideration
//Returns value meaning direction of line
// 1 - x(V); 2 - y(H); cross directions later todo
// 0 - not defined;
int checkDirection(Point prevPt, Point actPt, Point nextPt)
{
	//cout << __LINE__ << ": In checkDirection" << endl;
	int direction = 0;	

	Point diffActPrev = actPt - prevPt;
	Point diffNextAct = nextPt - actPt;
	
	//cout << __LINE__ << ": diffs are: " << diffActPrev << diffNextAct << endl;
 
	if( abs(diffActPrev.x) == 1 && abs(diffNextAct.x) == 1 )
		if( abs(diffActPrev.y) == 0 || abs(diffNextAct.y) == 0 ) //or until diagonal analysed
			direction = 1; // vertical
	if( abs(diffActPrev.y) == 1 && abs(diffNextAct.y) == 1 )
		if( abs(diffActPrev.x) == 0 || abs(diffNextAct.x) == 0 ) //or until diagonal analysed
			direction = 2; // horizontal
	
	return direction;
}

/*305: pt = [87, 73]
305: pt = [88, 73]
136: pt = [88, 73]
264: nextPt.x = -1
127: New cycle
131: " " =  
133: " " =  
305: pt = [88, 73]
136: pt = [-1, -1]
139: allAnalysed = 1
281: message = Out of while loop
144: " " =  
==7511== Invalid free() / delete / delete[] / realloc()
==7511==    at 0x4C29E90: free (vg_replace_malloc.c:473)
==7511==    by 0x9325E04: free_mem (in /lib/x86_64-linux-gnu/libc-2.19.so)
==7511==    by 0x9326141: __libc_freeres (in /lib/x86_64-linux-gnu/libc-2.19.so)
==7511==    by 0x4A236CC: _vgnU_freeres (vg_preloaded.c:63)
==7511==    by 0x9211AEA: __run_exit_handlers (exit.c:97)
==7511==    by 0x9211B74: exit (exit.c:104)
==7511==    by 0x91FBB4B: (below main) (libc-start.c:321)
==7511==  Address 0xff000009345a40 is not stack'd, malloc'd or (recently) free'd
==7511== 
==7511== Invalid free() / delete / delete[] / realloc()
==7511==    at 0x4C29E90: free (vg_replace_malloc.c:473)
==7511==    by 0x9325E0D: free_mem (in /lib/x86_64-linux-gnu/libc-2.19.so)
==7511==    by 0x9326141: __libc_freeres (in /lib/x86_64-linux-gnu/libc-2.19.so)
==7511==    by 0x4A236CC: _vgnU_freeres (vg_preloaded.c:63)
==7511==    by 0x9211AEA: __run_exit_handlers (exit.c:97)
==7511==    by 0x9211B74: exit (exit.c:104)
==7511==    by 0x91FBB4B: (below main) (libc-start.c:321)
==7511==  Address 0xff00000014cd1990 is not stack'd, malloc'd or (recently) free'd
==7511== 
==7511== 
==7511== HEAP SUMMARY:
==7511==     in use at exit: 2,683,369 bytes in 7,616 blocks
==7511==   total heap usage: 66,465 allocs, 58,851 frees, 878,648,203 bytes allocated
==7511== 
==7511== LEAK SUMMARY:
==7511==    definitely lost: 6 bytes in 1 blocks
==7511==    indirectly lost: 0 bytes in 0 blocks
==7511==      possibly lost: 1,969,265 bytes in 323 blocks
==7511==    still reachable: 635,162 bytes in 6,872 blocks
==7511==         suppressed: 0 bytes in 0 blocks
==7511== Rerun with --leak-check=full to see details of leaked memory
==7511== 
==7511== For counts of detected and suppressed errors, rerun with: -v
==7511== Use --track-origins=yes to see where uninitialised values come from
==7511== ERROR SUMMARY: 570 errors from 7 contexts (suppressed: 0 from 0)*/

