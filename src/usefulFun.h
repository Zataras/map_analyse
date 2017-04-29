/* File usefulFun.  */
#ifndef FILE_USEFULFUN_SEEN
#define FILE_USEFULFUN_SEEN

/*
Useful C/C++ functions for general purpose gathered by Zataras
Header file
*/

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <ostream>//font colors

#define SHOW(obj) cout << setw(3) << setfill('0') << __LINE__ << ": " << #obj << " = " << (obj) << "\n";//nice function for debug

using namespace cv;
using namespace std;

struct ColorsName
{
	const Vec3b blue;
	const Vec3b green;
	const Vec3b red;
	const Vec3b black;
	const Vec3b white;
	const Vec3b grey;
	const Vec3b yellow;
	const Vec3b orange;
	const Vec3b pink;
	const Vec3b violet;
	
	ColorsName() : 
		  blue(Vec3b(255,  0,  0)),
		 green(Vec3b(  0,255,  0)),
		   red(Vec3b(  0,  0,255)),
		 black(Vec3b(  0,  0,  0)),
		 white(Vec3b(255,255,255)),
		  grey(Vec3b(205,205,205)),
		yellow(Vec3b(  0,255,255)),
		orange(Vec3b(  0,153,255)),
		  pink(Vec3b(255,153,255)),
		violet(Vec3b(153,100,153))
	{}
};

//usage e.g. cout << Color::FG_RED << "text";
/*struct TermFontColors 
{
    const enum Code; //{
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
    //};
    TermFontColors():
    	 	Code(enum col {a:0,b:1})
 	 {}
    
    std::ostream& operator<<(std::ostream& os, Code code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }
}; */

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

/*const colors_name COLORS = 
{
	//	  (  B,  G,  R)
	Vec3b(255,  0,  0), 	//blue
	Vec3b(  0,255,  0), 	//green
	Vec3b(  0,  0,255), 	//red
	Vec3b(  0,  0,  0),	//black
	Vec3b(255,255,255),	//white
	Vec3b(205,205,205),	//grey
	Vec3b(  0,255,255),	//yellow
	Vec3b(  0,153,255),	//orange
	Vec3b(255,153,255),	//pink
	Vec3b(153,100,153)	//violet
};*/



/*from http://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
Here is a handy function you can use to help with identifying your opencv matrices at runtime. I find it useful for debugging, at least.
If M is a var of type Mat you can call it like so:

string ty =  type2str( M.type() );
printf("Matrix: %s %dx%d \n", ty.c_str(), M.cols, M.rows );

Will output data such as:

Matrix: 8UC3 640x480 
Matrix: 64FC1 3x2 

Its worth noting that there are also Matrix methods Mat::depth() and Mat::channels(). This function is just a handy way of getting a human readable interpretation from the combination of those two values whose bits are all stored in the same value.*/
string type2str(int type);

//To use with opencv library:
//displays input image resized with specified factor
//time specifies argument for WaitKey(timeMs)
void showResized(const Mat &srcImg, const string& winname, double factor, int timeMs);

#endif /* !FILE_USEFULFUN_SEEN */
