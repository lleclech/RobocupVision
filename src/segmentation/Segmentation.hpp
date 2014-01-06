#include "common.hpp"

class Segmentation{
protected:
  Segmentation(double _seuil, double _e1, double _e2, double _e3, 
	       double _e4, double _mu1, double _mu2){
    seuil = _seuil;
    e1 = _e1;
    e2 = _e2;
    e3 = _e3;
    e4 = _e4;
    mu1 = _mu1;
    mu2 = _mu2;
  }
  
public:
  virtual bool Mahalanobis(double composante1, double composante2);
  
private:
  double seuil, e1, e2, e3, e4, mu1, mu2;
};


/**
 * Pour la nature des composantes, se référer à chaque classe en particulier.
 **/
bool Segmentation::Mahalanobis(double composante1, double composante2){
  double a = composante2-mu1,
    b = composante2 - mu2;
  double det = e1*e4-e2*e3;
  double eps1 = e4/det,
    eps2 = - e2/det,
    eps3 = - e3/det,
    eps4 = e1/det;
  return  eps1*a*a + eps2*a*b + eps3*a*b + eps4*b*b < seuil*seuil;
}
