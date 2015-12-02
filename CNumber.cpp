#include "CNumber.h"

CNumber::CNumber(): re_(0), im_(0) { }

CNumber::CNumber(double re, double im): re_(re), im_(im) {}

double CNumber::getIm(){
  return im_;
}

double CNumber::getRe(){
  return re_;
}

void CNumber::squareIt(){
  double tmp_re = re_*re_ - im_*im_;
  im_ = 2*re_*im_;
  re_ = tmp_re;
}

void CNumber::add(CNumber toAdd){
  re_ += toAdd.getRe();
  im_ += toAdd.getIm();
}

double CNumber::getAbs(){
   return re_ * re_ + im_ * im_;
}
