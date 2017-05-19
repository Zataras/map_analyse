/* file main.cpp
Analyses maps created with ROS
Aim of this project is to define accuracy and recurrence of used mapping method
*/	

#include "main.h"

// TODO :
//WIZUALIZACJA DZIAŁANIA DO DOMKUNETACJI!
//Analiza wszystkich map i wyciagniecie sredniej
//program do liczenia powtarzalnosci

/** @function main */
int main( int argc, char* argv[] )
{
	string message;
	/// Load an image
    if (argc < 1)
    {
        cout << "Name of image not specified";
        return -1;
    }
    // Mat objects life block
    {
        Mat srcRgbImg = imread( argv[1] );

        namedWindow("debug window", WINDOW_AUTOSIZE);//WINDOW_AUTOSIZE);
        setMouseCallback("debug window", onMouse, NULL); //CHECK POINT COORD.
        //bshowResized(srcRgbImg, "auxRgbMap", resizeFactor, 10); //debug
        //destroyAllWindows();

        if( !srcRgbImg.data )
            return -1;

        int width = 6;
        if( argc > 2 )
        {
            width = atoi(argv[2]);
            cout << "width = " << width << endl;
        }

        string ty =  type2str( srcRgbImg.type() );
        message = "Matrix: " + ty + " " + to_string(srcRgbImg.cols) + "x" + to_string(srcRgbImg.rows);
        SHOW(message);

        Mat auxRgbMap = srcRgbImg.clone();
        //showResized(auxRgbMap, "auxRgbMap", resizeFactor, 0); //debug

        //Gray pixels to black:
        colorChangeAllRgb(auxRgbMap, COLORS.black, COLORS.grey);

        int blur_out = 3;
        lowThreshold = 70;

        auxRgbMap.copyTo(edgesRgbMap);

        medianBlur ( auxRgbMap, auxRgbMap, blur_out );
        Mat greyMap;
        cvtColor(auxRgbMap, greyMap, CV_RGB2GRAY);
        Canny( greyMap, edgesRgbMap, lowThreshold, lowThreshold*ratio, kernel_size );

        //that map will be compared with oryginal
        bitwise_not ( edgesRgbMap, edgesRgbMap );
        cvtColor(edgesRgbMap, edgesRgbMap, CV_GRAY2RGB);
        imwrite( "../maps/edges.pgm", edgesRgbMap );//to test brak edgesRgbMap?

        createVecOfMeanLines(srcRgbImg, edgesRgbMap);

        //remove last element of VecOfMeanPts
        //because it's being added "in case"
        VecOfMeanPts.pop_back();

        countAndDrawMeanLines(srcRgbImg);

        message = "after draw";
        SHOW(message);
        Point startPt, endPt;
        SHOW(VecOfMeanPts.size());
        Vec2f sumStdDevVsCount;

        for(unsigned int i=0; i<VecOfMeanPts.size(); i++)
        {
            SHOW("");
            startPt =  static_cast<Point>(*(VecOfMeanPts[i].meanPt.begin()));
            endPt = static_cast<Point>(*(VecOfMeanPts[i].meanPt.rbegin()));
            //write mean value
            if(VecOfMeanPts[i].direction == 1)
            {
                startPt.y = vecOfMeanVals[i][1];
                endPt.y   = vecOfMeanVals[i][1];
                //check if end greater than begin
                if(startPt.x > endPt.x)
                {
                    int temp = startPt.x;
                    startPt.x = endPt.x;
                    endPt.x 	 = temp;
                }
            }
            else if(VecOfMeanPts[i].direction == 2)
            {
                startPt.x = vecOfMeanVals[i][0];
                endPt.x   = vecOfMeanVals[i][0];
                //check if end greater than begin
                if(startPt.y > endPt.y)
                {
                    int temp = startPt.y;
                    startPt.y = endPt.y;
                    endPt.y 	 = temp;
                }
            }

            float stdDev = countStdDev(srcRgbImg, 10, startPt, endPt);
            SHOW(stdDev);
            sumStdDevVsCount[0] += stdDev;
            sumStdDevVsCount[1]++;
        }
        float finalStdDev = sumStdDevVsCount[0] / sumStdDevVsCount[1];
        SHOW(finalStdDev);

    }

	

	
	message = "at the end";
	SHOW(message);//debug
	
    //destroyAllWindows(); //For a simple program, you do not really have to call these functions because all the resources and windows of the application are closed automatically by the operating system upon exit.
  	return 0;
}


/// Create a window
//namedWindow( window_name, WINDOW_AUTOSIZE );
/// Create a Trackbar for user to enter threshold
//createTrackbar( "Min Lenght:", window_name, &minLenght, 100, CannyThreshold );
/// Show the image
//CannyThreshold(0, 0);
/// Wait until user exit program by pressing a key
//waitKey(0);

/*==OLD==:

void callFunctions(Mat &aRgbMapR, int width, int minLenght)
{
	
  	/// Reduce noise with a kernel 3x3
  	//blur( src_gray, detected_edges, Size(3,3) );
	//GaussianBlur( src_gray, gb, Size( 1, 1 ), 0, 0 );
	//medianBlur ( src_gray3, src_gray2, blur_out );
}
*/
/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
/*void CannyThreshold(int, void*)
{		
	//valuesChange.flip();
}*/

