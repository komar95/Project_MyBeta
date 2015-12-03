#ifndef MYMATH_H
#define MYMATH_H

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <thread>
#include "CNumber.h"
#include <png.h>
#include "PNGFile.h"


extern "C" {
  struct PNGFile *initImage(struct PNGFile *file, char* filename, int width, int height, char* title);
  void writeToImage(struct PNGFile *file, uint32_t *buffer);
  void finalise(struct PNGFile *file);
}

class MyMath{
  public:
    MyMath(int width, int height);
    ~MyMath();
    void calculate(char *filename);
    uint32_t *getImage();
    int getWdh();
    double getStartX();
    double getStartY();
    
  private:
    uint32_t getColor(double re, double im);
    //static void threadMethod(int i_start, int i_end, uint32_t **image, int width, int stepX, int stepY, std::vector<int> indexes);
    void fillColors();
    //static int diverges(CNumber a);
    int width_, height_;
    uint32_t *colors_;
    uint32_t *image_;
    static double START_X;
    static double END_X;
    static double START_Y;
    static double END_Y;
    static uint32_t START_COLOR;
    static uint32_t COLOR_STEP;
    static int WDH;
};

#endif
