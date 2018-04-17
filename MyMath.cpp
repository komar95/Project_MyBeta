#include "MyMath.h"

//9933x14043
double MyMath::START_X = -2.4;
double MyMath::END_X = 1.0;
double MyMath::START_Y = 1.20;
double MyMath::END_Y = -1.20;         //rgb
uint32_t MyMath::START_COLOR = 0x000000;
uint32_t MyMath::COLOR_STEP =  0x101010;
int MyMath::WDH = 1000;

void threadMethod(MyMath *context, int i_start, int i_end, int j, uint32_t *image, double stepX, double stepY, uint32_t *colors);
int diverges(CNumber a, int wdh);

MyMath::MyMath(int width, int height): width_(width), height_(height){
  image_ = new uint32_t[width_];
  colors_ = new uint32_t[30];
  fillColors();
}

MyMath::~MyMath(){
  delete[] image_;
  delete[] colors_;
}

void MyMath::calculate(char *filename){
  double stepX = ((double)std::abs(MyMath::START_X - MyMath::END_X)) / (double)width_;
  double stepY = ((double)std::abs(MyMath::START_Y - MyMath::END_Y)) / (double)height_;
  struct PNGFile *file = new struct PNGFile;
  
	// Allocate memory for one row (3 bytes per pixel - RGB)
  file->row = new unsigned char[3*width_];
  initImage(file, filename, width_, height_, filename);
  for(int i = 0; i < height_; i++){
    std::thread p1 (&threadMethod, this, 0,           width_/4,   i,  image_, stepX, stepY, colors_);
    std::thread p2 (&threadMethod, this, width_/4,    width_/2,   i,  image_, stepX, stepY, colors_);
    std::thread p3 (&threadMethod, this, width_/2,    3*width_/4, i,  image_, stepX, stepY, colors_);
    std::thread p4 (&threadMethod, this, 3*width_/4,  width_,     i,  image_, stepX, stepY, colors_);
    p1.join();
    p2.join();
    p3.join();
    p4.join();
    writeToImage((struct PNGFile *)file, image_);
    //printf("%f%%\n", ((float)i)/((float)height_));
  }
  finalise(((struct PNGFile *)file));
}

void threadMethod(MyMath *context, int i_start, int i_end, int j, uint32_t *image, double stepX, double stepY, uint32_t *colors){
  int i = i_start;
  int n = 0;
  for(; i < i_end; i++){
    n = diverges(CNumber(context->getStartX() + stepX*i, context->getStartY() - stepY*j), context->getWdh());
    if(n == 0)
      image[i] = colors[0];
    else{
      if(n < 60){
        image[i] = colors[n * 20/60+1];
      }else{
        image[i] = colors[20+((n - 60)*10/940)];
      }
    }
  }
}

uint32_t *MyMath::getImage(){
  return image_;
}

int MyMath::getWdh(){
  return MyMath::WDH;
}

double MyMath::getStartX(){
  return MyMath::START_X;
}

double MyMath::getStartY(){
  return MyMath::START_Y;
}

uint32_t MyMath::getColor(double re, double im){
  CNumber c(re, im);
  int i = diverges(c, 1000);
  if(i == 0){
    return colors_[0];
  }else{
    if(i > 70){
      return colors_[24+((i-70)*5/930)];
    }else{
      return colors_[i*25/70];
    }
  }
}

int diverges(CNumber a, int wdh){
  CNumber z;
  for(int i = 0; i < wdh; i++){
    z.squareIt();
    z.add(a);
    if(z.getAbs() > 1000.0d)
      return i;
  }
  return 0;
}

void MyMath::fillColors(){ // black to blue 
  int r = 0x5a, g = 0xff, b = 0x00; //c6c6ff r-5 g-5
  colors_[0] = 0;
  for(int i = 29;i > 0;i--){
    colors_[i] = (r << 16)+(g << 8)+b;
    r -= 2;
    g -= 5;
    b += 8;
  }
}
