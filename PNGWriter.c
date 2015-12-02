#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <stdint.h>
#include <png.h>
#include "PNGFile.h"

void setRGB(png_byte *ptr, uint32_t val)
{
  printf("ptr '%p'\n", ptr);
  ptr[0] = (val >> 16) & 0xFF;
  ptr[1] = (val >> 8) & 0xFF;
  ptr[2] = val & 0xFF;
}

void finalise(struct PNGFile *file){
  // End write
	png_write_end(file->png_ptr, NULL);

	if (file->fp != NULL) fclose(file->fp);
	if (file->info_ptr != NULL) png_free_data(file->png_ptr, file->info_ptr, PNG_FREE_ALL, -1);
	if (file->png_ptr != NULL) png_destroy_write_struct(&file->png_ptr, (png_infopp)NULL);
	if (file->row != NULL) free(file->row);
  free(file);
}

void writeToImage(struct PNGFile *file, uint32_t *buffer){
  // Write image data
	int x;
  for (x=0; x<file->width; x++) {
    printf("%d = %d\n", x, file->width);
    setRGB(&(file->row[x*2]), buffer[x]);
  }
  png_write_row(file->png_ptr, file->row);
}

struct PNGFile *initImage(char* filename, int width, int height, char* title)
{
  struct PNGFile *file = (struct PNGFile *)malloc(sizeof(struct PNGFile));
	file->fp = NULL;
	file->png_ptr = NULL;
	file->info_ptr = NULL;
	file->row = NULL;
  file->width = width;
	
	// Open file for writing (binary mode)
	file->fp = fopen(filename, "wb");
	if (file->fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		finalise(file);
	}

	// Initialize write structure
	file->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (file->png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		finalise(file);
	}

	// Initialize info structure
	file->info_ptr = png_create_info_struct(file->png_ptr);
	if (file->info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		finalise(file);
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(file->png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		finalise(file);
	}

	png_init_io(file->png_ptr, file->fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(file->png_ptr, file->info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(file->png_ptr, file->info_ptr, &title_text, 1);
	}

	png_write_info(file->png_ptr, file->info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	file->row = (png_byte *) malloc(3 * width * sizeof(png_byte));
  file->row[0] = 1;
  printf("%p, %p\n", file, file->row);
  return file;
}

//int cExample(){

	// Specify an output image size
	//int width = 2;
	//int height = 2;

	// Create a test image - in this case a Mandelbrot Set fractal
	// The output is a 1D array of floats, length: width * height
	//printf("Creating Image\n");
	//uint32_t buffer[] = {0xFF0000, 0x00FF00, 0x0000FF, 0x00FFFF};

	// Save the image to a PNG file
	// The 'title' string is stored as part of the PNG file
	//printf("Saving PNG\n");
	//int result = writeImage("img.png", width, height, buffer, "This is my test image");

	// Free up the memorty used to store the image
	//free(buffer);

	//return result;
//}
