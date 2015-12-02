#ifndef PNGFILE_H
#define PNGFILE_H

struct PNGFile{
  FILE *fp;
  png_structp png_ptr;
  png_infop info_ptr;
  png_byte *row;
  int width;
};

#endif
