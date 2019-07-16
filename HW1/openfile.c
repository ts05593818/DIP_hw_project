#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "openfile.h"
uint8_t  s8=sizeof(uint8_t);
uint16_t s16=sizeof(uint16_t);
uint32_t s32=sizeof(uint32_t);
uint8_t  temp[1024]={0};
uint8_t RGB_off[3]={2 ,1 ,0};//R G B
void Header_Read(bmp *Header_Data,FILE *image) {
  fread(&Header_Data->Header_Field,s8,s16,image);
  fread(&Header_Data->Size_Bmp,s8,s32,image);
  fread(&Header_Data->Reserved,s8,s32,image);
  fread(&Header_Data->Data_Offset,s8,s32,image);
  fread(&Header_Data->Size_Bitmap_Header,s8,s32,image);
  fread(&Header_Data->Width_Bitmap,s8,s32,image);
  fread(&Header_Data->Height_Bitmap,s8,s32,image);
  fread(&Header_Data->Color_Planes,s8,s16,image);
  fread(&Header_Data->Bit_per_Pixel,s8,s16,image);
  fread(&Header_Data->Compression,s8,s32,image);
  fread(&Header_Data->Size_Image,s8,s32,image);
  fread(&Header_Data->Resolution_Horizontal,s8,s32,image);
  fread(&Header_Data->Resolution_Vertical,s8,s32,image);
  fread(&Header_Data->Number_of_Colors,s8,s32,image);
  fread(&Header_Data->Important_Colors,s8,s32,image);
}

uint8_t *Pixel_Read(bmp *Header_Data,FILE *image) {
  //RGB 3*8bit
  uint32_t Total=(Header_Data->Width_Bitmap)*(Header_Data->Height_Bitmap)*(Header_Data->Bit_per_Pixel/8);
  uint8_t *data=malloc(Total);
  fseek(image,Header_Data->Data_Offset-1024,SEEK_SET);
  fread(temp,s8,1024,image);
  fseek(image,Header_Data->Data_Offset,SEEK_SET);
  fread(data,s8,Total,image);
  return data;
}

void Pixel_Write(bmp *Header_Data,uint8_t *Image_Data,FILE *image){
  uint32_t Total=(Header_Data->Width_Bitmap)*(Header_Data->Height_Bitmap)*(Header_Data->Bit_per_Pixel/8);
  fwrite(&Header_Data->Header_Field,s8,s16,image);
  fwrite(&Header_Data->Size_Bmp,s8,s32,image);
  fwrite(&Header_Data->Reserved,s8,s32,image);
  fwrite(&Header_Data->Data_Offset,s8,s32,image);
  fwrite(&Header_Data->Size_Bitmap_Header,s8,s32,image);
  fwrite(&Header_Data->Width_Bitmap,s8,s32,image);
  fwrite(&Header_Data->Height_Bitmap,s8,s32,image);
  fwrite(&Header_Data->Color_Planes,s8,s16,image);
  fwrite(&Header_Data->Bit_per_Pixel,s8,s16,image);
  fwrite(&Header_Data->Compression,s8,s32,image);
  fwrite(&Header_Data->Size_Image,s8,s32,image);
  fwrite(&Header_Data->Resolution_Horizontal,s8,s32,image);
  fwrite(&Header_Data->Resolution_Vertical,s8,s32,image);
  fwrite(&Header_Data->Number_of_Colors,s8,s32,image);
  fwrite(&Header_Data->Important_Colors,s8,s32,image);
  fwrite(temp,s8,1024,image);
  fwrite(Image_Data,s8,Total,image);
  fclose(image);
}

uint8_t *Reverse(bmp *Header_Data,uint8_t *Data) {
  uint32_t w,h;
  uint16_t p;
  w=Header_Data->Width_Bitmap;
  h=Header_Data->Height_Bitmap;
  p=Header_Data->Bit_per_Pixel;
  // memory use
  uint8_t *Reverse_Data=malloc((w)*(h)*(p/8));
  for (uint32_t row=0;row<h;row++) {
    for (uint32_t column=0;column<w;column++) {
    //R
    Reverse_Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[0])]
    =Data[(row*w+w)*(p/8)-column*(p/8)-(1+RGB_off[0])];
    //G
    Reverse_Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[1])]
    =Data[(row*(w)+w)*(p/8)-column*(p/8)-(1+RGB_off[1])];
    // B
    Reverse_Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[2])]
    =Data[(row*w+w)*(p/8)-column*(p/8)-(1+RGB_off[2])];
  }
}
return Reverse_Data;
}

uint8_t *Grayscale(bmp *Header_Data,uint8_t *Data) {
  uint32_t w,h;
  uint16_t p;
  uint8_t graygrid;
  FILE * histogram_data = fopen("histogram.txt","w");
  uint16_t histogram[256]={0};
  w=Header_Data->Width_Bitmap;
  h=Header_Data->Height_Bitmap;
  p=Header_Data->Bit_per_Pixel;
  for (uint32_t row=0;row<h;row++) {
    for(uint32_t column=0;column<w;column++) {
      graygrid=(299*Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[0])]+
                587*Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[1])]+
                114*Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[2])])/1000;
      // graygrid=(1*Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[0])]+
      //           2*Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[1])]+
      //           1*Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[2])])>>2;
      histogram[graygrid]++;
      Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[0])]=graygrid;
      Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[1])]=graygrid;
      Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[2])]=graygrid;
    }
  }
  for (int c=0;c<256;c++) {
    fprintf(histogram_data,"%d\n",histogram[c]);
  }
  fclose(histogram_data);
return Data;
}

uint8_t *Cover(bmp *lena_Header_Data,bmp *water_Header_Data,uint8_t *lena,uint8_t *water) {
  uint32_t w_l,h_l,w_w,h_w;
  uint16_t b_l,b_w;
  w_l=lena_Header_Data->Width_Bitmap;
  h_l=lena_Header_Data->Height_Bitmap;
  w_w=water_Header_Data->Width_Bitmap;
  h_w=water_Header_Data->Height_Bitmap;
  b_l=(lena_Header_Data->Bit_per_Pixel)/8;
  b_w=(lena_Header_Data->Bit_per_Pixel)/8;
  for (uint32_t row=10;row<164;row++)  {
    for (uint32_t column=112;column<190;column++) {
      lena[(row*w_l+column)*b_l+(2-RGB_off[0])]=water[((row+4)*w_w+column-60)*b_w+(2-RGB_off[0])];
      lena[(row*w_l+column)*b_l+(2-RGB_off[1])]=water[((row+4)*w_w+column-60)*b_w+(2-RGB_off[1])];
      lena[(row*w_l+column)*b_l+(2-RGB_off[2])]=water[((row+4)*w_w+column-60)*b_w+(2-RGB_off[2])];
    }
  }
  return lena;
}
