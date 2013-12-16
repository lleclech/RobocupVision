#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(){
  // Initialize capturing live feed from the camera
  VideoCapture capture(0);
  //printf("%f",capture.get(CV_CAP_PROP_FRAME_WIDTH));
  //printf("%f",capture.get(CV_CAP_PROP_FRAME_HEIGHT));

  // Couldn't get a device? Throw an error and quit
  if(!capture.isOpened()){
    printf("Could not initialize capturing...\n");
    return -1;
  }

  // The two windows we'll be using
  namedWindow("video",1);
  namedWindow("modifVideo",1);

  Mat frame, frameModif;
  while(true){
    capture >> frame;
    pyrDown(frame, frame, Size(frame.cols/2,frame.rows/2));
    cvtColor(frame,frameModif,CV_BGR2GRAY);
    
    for(int i=0; i<frame.rows; i++){
      for(int j=0; j<frame.cols; j++){
    	frameModif.at<char>(i,j) = 255;
      }
    }
    //inRange(frameModif,Scalar(0,20,20),Scalar(100,230,230), frameModif);
    
    imshow("video", frame);
    imshow("modifVideo", frameModif);

    // Wait for a keypress
    int c = waitKey(10);
    if(c!=-1){
      // If pressed, break out of the loop
      break;
    }
  }
  return EXIT_SUCCESS;
}
