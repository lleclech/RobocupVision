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
  String path = "../data/log8/";
  String filename = "rgb";
  String ext=".png";
  int nbImg=341;
  /*
   *    log1 -> 25
   *    log2 -> 150
   *    log3 -> 80
   *    log4 -> 51
   *    log5 -> 105
   *    log6 -> 70
   *    log7 -> 75
   *    log8 -> 341
    */


  // The two windows we'll be using
  namedWindow("image",1);
  namedWindow("modif image",1);
  namedWindow("But",1);
  namedWindow("Balle",1);
  namedWindow("Terrain",1);
  namedWindow("Ligne",1);

  // The three object we're interesting in
  Segmentation *but = new But();
  Segmentation *balle = new Balle();
  Segmentation *ligne = new Ligne();
  Segmentation *terrain = new Terrain();
  
  Robot *robot = new Robot();

  Mat frame, frameModif;

  for(int nb=0;nb<nbImg;nb++){
    frame = imread(path+filename+to_string(nb)+ext);//frame is in BGR
    frameModif=frame;
    imshow("image", frame);
    but->resetImg();
    terrain->resetImg();
    balle->resetImg();
    ligne->resetImg();
    for(int x = 0; x < frame.cols; x++ ){
      for( int y = 0; y < frame.rows; y++ ){
        Vec3b pixel=frame.at<Vec3b>(y,x);
        //RGB
        double R=pixel[2];
        double G=pixel[1];
        double B=pixel[0];

        //YUV convertion
        double Y=0.299*R+0.587*G+0.114*B;
        double U=0.492*(B-Y);
        double V=0.877*(R-Y);
        if(but->Mahalanobis(R,G)){
            frameModif.at<Vec3b>(y,x)=Vec3b(255,0,0);
            but->img.at<float>(y,x)=255;
        }else if(balle->Mahalanobis(U,V)){
            frameModif.at<Vec3b>(y,x)=Vec3b(0,127,255);
            balle->img.at<float>(y,x)=255;
        }else if (terrain->Mahalanobis(R,G)){
          frameModif.at<Vec3b>(y,x)=Vec3b(0,255,0);
          terrain->img.at<float>(y,x)=255;
        }else if (ligne->Mahalanobis(G,B)){
          frameModif.at<Vec3b>(y,x)=Vec3b(255,255,255);
          ligne->img.at<float>(y,x)=255;
        }else{
          frameModif.at<Vec3b>(y,x)=Vec3b(0,0,0);
        }
      }
    }
    //Mat frameCR=frameModif.clone();
    //erode(frameCR,frameCR,Mat(),Point(-1,-1),1);
    //dilate(frameCR,frameCR,Mat(),Point(-1,-1),3);
    CvPoint barycentre = robot->barycentre(frameModif);
    cout << "valeur x: " << barycentre.x << "valeur y: " << barycentre.y << "nb pixel" << robot->nbPixelsBall <<endl;
    robot->marqueur(frameModif,barycentre);
    imshow("modif image", frameModif);
    //imshow("erode dilate",frameCR);
    //imshow("But", but->img);
    //imshow("Balle",balle->img);
    //imshow("Ligne", ligne->img);
    //imshow("Terrain",terrain->img);
    // Wait for a keypress
    int c = waitKey(1000);
    if(c!=-1){
      // If pressed, break out of the loop
      break;
    }
  }
  return EXIT_SUCCESS;
}
