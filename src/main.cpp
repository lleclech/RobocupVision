#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "segmentation/Segmentation.cpp"
 
using namespace std;
using namespace cv;
template<typename T>


std::string to_string( const T & Value )
{
  // utiliser un flux de sortie pour créer la chaîne
  std::ostringstream oss;
  // écrire la valeur dans le flux
  oss << Value;
  // renvoyer une string
  return oss.str();
}

int main(void){
  String path = "../data/log2/";
  String filename = "rgb";
  String ext=".png";
  int nbImg=150;

  // The two windows we'll be using
  namedWindow("image",1);
  namedWindow("modif image",1);

  // The three object we're interesting in
  Segmentation but = new But();
  //Segmentation ball = new Ball();
  //Segmentation ligne = new Ligne(); 
  //Segmentation terrain = new Terrain();
  
  Mat frame, frameModif;
  for(int nb=0;nb<nbImg;nb++){
    frame = imread(path+filename+to_string(nb)+ext);
    frameModif = frame;
    imshow("image", frame);
    for(int x = 0; x < frame.cols; x++ ){
      for( int y = 0; y < frame.rows; y++ ){
	Vec3b pixel=frame.at<Vec3b>(y,x);
	if(but.Mahalanobis(pixel[2],pixel[1])){
	  frameModif.at<Vec3b>(y,x)=Vec3b(0,0,0);
	}else{
	  frameModif.at<Vec3b>(y,x)=Vec3b(250,250,250);
	}
      }
    }
    imshow("modif image", frameModif);
      
    // Wait for a keypress
    int c = waitKey(1000);
    if(c!=-1){
      // If pressed, break out of the loop
      break;
    }
  }
  return EXIT_SUCCESS;
}
