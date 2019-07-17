#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include "openfile.h"
uint8_t  s8=sizeof(uint8_t);
uint16_t s16=sizeof(uint16_t);
uint32_t s32=sizeof(uint32_t);
uint8_t RGB_off[3]={2 ,1 ,0};//R G B
uint8_t out_data_Sobel[1000*1000*3]={0};
uint8_t out_data_Dilation[1000*1000*3]={0};
uint8_t out_data_Erosion[1000*1000*3]={0};

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
  fseek(image,Header_Data->Data_Offset,SEEK_SET);
  fread(data,sizeof(uint8_t),Total,image);
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
  fwrite(Image_Data,s8,Total,image);
  fclose(image);
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
      graygrid=(299*Data[row*w*(p/8)+column*(p/8)+RGB_off[0]]+
                587*Data[row*w*(p/8)+column*(p/8)+RGB_off[1]]+
                114*Data[row*w*(p/8)+column*(p/8)+RGB_off[2]])/1000;
      // graygrid=(Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[0])]+
      //           Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[1])]+
      //           Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[2])])/3;
      histogram[graygrid]++;
      Data[row*w*(p/8)+column*(p/8)+RGB_off[0]]=graygrid;
      Data[row*w*(p/8)+column*(p/8)+RGB_off[1]]=graygrid;
      Data[row*w*(p/8)+column*(p/8)+RGB_off[2]]=graygrid;
    }
  }
  for (int c=0;c<256;c++) {
    fprintf(histogram_data,"%d\n",histogram[c]);
  }
  fclose(histogram_data);
return Data;
}
/*-*/
uint8_t *Unnormalized(bmp *Header_Data,uint8_t *Image_Data) {
  uint16_t h=Header_Data->Height_Bitmap,w=Header_Data->Width_Bitmap;
  uint8_t byte=Header_Data->Bit_per_Pixel/8;
  for (int row=0;row<h;row++) {
    for (int column=0;column<w;column++) {
      Image_Data[(row*w+column)*byte+0]*=255;
      Image_Data[(row*w+column)*byte+1]*=255;
      Image_Data[(row*w+column)*byte+2]*=255;
    }
  }
  return Image_Data;
}
/*---Sobel-----*/
uint8_t *Sobel (bmp *Header_Data,uint8_t *Image_Data,uint8_t Thresh) {
  float Gxy[2]={0};
  float total=0;
  int Mask[2][3][3]={
    {{-1,-2,-1},
     {0 , 0, 0},
     {1 , 2, 1}},
    {{-1, 0, 1},
     {-2, 0, 2},
     {-1, 0, 1}}
  };
  uint16_t h=Header_Data->Height_Bitmap,w=Header_Data->Width_Bitmap;
  uint8_t  byte=Header_Data->Bit_per_Pixel/8;
  //Sobel
    for (int row=0;row<h;row++) {
      for (int column=0;column<w;column++) {
        Gxy[0]=0;Gxy[1]=0;total=0;
          for (int u=-1;u<2;u++) {
            for (int v=-1;v<2;v++) {
              if (row+u >=0 && column+v>=0 && column+v <w && row+u<h) {
                Gxy[0]+=Image_Data[((row+u)*w+column+v)*byte]*Mask[0][u+1][v+1];
                Gxy[1]+=Image_Data[((row+u)*w+column+v)*byte]*Mask[1][u+1][v+1];
              }
            }
          }
          total=sqrt(Gxy[0]*Gxy[0]+Gxy[1]*Gxy[1]);
            for(int count=0;count<3;count++) {
              if(total > Thresh) {
                out_data_Sobel[(row*w+column)*byte+RGB_off[count]]=1;
              }
              else {
                out_data_Sobel[(row*w+column)*byte+RGB_off[count]]=0;
              }
           }
      }
    }
    //靠北
    for (int count=0;count<w;count++) {
      out_data_Sobel[count*byte+0]=0;out_data_Sobel[(h-1)*w*byte+count*byte+0]=0;
      out_data_Sobel[count*byte+1]=0;out_data_Sobel[(h-1)*w*byte+count*byte+1]=0;
      out_data_Sobel[count*byte+2]=0;out_data_Sobel[(h-1)*w*byte+count*byte+2]=0;
      out_data_Sobel[count*w*byte+0]=0;out_data_Sobel[count*w*byte+(w-1)*byte+0]=0;
      out_data_Sobel[count*w*byte+1]=0;out_data_Sobel[count*w*byte+(w-1)*byte+1]=0;
      out_data_Sobel[count*w*byte+2]=0;out_data_Sobel[count*w*byte+(w-1)*byte+2]=0;
    }
    return out_data_Sobel;
}
/*-------Dilation--------*/
uint8_t *Dilation (bmp *Header_Data,uint8_t *Image_Data,uint8_t time) {
  int Kernel[3][3]={{0,1,0},
                    {1,1,1},
                    {0,1,0}};
  uint8_t test_data[18]={0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8};
  int temp=0;
  uint16_t h=Header_Data->Height_Bitmap,w=Header_Data->Width_Bitmap;
  uint8_t  byte=Header_Data->Bit_per_Pixel/8;
  // h=6;w=3;
  while(time >= 1) {
      for (int row=0;row<h;row++) {
        for (int column=0;column<w;column++) {
          temp=0;
          for (int u=-1;u<2;u++) {
            for (int v=-1;v<2;v++) {
              if (row+u >=0 && column+v>=0 && column+v <w && row+u<h) {
                // temp+=test_data[(row+u)*w+column+v]*Kernel[u+1][v+1];
                temp+=Image_Data[((row+u)*w+column+v)*byte]*Kernel[u+1][v+1];
              }
            }
          }
          if (temp>0) {
            out_data_Dilation[(row*w+column)*byte+0]=1;
            out_data_Dilation[(row*w+column)*byte+1]=1;
            out_data_Dilation[(row*w+column)*byte+2]=1;
          }
          else {
            out_data_Dilation[(row*w+column)*byte+0]=0;
            out_data_Dilation[(row*w+column)*byte+1]=0;
            out_data_Dilation[(row*w+column)*byte+2]=0;
          }
        }
      }
      if (time>1) {
        for (int row=0;row<h;row++) {
          for (int column=0;column<w;column++) {
            Image_Data[(row*w+column)*byte+0]=out_data_Dilation[(row*w+column)*byte+0];
            Image_Data[(row*w+column)*byte+1]=out_data_Dilation[(row*w+column)*byte+1];
            Image_Data[(row*w+column)*byte+2]=out_data_Dilation[(row*w+column)*byte+2];
          }
        }
      }time=time-1;
  }
  return out_data_Dilation;
}
/*----Erosion------*/
uint8_t *Erosion(bmp *Header_Data,uint8_t *Image_Data,uint8_t time) {
  int Kernel[3][3]={{0,1,0},
                    {1,1,1},
                    {0,1,0}};
  int temp=0;
  uint16_t h=Header_Data->Height_Bitmap,w=Header_Data->Width_Bitmap;
  uint8_t  byte=Header_Data->Bit_per_Pixel/8;

  while(time >= 1) {
      for (int row=0;row<h;row++) {
        for (int column=0;column<w;column++) {
          temp=0;
          for (int u=-1;u<2;u++) {
            for (int v=-1;v<2;v++) {
              if (row+u >=0 && column+v>=0 && column+v <w && row+u<h) {
                // temp+=test_data[(row+u)*w+column+v]*Kernel[u+1][v+1];
                temp+=Image_Data[((row+u)*w+column+v)*byte]*Kernel[u+1][v+1];
              }
            }
          }
          if (temp==5) {
            out_data_Erosion[(row*w+column)*byte+0]=1;
            out_data_Erosion[(row*w+column)*byte+1]=1;
            out_data_Erosion[(row*w+column)*byte+2]=1;
          }
          else {
            out_data_Erosion[(row*w+column)*byte+0]=0;
            out_data_Erosion[(row*w+column)*byte+1]=0;
            out_data_Erosion[(row*w+column)*byte+2]=0;
          }
        }
      }
      if (time>1) {
        for (int row=0;row<h;row++) {
          for (int column=0;column<w;column++) {
            Image_Data[(row*w+column)*byte+0]=out_data_Erosion[(row*w+column)*byte+0];
            Image_Data[(row*w+column)*byte+1]=out_data_Erosion[(row*w+column)*byte+1];
            Image_Data[(row*w+column)*byte+2]=out_data_Erosion[(row*w+column)*byte+2];
          }
        }
      }time=time-1;
  }
  return out_data_Erosion;
}
/*------Sorting-------*/
uint8_t *Sort(uint8_t *Data) {
  int num=8,temp=0;;
  for (uint8_t i=0;i<num;i++) {
    for (uint8_t j=0;j<num;j++) {
      temp=0;
      if(Data[i]<Data[j]) {
        temp=Data[i];
        Data[i]=Data[j];
        Data[j]=temp;
      }
    }
  }
  return Data;
}

uint16_t getMaximum(bmp *Header_Data,uint8_t *back_data) {
  uint16_t max=0;
  for (int i=0;i<(Header_Data->Width_Bitmap)*(Header_Data->Height_Bitmap)-1;i++) {
    if (back_data[i+1]>back_data[i]) max=back_data[i+1];
  }
  return max;
}
