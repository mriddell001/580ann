/*
 * File: bmp.h
 * Description: 
 *   Parse pixel value of .bmp value into a row major order array
 *    1 == black pixel, 0 == white pixel
 *   To use, pass in the name of a bmp file as a char*.  
 *    get_pixels returns a pointer to a the row major order vector<char>
 *    get_width and get_height will give the original dimensions of the picture
 */
#ifndef BMP_H
#define BMP_H
#include <iostream>
#include <vector>
#include <fstream>
#pragma pack(2)
class bmpParser {
   public:
      bmpParser(const char* file);
      int get_width()  {return width;}
      int get_height() {return height;}
      std::vector<char>* get_pixels() {return &pixels;}
      void display();
   private:
      void parse(std::fstream &file);
      int width;
      int height;
      std::vector<char> pixels;
};

/* .bmp header structures */
typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct tagBITMAPFILEHEADER {
   WORD  bfType;
   DWORD bfSize;
   WORD  bfReserved1;
   WORD  bfReserved2;
   DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
   DWORD biSize;
   LONG  biWidth;
   LONG  biHeight;
   WORD  biPlanes;
   WORD  biBitCount;
   DWORD biCompression;
   DWORD biSizeImage;
   LONG  biXPelsPerMeter;
   LONG  biYPelsPerMeter;
   DWORD biClrUsed;
   DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

#endif 
