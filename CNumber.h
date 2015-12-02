#ifndef CNUMBER_H
#define CNUMBER_H
#include <cmath>

class CNumber{
  public:
    CNumber();
    CNumber(double re, double im);
    
    void squareIt();
    void add(CNumber toAdd);
    double getAbs();
    double getIm();
    double getRe();
    
  private:
    double re_, im_;
    
};


#endif
