#include "openfile.h"

#include <math.h>
#define Mask_N 2
#define Mask_X 3
#define Mask_Y 3

int x=0;
uint8_t Offset[3]={2,1,0};
uint32_t Gxy[2]={0};//0:Y 1:X
uint8_t R=0,G=0,B=0;
uint8_t padding[1002*1002*3]={0};
float total=0;
void Dilation (bmp *Header_Data,uint8_t *Image_Data);
uint8_t out_data[1000*1000*3]={0};

int main (void) {

  FILE *lena=fopen("01.bmp","rb");
  FILE *output=fopen("output.bmp","wb");
  int Mask[2][3][3]={
    {{-1,-2,-1},
     {0 , 0, 0},
     {1 , 2, 1}},

    {{-1, 0, 1},
     {-2, 0, 2},
     {-1, 0, 1}}
  };


  bmp ini1={0};
  bmp *lena_header=&ini1;
  uint8_t *lena_data;
  Header_Read(lena_header,lena);
  uint32_t Total_Pixel_lena=(lena_header->Width_Bitmap)*(lena_header->Height_Bitmap)*(lena_header->Bit_per_Pixel/8);
  uint8_t byte=lena_header->Bit_per_Pixel/8;
  lena_data = malloc(Total_Pixel_lena);
  lena_data = Pixel_Read(lena_header,lena);
  lena_data = Grayscale(lena_header,lena_data);
//zero padding
  for (int row=1;row<1001;row++) {
    for (int column=1;column<1001;column++) {
      for(int count=0;count<3;count++) {
        padding[(row*1002+column)*byte+Offset[count]]=lena_data[((row-1)*1000+column-1)*byte+Offset[count]];
      }
    }
  }
//Sobel
  for (int row=0;row<lena_header->Height_Bitmap;row++) {
    for (int column=0;column<lena_header->Width_Bitmap;column++) {
      Gxy[0]=0;Gxy[1]=0;
        for (int v=0;v<3;v++) {
          for (int u=0;u<3;u++) {
            R = padding[((row+v)*1002+(column+u))*3 + Offset[0]];
            G = padding[((row+v)*1002+(column+u))*3 + Offset[1]];
            B = padding[((row+v)*1002+(column+u))*3 + Offset[2]];
            Gxy[0]+=(R+B+G)/3*Mask[0][v][u];
            Gxy[1]+=(R+B+G)/3*Mask[1][v][u];
          //  printf("%d %d %d\n",v,u,Mask[n][v][u]);
          }
        }
      total=sqrt(Gxy[0]*Gxy[0]+Gxy[1]*Gxy[1]);
        for(int count=0;count<3;count++) {
          if(total > 128) {
            lena_data[(row*lena_header->Width_Bitmap+column)*byte+Offset[count]]=1;
          }
          else {
            lena_data[(row*lena_header->Width_Bitmap+column)*byte+Offset[count]]=0;
          }
       }
    }
  }
//Dilation

  Dilation(lena_header,lena_data);

  Pixel_Write(lena_header,out_data,output);
}

void Dilation (bmp *Header_Data,uint8_t *Image_Data) {
  int Kernel[3][3]={{0,1,0},
                    {1,1,1},
                    {0,1,0}};
  uint8_t test_data[18]={0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8};
  int temp=0;
  uint16_t h=Header_Data->Height_Bitmap,w=Header_Data->Width_Bitmap;
  uint8_t  byte=Header_Data->Bit_per_Pixel/8;
  // h=6;w=3;
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
          out_data[(row*w+column)*byte+0]=255;
          out_data[(row*w+column)*byte+1]=255;
          out_data[(row*w+column)*byte+2]=255;
        }
        else {
          out_data[(row*w+column)*byte+0]=0;
          out_data[(row*w+column)*byte+1]=0;
          out_data[(row*w+column)*byte+2]=0;
        }
      }
    }
}
