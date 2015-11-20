#include "bmp.h"
using namespace std;

bmpParser::bmpParser(const char* file)
{
   fstream f(file);
   if (f) {
      parse(f);
   } else {
      cerr << "Couldn't open up file: " << f << endl;
   }
}

void bmpParser::parse(fstream &file)
{
   vector<unsigned char> buffer;
   streampos length;
   PBITMAPFILEHEADER file_header;
   PBITMAPINFOHEADER info_header;
   int offset;
   int num_pixels;
   int padding;
   int t_padding;

   file.seekg(0, ios::end);
   length = file.tellg();
   file.seekg(0, ios::beg);
   buffer.resize(length);
   file.read((char*) &buffer[0], length);

   file_header = (PBITMAPFILEHEADER) (&buffer[0]);
   info_header = (PBITMAPINFOHEADER) (&buffer[0] + sizeof(BITMAPFILEHEADER));

   width  = info_header->biWidth;
   height = info_header->biHeight;
   offset = file_header->bfOffBits;
   num_pixels = width * height;
   pixels.resize(num_pixels);

   /*
    * .bmp files pad the end of each line so that they are a multiple of 4 bytes
    * And while they are also a effictively a row major order array, the first 
    *  pixel is the bottom left pixel, as opposed to the top left
    * This loop fixes both those problems
    * Also each pixel is 3 bytes long, in BGR order, though that isn't
    *  important for what this program is doing
    */
   padding   = (width * 3) % 4 ? 4 - ((width * 3) % 4) : 0;
   t_padding = padding * height; 
   for (int i = offset, j = pixels.size() - width; 
        i < offset + num_pixels*3 + t_padding; i += 3, j++)
   {
      if (buffer[i] != 255) {
         pixels[j] = 1;
      } 
      else {
         pixels[j] = 0;
      }
      if (j % width == width - 1) {
         j -= 2 * width;
         i += padding;
      }
   }
}

//For testing
void bmpParser::display() 
{
   for (int i = 0; i < pixels.size(); i++) {
      cout << (int) pixels[i];
      if (i % width == width - 1) {
         cout << endl;
      }
   }
}

/*
int main(int argc, char **argv)
{
   bmpParser BP(argv[1]);
   BP.display();
}
*/
