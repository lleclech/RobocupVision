#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

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

bool terrain(double R, double G){
    Mat mu(1,2,CV_64F),cov(2,2,CV_64F), point(1,2,CV_64F);
    double seuil=2000;
    //Mat::zeros(bbin*scale, abin*scale, CV_8UC3);
    mu.at<double>(0,0)=187.26119;
    mu.at<double>(0,1)=154.00789;
    point.at<double>(0,0)=R;
    point.at<double>(0,1)=G;
    cov.at<double>(0,0)=406.11914;
    cov.at<double>(0,1)=491.43532;
    cov.at<double>(1,0)=491.43532;
    cov.at<double>(1,1)=723.74627;
    return Mahalanobis(point,mu,cov)<seuil;
}

int main(){
    String path = "../data/log2/";
    String filename = "rgb";
    String ext=".png";
    int nbImg=150;

  // The two windows we'll be using
  namedWindow("image",1);
  namedWindow("modif image",1);

  Mat frame, frameModif;
  for(int nb=0;nb<nbImg;nb++){
      frame = imread(path+filename+to_string(nb)+ext);
      frameModif = frame;
    imshow("image", frame);
    for( int x = 0; x < frame.cols; x++ ){
        for( int y = 0; y < frame.rows; y++ ){
            Vec3b pixel=frame.at<Vec3b>(y,x);
            if(terrain(pixel[0],pixel[1])){
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
