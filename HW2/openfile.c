#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "openfile.h"
uint8_t  s8=sizeof(uint8_t);
uint16_t s16=sizeof(uint16_t);
uint32_t s32=sizeof(uint32_t);
// uint8_t RGB_off[3]={2 ,1 ,0};//R G B
uint8_t  temp[1024]={0};
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
float Re[512][512];
float Im[512][512];
float F_Re[512][512];
float F_Im[512][512];
float sqrt_data[512*512];
float c=0;
int m[2]={1,-1};
int d[512*512]={0};
int count=0;
uint8_t *DFT(bmp *Header_Data,uint8_t *Data) {
  uint16_t h=Header_Data->Height_Bitmap;
  uint16_t w=Header_Data->Width_Bitmap;
  uint16_t b=Header_Data->Bit_per_Pixel/8;
  uint32_t t=h*w;
  for (uint32_t u=0;u<h;u++) {
    for (uint32_t v=0;v<w;v++) {
      Data[(u*w+v)*b]*=m[(u+v)%2];
    }
  }
  //1-D integrator
    for (int x=0;x<h;x++) {
      for (int v=0;v<w;v++) {
        for (int count=0;count<w;count++) {
          Re[x][v]+=Data[x*w+count]*cos(2*M_PI*v*count/w);
          Im[x][v]-=Data[x*w+count]*sin(2*M_PI*v*count/w);
          // printf("%f\n",Re[x][v]);
        }
      }
    }
    //2-D integrator
      for (uint32_t v=0;v<w;v++) {
        for (uint32_t u=0;u<h;u++) {
          for (count=0;count<h;count++) {
            F_Re[u][v]+=(Re[count][v]*cos(2*M_PI*u*count/h)+Im[count][v]*sin(2*M_PI*u*count/h));
            F_Im[u][v]+=(Im[count][v]*cos(2*M_PI*u*count/h)-Re[count][v]*sin(2*M_PI*u*count/h));
            // printf("%f\n",F_Re[u][v]);
          }
        }
      }
      //absolute value //Find maximum
      for (uint32_t row=0;row<h;row++) {
        for (uint32_t column=0;column<w;column++) {
          sqrt_data[row*w+column]=sqrt(F_Re[row][column]*F_Re[row][column]+F_Im[row][column]*F_Im[row][column]);
          //printf("%f\n",sqrt_data[row*w+column]);
          if (c < sqrt_data[row*w+column]) {c=sqrt_data[row*w+column];}
        }
      }
      //mapping
      c=255/log(1+c);
      printf("255/log(1+c)=%f\n",c);
      //
      for (count=0;count<t;count++) {
        Data[count]=(uint8_t)(c*log(1+sqrt_data[count]));
        //printf("%d\t",Data[count]);
      }
      for (count=0;count<2;count++) {
        for (int x=0;x<2;x++) {
          //printf("-%d\t",Data[count*512+x]);
        }
      }
return Data;
}

uint8_t *Filter(uint8_t *Origin_Data) {

  uint8_t padding[514*514]={0};
  uint8_t LPF1[512*512]={0};
  uint8_t LPF2[512*512]={0};
  int  Mask1[3][3]={{1,2,1},{2,4,2},{1,2,1}};
  int  Mask2[3][3]={{1,1,1},{1,2,1},{1,1,1}};
  for (int row=1;row<513;row++) {
    for (int column=1;column<513;column++) {
      padding[row*514+column]=Origin_Data[(row-1)*512+(column-1)];
    //  printf("%d\t",padding[row*541+column]);
    }
  }
  uint8_t x=0;
  printf("\n++++++++++++\n");
  for (int row=1;row<513;row++) {
    for (int column=1;column<513;column++) {
      x=( padding[(row-1)*514+(column-1)]*Mask1[0][0]+
          padding[(row-1)*514+(column-0)]*Mask1[0][1]+
          padding[(row-1)*514+(column+1)]*Mask1[0][2]+
          padding[(row  )*514+(column-1)]*Mask1[1][0]+
          padding[(row  )*514+(column-0)]*Mask1[1][1]+
          padding[(row  )*514+(column+1)]*Mask1[1][2]+
          padding[(row+1)*514+(column-1)]*Mask1[2][0]+
          padding[(row+1)*514+(column-0)]*Mask1[2][1]+
          padding[(row+1)*514+(column+1)]*Mask1[2][2] )/16;
      LPF1[(row-1)*512+(column-1)]=x;
    }
  }
  for (int row=1;row<513;row++) {
    for (int column=1;column<513;column++) {
      x=( padding[(row-1)*514+(column-1)]*Mask2[0][0]+
          padding[(row-1)*514+(column-0)]*Mask2[0][1]+
          padding[(row-1)*514+(column+1)]*Mask2[0][2]+
          padding[(row  )*514+(column-1)]*Mask2[1][0]+
          padding[(row  )*514+(column-0)]*Mask2[1][1]+
          padding[(row  )*514+(column+1)]*Mask2[1][2]+
          padding[(row+1)*514+(column-1)]*Mask2[2][0]+
          padding[(row+1)*514+(column-0)]*Mask2[2][1]+
          padding[(row+1)*514+(column+1)]*Mask2[2][2] )/10;
      LPF2[(row-1)*512+(column-1)]=x;
    }
  }
  for (int row=0;row<512;row++) {
    for (int column=0;column<512;column++) {
      Origin_Data[row*512+column]=Origin_Data[row*512+column]-LPF1[row*512+column];//-LPF2[row*512+column];
    }
  }
  return Origin_Data;
}
