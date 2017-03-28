/*
	W plikach z takim rozszerzeniem umieszczamy tylko 
	i wyłącznie definicje funkcji, czyli nazwę funkcji 
	razem z jej ciałem (czyli blokiem funkcji). 
*/

#include "strLines.h"

//usage e.g. cout << Color::FG_RED << "text";
namespace Color {
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

void showResized(const Mat &srcImg, const string& winname, double factor, int timeMs)
{
	Mat bigImg;
	//INTER_NEAREST do not fuzz pixels
	resize(srcImg, bigImg, Size(), factor, factor, INTER_NEAREST);
	//string ty =  type2str( bigImg.type() );
	//string message = "Matrix: " + ty + " " + to_string(bigImg.cols) + "x" + to_string(bigImg.rows);
	//SHOW(message);
	namedWindow(winname, WINDOW_AUTOSIZE);
	imshow(winname, bigImg);
	waitKey(timeMs);
}

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
        
        cout << "clicked on " << Pt << endl;
    }
}

//change grey pixels to black to avoid double parallel lines:
//takes references and changes dst colours
//third arg is colour of grey
void colorChangeAllRgb(Mat &Img, Vec3b srcColor, Vec3b dstColor)
{
	for(int x = 0; x < Img.rows; x++){
		for(int y = 0; y < Img.cols; y++){
			//Vec3b pxColor = Img.at<Vec3b>(x, y);
			if( Img.at<Vec3b>(x, y) == srcColor)
				Img.at<Vec3b>(x, y) = dstColor;
		}
	}
}

void createMapOfMeanLines(Mat &aSrcRgbImgR, Mat &aAuxRgbMap)
{
	Point pt(0,0), diffPt;
	bool minLineLenReached, lookInRevDir;
	int lineLength, direction, currDir, diffSum, dirChangeCount, reachedLen, linesCounter;
	int diffSumLimit = 5, lineLenghtMin = 20, dirChangeLimit = 3;
	
	//until reaches map's last pixel
	while(pt.x  < (aSrcRgbImgR.rows) || pt.y < (aSrcRgbImgR.cols))
	{	
		linesCounter++;
		lineLength = 0; reachedLen = 0;
		dirChangeCount = 0; lineLength = 0; diffSum = 0;
		lookInRevDir = false; minLineLenReached = false;
		currDir = -1;

		cout << __LINE__ << ": New cycle" <<endl;
		pt = lookForSpecColPxls(aAuxRgbMap, pt, COLORS.black);
		//showResized(aAuxRgbMap, "aAuxRgbMap", 2.5, 0); //debug
		//cout << pt << endl; //debug
		Point nextPt = pt, actPt = pt, prevPt = pt;
		
		//while there is still any not analysed pixel
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
					nextPt = prevPt; 
					prevPt = tempPt;
				}			
			}
			
			nextPt = findNextPixelEdge(aAuxRgbMap, prevPt, actPt, lookInRevDir);
			showResized(aAuxRgbMap, "debug window", resizeFactor, 0); //debug			
			
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
			
			if (nextPt.x == -1)
			{
				if( minLineLenReached )
				{
					lookInRevDir = true;
					dirChangeCount = -1;
					reachedLen = lineLength;
					SHOW(lineLength); cout << "	Next point not found, but min len reached" << endl;
					lineLength = 0;
					nextPt.x == -2;//why?
					Point tmpPt = prevPt;
					prevPt = nextPt;
					nextPt = tmpPt;
				}
				else
				{
					//if not set -1 and this series will be ommited
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
		}
	}
		
}

//idea for variable naming convention: [Argument/Local/Static][name][Reference, Pointer]
Point lookForSpecColPxls(Mat &aImgR, Point aPt, Vec3b aColour)
{
	bool stop = false;
	Point pt = aPt;

	while((pt.x < (aImgR.cols) || pt.y < (aImgR.rows)) && stop == false){
		while(pt.x < (aImgR.cols) && stop == false){
			Vec3b pxColour = aImgR.at<Vec3b>(pt);

			if( pxColour == aColour)
			{
				stop = true;
				//aImgR.at<Vec3b>(pt) = COLORS.green; //for debug
			}

			if( !stop )			
				pt.x++;
		}
  		if( !stop && pt.y < (aImgR.rows) )
  		{		
			pt.x = 0;
			pt.y++;
		}
	}
	return pt;
}

//szuka kolejnego pixela nalezącego do konturu
Point findNextPixelEdge(Mat &aImgR, Point prevPt, Point actPt, bool lookInRevDir){
	Point nextPt(-1,-1);

	//cout << "In findNextPixelEdge" << endl;
	nextPt = checkSpecDirection( aImgR, prevPt, actPt, 3, lookInRevDir );
	//cout << __LINE__ << ": Next point is " << nextPt << endl;

	if( !lookInRevDir )
		aImgR.at<Vec3b>(prevPt) = COLORS.red; //for debug
	else
		aImgR.at<Vec3b>(prevPt) = COLORS.pink; //for debug
	
	return nextPt;
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
				if( srcImg.at<Vec3b>(modPt) == wantedColor && modPt != prevPt )
				{	
					foundPt = modPt;
					found = true;
				}
				direction++;
			}
			mG++;
		}
		rad++;
	}
	return foundPt;
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
	cout << Color::FG_BLUE << "   :Checking above point in countTrueMeanInt. Its colour is ";
	cout << aSrcRgbImgR.at<Vec3b>(PrevPtMod) << Color::FG_DEFAULT << endl;
	
	++counterAllOut;
	//waitKey(0);
	
	if(aSrcRgbImgR.at<Vec3b>(PrevPtMod) == COLORS.black )
	{

		string message = "Coloured yellow";
		SHOW(message);
		aSrcRgbImgR.at<Vec3b>(PrevPtMod) = COLORS.yellow; //zaznacza na zolty uwzglednione pixelec
		aImgRgbEdgeR.at<Vec3b>(PrevPtMod) = COLORS.yellow;
		//if(currWidth > maxWidth)
			//maxWidth = currWidth;
		//showResized(aSrcRgbImgR, "SrcRgbImg", 2.5, 0);
		return PrevPtMod;
	}
	else 
		return Point(-1, -1);
	
	
	
}

float countTrueMean(Mat &aRgbEdgeMapR, Mat &aSrcRgbImgR, Point &prevPt, Point &actPt, int &width, int currDir)
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

	/*====SCIĄGA MAT SIZE====:
	cv:Mat mat;
	int rows = mat.rows;
	int cols = mat.cols;

	cv::Size s = mat.size();
	rows = s.height;
	cols = s.width;
	*/
	
	Point diffPt = actPt - prevPt;

	//why has to use prevPoint, why not actPoint??
	prevPt = actPt;

  	//Czy pixel niby usredniony przez filtr jest czarny?:
	if(aSrcRgbImgR.at<Vec3b>(prevPt.x, prevPt.y) == COLORS.black)
	{
		pointsArray[0] = prevPt;
		//aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.orange;
		//aSrcRgbImgR.at<Vec3b>(PrevPtMod) = COLORS.orange;
		string message = "should be colored orange";
		SHOW(message);
		++counterAllOut;
	}
	
	//{
	
 	//cout <<"diffPt = " << diffPt << endl;
	if(currDir == 1)//( diffPt.x != 0 )
	{
		//if( diffPt.y == 0 )//then look in vertical direction
		//{
			for( int i = 1; i<=width; i++ )
			{
				//check in both vertical directions
				//cout <<"counterAll: " << ++counterAll << endl;
				if( prevPt.y + i <= aRgbEdgeMapR.cols ){ // if not exceeds maps size - cols px
					PrevPtMod.x = prevPt.x;
					PrevPtMod.y = prevPt.y + i;
					cout << Color::FG_DARK_GRAY; SHOW(""); cout << Color::FG_DEFAULT;
					aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.blue;
					pointsArray[i] = countTrueMeanInt(aSrcRgbImgR, aRgbEdgeMapR, pointsArraySize, PrevPtMod, counterAllOut, i, maxWidth);
				}
				
				if( prevPt.y - i >= 0 ){ // if not exceeds maps size - 0px
					PrevPtMod.x = prevPt.x;
					PrevPtMod.y = prevPt.y - i;
					cout << Color::FG_DARK_GRAY; SHOW(""); cout << Color::FG_DEFAULT;
					aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.blue;
					pointsArray[pointsArraySize / 2 + i] = countTrueMeanInt(aSrcRgbImgR, aRgbEdgeMapR, pointsArraySize, PrevPtMod, counterAllOut, i, maxWidth);
				}
			}
		//}
	}
	if(currDir == 2)//( diffPt.y != 0 )
	{
		//if( diffPt.x == 0 ){//then look in vertical direction
			for(int i = 1; i<=width; i++){
				//check in both horizontal directions
				//cout <<"counterAll: " << ++counterAllOut << endl;
				if( prevPt.x + i <= aRgbEdgeMapR.rows ){
					PrevPtMod.x = prevPt.x + i;
					PrevPtMod.y = prevPt.y;
					cout << Color::FG_DARK_GRAY; SHOW(""); cout << Color::FG_DEFAULT;
					aRgbEdgeMapR.at<Vec3b>(PrevPtMod) = COLORS.blue;
					pointsArray[i] = countTrueMeanInt(aSrcRgbImgR, aRgbEdgeMapR, pointsArraySize, PrevPtMod, counterAllOut, i, maxWidth);
				}
				if( prevPt.x - i >= 0 ){
					PrevPtMod.x = prevPt.x - i;
					PrevPtMod.y = prevPt.y;
					cout << Color::FG_DARK_GRAY; SHOW(""); cout << Color::FG_DEFAULT;
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
		SHOW(message);
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
	}
	if(meanCounter) // > 1){
	{
		string message = "!!!!!!!!!!!!Mean point is " + to_string(meanPt.x) + ", " + to_string(meanPt.y);
		SHOW(message);
		//a.at<Vec3b>(meanPt.x, meanPt.y) = COLORS.green;
		aSrcRgbImgR.at<Vec3b>(meanPt) = COLORS.pink;//to psuje obliczenia
	}
	else
	{ 
		string message = "Couldn't count mean at: " + to_string(meanPt.x) + ", " + to_string(meanPt.y);
		SHOW(message);
		waitKey(0);
	}
	
	float sumSqrXi = 0; // for counting standart deviation
	
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

	showResized(aRgbEdgeMapR, "debug window", resizeFactor, 0); //debug
  
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

//Poprawic sposob obliczania dokladnosci!!!!
//dokladnosc to odchylenie danego pomiaru od spodziewanej wartosci referencyjnej
//czyli moze dla kazdego pixela osobno nalezy wyznaczyc dokladnosc?

//returns standard deviation of measurements with laser scanner
//okresla ile pixeli w najblizszym otoczeniu linii nie nalezy do linii
//okreslic jako linie tylko dlugie proste i tylko je analizowac
//wyszlo probnie ze odchylenie st. = 0.0878308 
/*

		//while there is still any not analysed pixel
		while( nextPoint.x != -1 )
		{
			
			//if( lookInRevDir )
			//	dirChangeLimit = 5; 
			//pt = nextPoint;
			
			//find next "obstacle" pixel in current series
			nextPoint = findNextPixelEdge(edgeImg, prevPoint, actPoint, lookInRevDir);

			//if previous, actual and next points difference points that they
			// may be lying on straight line then take this point into consideration in 
			// counting accuracy
			//cout << "Pts are: " << prevPoint << actPoint << nextPoint << endl;
			
			//check current direction of last 3 pixels
			if( !lookInRevDir )
				direction = checkDirection( prevPoint, actPoint, nextPoint );
			
			//cout << "Direction is: " << direction << endl;
			//cout << ". Act point is: " << actPoint << endl;
			// dodac kontrole calkowitej zmiany wspolrzednej
			
			//count whole coordinate change if direction is opposite to currently analysed
			//to check if limit is not exceeded
			if( !lookInRevDir )
			{
				diffPt = actPoint - prevPoint;
				if( currDir == 1)
					diffSum += diffPt.y;
				if( currDir == 2)
					diffSum += diffPt.x;
			}

			//save first considered currently direction as refernece
			if( currDir == -1 && direction != 0 )
				currDir = direction;

			//if current direction is not equal to currenly analysed, increase counter value
			//if is reset value
			if( direction != currDir )
				dirChangeCount++;
			else
				dirChangeCount = 0;
			
			//if sum of direction changes or whole coordinate change limit is exceed 
			//then check if line has reched minimum length
			//is yes it will be further analysed
			//if not set -1 and this series will be ommited
			if ( dirChangeCount > dirChangeLimit || diffSum > diffSumLimit )
			{
				//currDir = -1;
				diffSum = 0;
				if( minLineLenReached )
				{
					lookInRevDir = true;
					dirChangeCount = -1;
					reachedLen = lineLength;
					//cout << "Direction changed and min len reached = " << lineLength << endl;
					lineLength = 0;
				}
				else
				{
					//lineLength = 0;
					nextPoint.x = -1;
				}
			}
			
			//If going backwards measured forward length is reched then finish analying this series.
			//Maybe it's bad idea? maybe should follow until doesn't fit previous condiditons
			//like sum of direction changes or whole coordinate change limit is exceed 
			if( lookInRevDir && (lineLength >= (reachedLen - 2)) )
			{
				nextPoint.x = -1;
				ileDlugich++;
			}

			//set true if reached
			if( lineLength > lineLenghtMin )
				minLineLenReached = true;
			
			//update points values
			if( !lookInRevDir || lineLength != 0 )
			{
				prevPoint = actPoint;
				actPoint = nextPoint;
			}
			else 
			{
				if( lineLength == 0 )
				{
					Point tempPt = nextPoint;			
					nextPoint = prevPoint; 
					prevPoint = tempPt;
				}			
			}
			
			if( lookInRevDir ) //if accepted to analyse
			{
				dokladnosc = countTrueMean(pixImg, prevPoint, actPoint, otoczenie, currDir);
				if( dokladnosc >= 0 ){
					sum2 = sum2 + dokladnosc;
					sum = sum + dokladnosc;
					ile++;
					ile2++;
				}
				//cout << "licze dokladnosc" << endl;
			}

			lineLength++;
		}
		cout << "---BREAK--- length:" << lineLength << endl;

		if (lineLength > longest)
			longest = lineLength;

	}
	imshow( "linie", color_dst3 );
	cout << "Najdluzszych linii jest = " << ileDlugich << endl;
	dokladnosc = sum / ile;
	cout << "linesCounter = " << linesCounter << endl;
	cout << "Odchylenie standardowe = " << dokladnosc << " px" << endl;
	return 0;
*/
