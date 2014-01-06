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


  // The windows we'll be using
  namedWindow("image",1);
  //namedWindow("modif image",1);
  //namedWindow("But",1);
  //namedWindow("Balle",1);
  //namedWindow("Ligne",1);
  namedWindow("Terrain",1);

  // The three object we're interesting in
  Segmentation *but = new But();
  Segmentation *balle = new Balle();
  Segmentation *ligne = new Ligne();
  Segmentation *terrain = new Terrain();
  
  Robot *robot = new Robot();

  Mat frame, frameModif;

  for(int nb=0;nb<nbImg;nb++){
    /*
     * Reinitialise
     */
    frame = imread(path+filename+to_string(nb)+ext);//frame is in BGR
    frameModif=frame.clone();
    but->resetImg();
    balle->resetImg();
    ligne->resetImg();
    terrain->resetImg();

    /**
     * Segmentation
     **/
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
            but->img.at<uchar>(y,x)=255;
        }else if(balle->Mahalanobis(U,V)){
            frameModif.at<Vec3b>(y,x)=Vec3b(0,127,255);
            balle->img.at<uchar>(y,x)=255;
        }else if (terrain->Mahalanobis(R,G)){
            frameModif.at<Vec3b>(y,x)=Vec3b(0,255,0);
            terrain->img.at<uchar>(y,x)=255;
        }else if (ligne->Mahalanobis(G,B)){
            frameModif.at<Vec3b>(y,x)=Vec3b(255,255,255);
            ligne->img.at<uchar>(y,x)=255;

        }else{
          frameModif.at<Vec3b>(y,x)=Vec3b(0,0,0);
        }
      }
    }

    /**
     * Simplification v1
     **/
    //Mat frameCR=frameModif.clone();
    //erode(frameCR,frameCR,Mat(),Point(-1,-1),1);
    //dilate(frameCR,frameCR,Mat(),Point(-1,-1),3);

    /**
     * Erase noise from ball (Simplification v2)
     **/
    erode(balle->img,balle->img,Mat(),Point(-1,-1),1);
    dilate(balle->img,balle->img,Mat(),Point(-1,-1),3);
    Mat eraseBalle=terrain->img.clone();
    for(int x = 0; x < frame.cols; x++ ){
      for( int y = 0; y < frame.rows; y++ ){
	if( ligne->img.at<uchar>(y,x) != 0){
	  eraseBalle.at<uchar>(y,x) = ligne->img.at<uchar>(y,x);
	}
	else if( balle->img.at<uchar>(y,x) != 0){
	  eraseBalle.at<uchar>(y,x) = balle->img.at<uchar>(y,x);
	}
      }
    }
    //erode(eraseBalle,eraseBalle,Mat(),Point(-1,-1),3);
    //dilate(eraseBalle,eraseBalle,Mat(),Point(-1,-1),1);

    /*******************************************************/
    //Mat dst = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
    //vector<vector<Point> > contours;
    //vector<Vec4i> hierarchy;
    
    //findContours( eraseBalle, contours, hierarchy,
    //		  CV_RETR_CCOMP, CV_CHAIN_APPROX_TC89_L1 );

    //int idx = 0;
    //for( ; idx >= 0; idx = hierarchy[idx][0] ) {
    // Scalar color( rand()&255, rand()&255, rand()&255 );
    // drawContours( dst, hull/*contours*/, idx, color, CV_FILLED, 8, hierarchy );
    //}

    // int largestIndex = 0;
    // int largestContour = 0;
    // int secondLargestIndex = 0;
    // int secondLargestContour = 0;
    // for( int i = 0; i< contours.size(); i++ ){
    //   if(contours[i].size() > largestContour){
    // 	secondLargestContour = largestContour;
    // 	secondLargestIndex = largestIndex;
    // 	largestContour = contours[i].size();
    // 	largestIndex = i;
    //   }else if(contours[i].size() > secondLargestContour){
    // 	secondLargestContour = contours[i].size();
    // 	secondLargestIndex = i;
    //   }
    // }

    // Scalar color = Scalar(0,0,255);
    // drawContours( dst, contours, largestIndex, color, CV_FILLED, 8);
    // drawContours( dst, contours, secondLargestIndex, color, CV_FILLED, 8);
    /**********************************************************/


    for(int x = 0; x < frame.cols; x++ ){
      for( int y = 0; y < frame.rows; y++ ){
	if(eraseBalle.at<uchar>(y,x))
	  balle->img.at<uchar>(y,x)=balle->img.at<uchar>(y,x);
      }
    }

    /**
     * Barycenter detection
     **/
    CvPoint barycentre = robot->barycentre(frame);
    cout << "valeur x: " << barycentre.x << "valeur y: " << barycentre.y << "nb pixel" << robot->nbPixelsBall <<endl;
    robot->marqueur(frame,barycentre);

    /**
     * Display
     **/
    imshow("image", frame);
    //imshow("modif image", frameModif);
    //imshow("erode dilate",frameCR);
    //imshow("But", but->img);
    //imshow("Balle",balle->img);
    //imshow("Ligne", ligne->img);
    //imshow("Terrain",terrain->img);
    imshow("Terrain", eraseBalle); // DEBUG

    // Wait for a keypress
    int c = waitKey(1000);
    if(c!=-1){
      // If pressed, break out of the loop
      break;
    }
  }
  return EXIT_SUCCESS;
}
