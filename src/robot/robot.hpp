#include "../segmentation/common.hpp"

class Robot{


protected:


public:

  int nbPixelsBall;
  int nbPixelsGoal;
  bool isBall;
  int position;


  //virtual bool (double composante1, double composante2);
  Robot(){
    isBall=false;
    nbPixelsBall = 0;
    nbPixelsGoal = 0;
    position = -1;
  }
  virtual CvPoint barycentreBall(Mat frameModif);
  virtual CvPoint barycentreGoal(Mat frameModif);
  virtual bool marqueur(Mat frameModif, CvPoint barycentreBall, CvPoint barycentreGoal);
  virtual int goalPosition(Mat frameGoal, CvPoint barycentreGoal);

private:

};

/**
 * Pour la nature des composantes, se référer à chaque classe en particulier.
 **/

CvPoint Robot::barycentreBall(Mat frameModif){

    int x,y;
    int sommeX = 0;
    int sommeY = 0;
    CvPoint barycentre = cvPoint(-1,-1);
    nbPixelsBall = 0;
    isBall = false;

    for(x = 0; x < frameModif.cols; x++) {
        for(y = 0; y < frameModif.rows; y++) {

            if(frameModif.at<float>(y,x)==255)
            {
                sommeX += x;
                sommeY += y;
                nbPixelsBall++;
            }
        }
    }
    if(nbPixelsBall !=0)
    {
        barycentre = cvPoint((sommeX/nbPixelsBall),(sommeY/nbPixelsBall));
        isBall = true;
    }
    return barycentre;
}

CvPoint Robot::barycentreGoal(Mat frameModif){

    int x,y;
    int sommeX = 0;
    int sommeY = 0;
    CvPoint barycentre = cvPoint(-1,-1);
    nbPixelsGoal = 0;

    for(x = 0; x < frameModif.cols; x++) {
        for(y = 0; y < frameModif.rows; y++) {

            if(frameModif.at<float>(y,x)==255)
            {
                sommeX += x;
                sommeY += y;
                nbPixelsGoal++;
            }
        }
    }
    if(nbPixelsGoal != 0)
        barycentre = cvPoint((sommeX/nbPixelsGoal),(sommeY/nbPixelsGoal));
    return barycentre;
}

int Robot::goalPosition(Mat frameGoal, CvPoint barycentreGoal){
    int nbGauche =0;
    int nbDroite =0;
    int x,y;
    for(x = 0; x < frameGoal.cols; x++) {
        for(y = 0; y < frameGoal.rows; y++) {

            if(frameGoal.at<float>(y,x)==255 && x < barycentreGoal.x)
                nbGauche++;
            if(frameGoal.at<float>(y,x)==255 && x > barycentreGoal.x)
                nbDroite++;
        }
    }
    if(barycentreGoal.x == -1)
        return position =-1;
    else if((nbGauche - nbDroite) > 50)
        return position = 0;
    else if((nbDroite - nbGauche) > 50)
        return position = 2;
    else
        return position = 1;
}

bool Robot::marqueur(Mat frameModif, CvPoint barycentreBall, CvPoint barycentreGoal){
    if(nbPixelsBall>40)
    {
        circle(frameModif,barycentreBall,10,CV_RGB(255,0,0),2);
        isBall=true;
    }
    if(nbPixelsGoal>400)
    {
        circle(frameModif,barycentreGoal,10,CV_RGB(64,224,208),-1);
        isBall=true;
    }

    return EXIT_SUCCESS;
}


