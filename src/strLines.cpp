/*
	W plikach z takim rozszerzeniem umieszczamy tylko 
	i wyłącznie definicje funkcji, czyli nazwę funkcji 
	razem z jej ciałem (czyli blokiem funkcji). 
*/

#include "strLines.h"

void showResized(const Mat &srcImg, const string& winname, double factor, int timeMs)
{
	Mat bigImg;
	//INTER_NEAREST do not fuzz pixels
	resize(srcImg, bigImg, Size(), factor, factor, INTER_NEAREST);
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

//change grey pixels to black to avoid double parallel lines:
//takes references and changes dst colours
//third arg is colour of grey
void colorChangeAllRgb(Mat &Img, Vec3b srcColor, Vec3b dstColor)
{
	for(int x = 0; x <= Img.rows; x++){
		for(int y = 0; y <= Img.cols; y++){
			//Vec3b pxColor = Img.at<Vec3b>(x, y);
			if( Img.at<Vec3b>(x, y) == srcColor)
				Img.at<Vec3b>(x, y) = dstColor;
		}
	}
}

void createMapOfMeanLines(const Mat &caSrcRgbImgR, Mat &aAuxRgbMap)
{
	Point pt(0,0), diffPt;
	bool minLineLenReached, lookInRevDir;
	int lineLength, direction, currDir, diffSum, dirChangeCount, reachedLen, linesCounter;
	int diffSumLimit = 5, lineLenghtMin = 20, dirChangeLimit = 3;
	
	//until reaches map's last pixel
	while(pt.x  < (caSrcRgbImgR.rows) || pt.y < (caSrcRgbImgR.cols))
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
			nextPt = findNextPixelEdge(aAuxRgbMap, prevPt, actPt, lookInRevDir);
			showResized(aAuxRgbMap, "aAuxRgbMap", 2.5, 0); //debug
			
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
			
			lineLength++;
			//------------------------------------------
			//check current direction of last 3 pixels
			direction = checkDirection( prevPt, actPt, nextPt );
			
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
					//cout << "Direction changed and min len reached = " << lineLength << endl;
					lineLength = 0;
				}
				else
				{
					//if not set -1 and this series will be ommited
					nextPt.x = -1;
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

			//set true if reached
			if( lineLength > lineLenghtMin )
			{
				minLineLenReached = true;
				cout << __LINE__ << ": Line accepted" << endl;
			}
				
			if( lookInRevDir ) //if accepted to analyse
			{
				//dokladnosc = countTrueMean(pixImg, prevPoint, actPoint, otoczenie, currDir);
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
				aImgR.at<Vec3b>(pt) = COLORS.green; //for debug
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
	cout << __LINE__ << ": Next point is " << nextPt << endl;

	if( !lookInRevDir )
		aImgR.at<Vec3b>(prevPt) = COLORS.red; //for debug
	
	return nextPt;
}

//should help in checking pixels, //direction 1 - x++; 2 - y++; 3 - x--; 4 - y--
Point checkSpecDirection( Mat &srcImg, Point prevPt, Point actPt, int maxGap, bool lookInRevDir )// pt = point
{
	Vec3b wantedColor;	
	Point foundPt( -1, -1 ), modPt; 
	if( lookInRevDir )
		wantedColor = COLORS.grey;
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
				cout << __LINE__ << ": at "<< modPt << " found color is "<< srcImg.at<Vec3b>(modPt) << endl;
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
	Point diffPoint = refPt - checkPt;
	if( refPt != checkPt )	
		if( abs(diffPoint.x) != 1 )
			if( abs(diffPoint.y) != 1 )
				retVal = true;
	return retVal;
}

Point countTrueMeanInt(Mat &pixImg, /*Point (&pointsArray)[],*/ int pointsArraySize, Point PrevPtMod, int &counterAllOut, /*bool fOrS,*/ int currWidth, int &maxWidth)
{
	//uchar black = 0;
	cout << "In countTrueMeanInt" << endl;
	cout << "Checking " << PrevPtMod << endl;
	Vec3b color = pixImg.at<Vec3b>(PrevPtMod);
	cout << "Its colour is " << color << endl;
	
	++counterAllOut;
	waitKey(0);
	if(pixImg.at<Vec3b>(PrevPtMod) == COLORS.black )
	{

		cout << "Coloured 100" << endl;
		pixImg.at<Vec3b>(PrevPtMod) = COLORS.blue; //zaznacza na ciemny szary uwzglednione pixele
		//if(currWidth > maxWidth)
			//maxWidth = currWidth;
		return PrevPtMod;
	}
	else 
		return Point(-1, -1);
	
	
	
}

float countTrueMean(Mat &pixImg, Point &prevPt, Point &actPt, int &width, int currDir)
{
	int counterAll = 0, counterAllEdge = 0, counterAllOut = 0;
	int counterFound = 0, counterFoundEdge = 0, counterFoundOut = 0;
	uchar black = 0;
	Point diffPoint;
	
	cout << "In countTrueMean with dir " << currDir << endl;
	
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

  	//Czy pixel niby usredniony przez filtr jest czarny?:
	if(pixImg.at<uchar>(prevPt.x, prevPt.y) == black)
	{
		pointsArray[0] = prevPt;
		pixImg.at<uchar>(PrevPtMod) = 80;
		cout << "should be colored" << endl;
		++counterAllOut;
	}
	//{
	diffPoint = actPt - prevPt;
 	//cout <<"diffPoint = " << diffPoint << endl;
	if(currDir == 1)//( diffPoint.x != 0 )
	{
		//if( diffPoint.y == 0 )//then look in vertical direction
		//{
			for( int i = 1; i<=width; i++ )
			{
				//check in both vertical directions
				//cout <<"counterAll: " << ++counterAll << endl;
				if( prevPt.y + i <= pixImg.cols ){ // if not exceeds maps size - cols px
					PrevPtMod.x = prevPt.x;
					PrevPtMod.y = prevPt.y + i;
					pixImg.at<uchar>(PrevPtMod) = 150;
					pointsArray[i] = countTrueMeanInt(pixImg, pointsArraySize, PrevPtMod, counterAllOut, i, maxWidth);
				}
				
				if( prevPt.y - i >= 0 ){ // if not exceeds maps size - 0px
					PrevPtMod.x = prevPt.x;
					PrevPtMod.y = prevPt.y - i;
					pixImg.at<uchar>(PrevPtMod) = 150;
					pointsArray[pointsArraySize / 2 + i] = countTrueMeanInt(pixImg, pointsArraySize, PrevPtMod, counterAllOut, i, maxWidth);
				}
			}
		//}
	}
	if(currDir == 2)//( diffPoint.y != 0 )
	{
		//if( diffPoint.x == 0 ){//then look in vertical direction
			for(int i = 1; i<=width; i++){
				//check in both horizontal directions
				//cout <<"counterAll: " << ++counterAllOut << endl;
				if( prevPt.x + i <= pixImg.rows ){
					PrevPtMod.x = prevPt.x + i;
					PrevPtMod.y = prevPt.y;
					pixImg.at<uchar>(PrevPtMod) = 150;
					pointsArray[i] = countTrueMeanInt(pixImg, pointsArraySize, PrevPtMod, counterAllOut, i, maxWidth);
				}
				if( prevPt.x - i >= 0 ){
					PrevPtMod.x = prevPt.x - i;
					PrevPtMod.y = prevPt.y;
					pixImg.at<uchar>(PrevPtMod) = 150;
					pointsArray[pointsArraySize / 2 + i] = countTrueMeanInt(pixImg, pointsArraySize, PrevPtMod, counterAllOut, i, maxWidth);
				}
			}
		//}
	}

	
	//debug:
	int meanCounter = 0;
	Point meanSum(0,0);
	Point meanPt;
	for(int  i = 0; i <= pointsArraySize - 1; i++)
	{	
		//cout << "pointsArray[" << i << "] = " << pointsArray[i] << endl;
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
	if(meanCounter){ // > 1){
		//cout << "!!!!!!!!!!!!Mean point is " << meanPt << endl;
		detected_edges2.at<uchar>(meanPt.x, meanPt.y) = 200;
		pixImg.at<uchar>(meanPt.x, meanPt.y) = 220;//to psuje obliczenia
	}
	else
		cout << "Couldn't count mean at: " << meanPt.x << endl;
	
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

	Mat pixImgBig;

	resize(pixImg, pixImgBig, Size(), 2.5, 2.5, INTER_CUBIC);
	namedWindow( "Curent State pixImg", WINDOW_AUTOSIZE ); //uncomment to see image
	//imshow( "Curent State pixImg", pixImg ); //uncomment to see image
	imshow( "Curent State pixImg", pixImgBig );
   waitKey(1);	//uncomment to see image
  
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
	cout << __LINE__ << ": In checkDirection" << endl;
	int direction = 0;	

	Point diffPt1 = actPt - prevPt;
	Point diffPt2 = nextPt - actPt;

	if( abs(diffPt1.x) == 1 && abs(diffPt2.x) == 1 )
		if( abs(diffPt1.y) == 0 || abs(diffPt2.y) == 0 ) //or until diagonal analysed
			direction = 1; // vertical
	if( abs(diffPt1.y) == 1 && abs(diffPt2.y) == 1 )
		if( abs(diffPt1.x) == 0 || abs(diffPt2.x) == 0 ) //or until diagonal analysed
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
/*int countStdDev(Mat &edgeImg, Mat &pixImg, int otoczenie)
{
	cout << "In countStdDev" << endl;
	//int val = src_gray.at<uchar>(x, y);//wartosc pixela w (x,y)
	//skanuj mapę edges w poszukiwaniu białych pixeli:
	//mapaRobocza = Scalar::all(0);
	//src.copyTo( mapaRobocza, detected_edges);
	int linesCounter = 0;
	float sum = 0.0, sum2 = 0.0;
	int sum3 = 0;
	float dokladnosc;
	int ile = 0, ile2 = 0, ile3 = 0;
	int longest = 0, ileDlugich = 0;
	Point pt(0,0);
	cvtColor( detected_edges2, color_dst3, CV_GRAY2BGR );
	int lines[4];
	
	//until reaches map's last pixel
	while(pt.x  < (edgeImg.rows) || pt.y < (edgeImg.cols))
	{
		//cout << "new cycle"<< endl;	
			//pt = lookForWhitePxls(edgeImg, pt);
		
		Point nextPoint = pt, actPoint = pt, prevPoint = pt;
		int tempInt = edgeImg.at<uchar>(pt.x, pt.y);
		//cout << "Value at found point is: " << tempInt << endl ;

		//for(int i = 0; i <= 3; i++){
		//cout << "iteracja: " << i << endl;
		linesCounter++;
		int lineLength;
		sum2 = 0;
		ile2 = 0;
		int index = 0;
		lines[0] = pt.y;
		lines[1] = pt.x;

		int dirChangeCount;
		int dirChangeLimit; 
		int lineLenghtMin = 50;
		int direction;
		int currDir;
		bool minLineLenReached;
		bool lookInRevDir;
		int reachedLen;

		dirChangeLimit = 3; 
		lookInRevDir = false;
		minLineLenReached = false;
		reachedLen = 0;
		currDir = -1;
		dirChangeCount = 0;
		lineLength = 0;
		
		Point diffPt;
		int diffSum;
		int diffSumLimit = 5;
		diffSum = 0;

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
