#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "segmentation/common.hpp"
 
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
  Segmentation *but = new But();
  Segmentation *balle = new Balle();
  Segmentation *ligne = new Ligne();
  Segmentation *terrain = new Terrain();
  
  Mat frame, frameModif;

  for(int nb=0;nb<nbImg;nb++){
    frame = imread(path+filename+to_string(nb)+ext);//img are in BGR
    imshow("image", frame);
    for(int x = 0; x < frame.cols; x++ ){
      for( int y = 0; y < frame.rows; y++ ){
        Vec3b pixel=frame.at<Vec3b>(y,x);
        //RGB
        float R=pixel[2];
        float G=pixel[1];
        float B=pixel[0];

        //YUV convertion
        float Y=0.299*R+0.587*G+0.114*B;
        float U=0.492*(B-Y);
        float V=0.877*(R-Y);

        if(balle->Mahalanobis(U,V)){
          frameModif.at<Vec3b>(y,x)=Vec3b(0,127,255);
        }else if(but->Mahalanobis(R,G)){
          frameModif.at<Vec3b>(y,x)=Vec3b(255,0,0);
        }else if (ligne->Mahalanobis(G,B)){
          frameModif.at<Vec3b>(y,x)=Vec3b(0,0,0);
        }else if (terrain->Mahalanobis(R,G)){
          frameModif.at<Vec3b>(y,x)=Vec3b(0,255,0);
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
