#include "../segmentation/common.hpp"

class Robot{


protected:

  bool isBall;



public:
  //virtual bool (double composante1, double composante2);
  Robot(){
    isBall=false;
    nbPixelsBall=0;
  }
  virtual CvPoint barycentre(Mat frameModif);
  virtual bool marqueur(Mat frameModif, CvPoint barycentre);

  int nbPixelsBall;
private:

};

/**
 * Pour la nature des composantes, se référer à chaque classe en particulier.
 **/
CvPoint Robot::barycentre(Mat frameModif){

    int x,y;
    int sommeX = 0;
    int sommeY = 0;
    CvPoint barycentre = cvPoint(-1,-1);

    for(y = 0; y < frameModif.cols; y++) {
        for(x = 0; x < frameModif.rows; x++) {
            //Vec3b pixel=frameModif->at<vec3b>(y,x);

            if(frameModif.at<Vec3b>(x,y)==Vec3b(0,127,255))
            {
                sommeX += x;
                sommeY += y;
                nbPixelsBall++;
            }
        }
    }
    if(nbPixelsBall !=0)
        barycentre = cvPoint((sommeX/nbPixelsBall),(sommeY/nbPixelsBall));
    return barycentre;
}

bool Robot::marqueur(Mat frameModif, CvPoint barycentre){
    if(nbPixelsBall>400)
    {
        circle(frameModif,barycentre,10,CV_RGB(255,0,0),2);
        isBall=true;
        nbPixelsBall = 0;
    }
    return EXIT_SUCCESS;
}

