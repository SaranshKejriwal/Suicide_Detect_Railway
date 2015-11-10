#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
	using namespace std;

int main ()
{
	
Mat frame;//consistant of Track ROI
Mat back;//background of ROI
Mat fore;//foreground of ROI
Mat input;//takes video stream from camera
int flag = 0;
  
VideoCapture cap1("model.mp4");/*to capture from pre-designed video*/
  
//Background Subtraction Part_____________________________________________________________
BackgroundSubtractorMOG2 bg;
bg.set ("nmixtures", 10);
bool bShadowDetection = false;
vector < vector < Point > >contours;
double ti=getTickCount()/getTickFrequency();
namedWindow("Frame");
cvMoveWindow("Frame",0,0);//to relocate the "Frame" window to desired coordinates
namedWindow( "Output");
namedWindow("Proc");
cvMoveWindow("Output",730,70);
cvMoveWindow("Proc",520,70);
namedWindow("Input");
cvMoveWindow("Input",70,70);//from top left
for (;;)
    {
	double t1= getTickCount();
	cap1 >> input;
	Rect r(120,0,185,input.rows);//ROI for model.wmv
 	frame=input(r).clone();//change this region to get calibrated ROI
	bg.operator()(frame, fore);
      	bg.getBackgroundImage (back);
      	erode (fore, fore, cv::Mat ());//to remove image noise
     	erode (fore, fore, cv::Mat ());//to remove image noise more, but reduces contour size
     	dilate (fore, fore, cv::Mat ());// 
     	dilate (fore, fore, cv::Mat ());// 
     	dilate (fore, fore, cv::Mat ());// sharpen contour without noise
     	findContours (fore, contours, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);//CHAIN_APPROX_NONE ensures that straight lines of contour are also included
	drawContours (frame, contours, -1, Scalar (255, 255, 255), 1);
	imshow("Input",input);
	
	    
	 
	if (cv::waitKey(15)>=10)
       break;

	//Rectangles Part________________________________________________
	    Mat drawing = Mat::zeros( fore.size(), CV_8UC3 );//to initialize drawing to a blank matrix
	    Scalar color = Scalar( 100, 100, 100);
	    int a=0;
	 	vector<Rect> boundRect( contours.size() );
      double t2;
	  
 	  for( int i = 0; i < contours.size(); i++ )
	  {
            boundRect[i] = boundingRect( contours[i] );
		   //color coding part____________________________________________________________________________________
       if(boundRect[i].width>=20 && boundRect[i].height>=40)//eliminates small boxes
	   {
		  if(boundRect[i].x+boundRect[i].width>drawing.cols/4 && boundRect[i].x<drawing.cols/1.333 && boundRect[i].height>120 && (getTickCount()/getTickFrequency()-ti)>2)//ensures a 2 second gap to eliminate noise and that blob is central
			 { 
				
				 color=Scalar(0,0,255);//red
				 putText(drawing,"Suicide Detected",Point(5,30),FONT_HERSHEY_TRIPLEX,0.6,Scalar(0,255,255),1);
				 putText(frame,"Suicide Detected",Point(5,30),FONT_HERSHEY_TRIPLEX,0.6,Scalar(0,255,255),1);
				 t2= getTickCount();
				int n= (t2-t1)/getTickFrequency(); 	 
			 
				}
		drawContours( drawing, contours, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
		rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 1,0);//0 decides that the rectangle superimposes contour
		rectangle( frame, boundRect[i].tl(), boundRect[i].br(), color, 1,0);//0 decides that the rectangle superimposes contour
		}
	 }
    	
	imshow( "Output", drawing );
	imshow("Proc",frame);
	//char name[10];
	//sprintf(name, "Frame%d.jpeg ", j+1);//to print a variable text on screen.
	//imwrite(name,frame);
}
  return 1;
}
