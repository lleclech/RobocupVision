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
  /*Mat mu(1,2,CV_64F),cov(2,2,CV_64F), point(1,2,CV_64F);
  double seuil=2000;
  //Mat::zeros(bbin*scale, abin*scale, CV_8UC3);
  mu.at<double>(0,0)=;
  mu.at<double>(0,1)=;
  point.at<double>(0,0)=R;
  point.at<double>(0,1)=G;
  cov.at<double>(0,0)=;
  cov.at<double>(0,1)=;
  cov.at<double>(1,0)=;
  cov.at<double>(1,1)=;
  return Mahalanobis(point,mu,cov)<seuil;
  */
  double seuil = 4.;
  double e1 = 406.11914,
    e2 = 491.43532,
    e3 = 491.43532,
    e4 = 723.74627;
  double mu1 = 187.26119,
    mu2 = 154.00789;

  double a = R-mu1,
    b = G - mu2;
  double det = e1*e4-e2*e3;
  double eps1 = e4/det,
    eps2 = - e2/det,
    eps3 = - e3/det,
    eps4 = e1/det;
  return  eps1*a*a + eps2*a*b + eps3*a*b + eps4*b*b < seuil;
}

int main(void){
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
            if(terrain(pixel[2],pixel[1])){
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
