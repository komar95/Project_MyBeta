#include <string>
#include <iostream>
#include "MyMath.h"

int main(int argc, char **argv){
  if(argc < 4){
    std::cout << "Usage " << argv[0] << " <width> <height> <filename>" << std::endl;
    return -1;
  }
  int width = std::stoi(argv[1]);
  int height = std::stoi(argv[2]);
  MyMath math(width, height);
  math.calculate(argv[3]);
  
  //writeImage(argv[3], width, height, math.getImage(), argv[3]);
}
