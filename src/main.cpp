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


/**
 * OSEF -> pour parcourir l'ensemble des fichiers
 *         d'un répertoire
 **/
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
   * INITIALISATION BDD  
   *
   *    log1 -> 25
   *    log2 -> 150
   *    log3 -> 80
   *    log4 -> 51
   *    log5 -> 105
   *    log6 -> 70
   *    log7 -> 75
   *    log8 -> 341
   */


  // Les fenêtres que nous utiliserons
  namedWindow("image",1);
  //namedWindow("modif image",1);
  namedWindow("But",1);
  //namedWindow("Balle",1);
  //namedWindow("Ligne",1);
  //namedWindow("Terrain",1);

  // Les objets qui nous intéressent
  Segmentation *but = new But();
  Segmentation *balle = new Balle();
  Segmentation *ligne = new Ligne();
  Segmentation *terrain = new Terrain();
  Robot *robot = new Robot();
  Mat frame, frameModif;


  /**
   * Boucle principale : sur l'ensemble des images, ...
   **/
  for(int nb=0;nb<nbImg;nb++){
    /*
     * On (ré)initialise
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
     * Atténuation du bruit pour la balle
     **/
    erode(balle->img,balle->img,Mat(),Point(-1,-1),1);
    dilate(balle->img,balle->img,Mat(),Point(-1,-1),3);
    Mat eraseBalle = terrain->img.clone();
    //eraseLine = terrain->img.clone();
    erode(eraseBalle,eraseBalle,Mat(),Point(-1,-1),3);
    //dilate(eraseLine,eraseLine,Mat(),Point(-1,-1),0);


    /**
     * Elaboration d'un filtre (eraseBall)
     **/
    vector<vector<Point> > contours;    
    findContours( eraseBalle, contours,
    		  CV_RETR_LIST, CV_CHAIN_APPROX_TC89_L1 );
    // Calcule le bord des rectangles et l'affiche
    for(int idx=0; idx<contours.size();idx++) {
      Rect rect = boundingRect(contours[idx]);
      Point pt1, pt2;
      pt1.x = rect.x;
      pt1.y = rect.y;
      pt2.x = rect.x + rect.width;
      pt2.y = rect.y + rect.height;
      // Dessine le rectangle dans le filtre
      rectangle(eraseBalle, pt1, pt2, CV_RGB(255,255,255), -1);
    }


    /**
     * Elaboration d'un filtre pour les buts
     **/
    Mat eraseGoal = but->img.clone();
    Canny(eraseGoal, eraseGoal, 50, 200, 3);
    vector<Vec4i> lines;
    //HoughLines(eraseGoal, lines, 1, CV_PI/180, 100, 0, 0);
    HoughLinesP(eraseGoal, lines, 1.5, CV_PI/2, 40);
    
    for( size_t i = 0; i < lines.size(); i++ ){
      line( eraseGoal, Point(lines[i][0], lines[i][1]),
	    Point(lines[i][2], lines[i][3]), 255, 8, CV_AA );
    }
    /*for( size_t i = 0; i < lines.size(); i++ ){
      float rho = lines[i][0], theta = lines[i][1];
      Point pt1, pt2;
      double a = cos(theta), b = sin(theta);
      double x0 = a*rho, y0 = b*rho;
      pt1.x = cvRound(x0 + 1000*(-b));
      pt1.y = cvRound(y0 + 1000*(a));
      pt2.x = cvRound(x0 - 1000*(-b));
      pt2.y = cvRound(y0 - 1000*(a));
      line(eraseGoal, pt1, pt2, 255, 3, CV_AA);
    }*/

    /**
     * Filtre eraseBalle appliqué à l'image de la balle
     * (et aux lignes/terrains ?)
     **/
    for(int x = 0; x < frame.cols; x++ ){
      for( int y = 0; y < frame.rows; y++ ){
    	// BALLE
	if((eraseBalle.at<uchar>(y,x) != 0) && (balle->img.at<uchar>(y,x) != 0))
    	  balle->img.at<uchar>(y,x)=255;
    	else
    	  balle->img.at<uchar>(y,x)=0;
	
	// LIGNES
    	if((eraseBalle.at<uchar>(y,x) != 0) && (ligne->img.at<uchar>(y,x) != 0))
    	  ligne->img.at<uchar>(y,x)=255;
    	else
    	  ligne->img.at<uchar>(y,x)=0;	
	
	//TERRAINS
    	if((eraseBalle.at<uchar>(y,x) != 0) && (terrain->img.at<uchar>(y,x) != 0))
    	  terrain->img.at<uchar>(y,x)=255;
    	else
    	  terrain->img.at<uchar>(y,x)=0;	
      }
    }


    /**
     * Detection des barycentres (balle et buts)
     **/
    Mat frameBall=balle->img.clone();
    erode(frameBall,frameBall,Mat(),Point(-1,-1),2);
    dilate(frameBall,frameBall,Mat(),Point(-1,-1),3);

    Mat frameGoal=but->img.clone();
    erode(frameGoal,frameGoal,Mat(),Point(-1,-1),2);
    dilate(frameGoal,frameGoal,Mat(),Point(-1,-1),3);

    CvPoint barycentreBall = robot->barycentreBall(frameBall);
    CvPoint barycentreGoal = robot->barycentreGoal(frameGoal);
    robot->marqueur(frameModif,barycentreBall,barycentreGoal);
    robot->goalPosition(frameGoal,barycentreGoal);
    cout << robot->position << endl;


    /**
     * Reconstruction de l'image
     **/
    for(int x = 0; x < frame.cols; x++ ){
      for( int y = 0; y < frame.rows; y++ ){
        if(but->img.at<uchar>(y,x)!=0){
	  frameModif.at<Vec3b>(y,x)=Vec3b(255,0,0);
        }else if(balle->img.at<uchar>(y,x) != 0){
	  frameModif.at<Vec3b>(y,x)=Vec3b(0,127,255);
        }else if (terrain->img.at<uchar>(y,x) != 0){
	  frameModif.at<Vec3b>(y,x)=Vec3b(0,255,0);
        }else if (ligne->img.at<uchar>(y,x) != 0){
	  frameModif.at<Vec3b>(y,x)=Vec3b(255,255,255);
        }else{
          frameModif.at<Vec3b>(y,x)=Vec3b(0,0,0);
        }
      }
    }


    /**
     * Affichage
     **/
    imshow("image", frame);
    //imshow("modif image", frameModif);
    //imshow("erode dilate",frameCR);
    imshow("But", eraseGoal);
    //imshow("Balle",frameBall);
    //imshow("Balle",balle->img);
    //imshow("Ligne", ligne->img);
    //imshow("Terrain",terrain->img);
    //imshow("Terrain", eraseBalle); // DEBUG

    // Pause et attente d'un évènement
    int c = waitKey(1000);
    if(c!=-1){
      // Si touche préssée, on sort de la boucle
      break;
    }
  }
  
  return EXIT_SUCCESS;
}
