#ifndef _openfile_h_
#define _openfile_h_
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
typedef struct BMP_HEADER {
  // Bitmap_file_header
  uint16_t Header_Field;
  uint32_t Size_Bmp;
  uint32_t Reserved;
  uint32_t Data_Offset;
  // DIB_header
  uint32_t Size_Bitmap_Header;
  uint32_t Width_Bitmap;
  uint32_t Height_Bitmap;
  uint16_t Color_Planes;
  uint16_t Bit_per_Pixel;
  uint32_t Compression;
  uint32_t Size_Image;
  uint32_t Resolution_Horizontal;
  uint32_t Resolution_Vertical;
  uint32_t Number_of_Colors;
  uint32_t Important_Colors;
}bmp;

void      Header_Read(bmp *Header_Data,FILE *image);
uint8_t  *Pixel_Read(bmp *Header_Data,FILE *image);
void      Pixel_Write(bmp *Header_Data,uint8_t *Image_Data,FILE *image);
uint8_t *Reverse(bmp *Header_Data,uint8_t *Data);
uint8_t *Grayscale(bmp *Header_Data,uint8_t *Data);
uint8_t *Cover(bmp *lena_Header_Data,bmp *water_Header_Data,uint8_t *lena,uint8_t *water);

#endif
