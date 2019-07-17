#include "openfile.h"



void PSNR(uint8_t *Origin,uint8_t *LPF) ;

int main (void) {
  int i=0;
  FILE *picture=fopen("1.bmp","rb");
  FILE *output=fopen("output.bmp","wb");
  bmp  ini={0};
  bmp *picture_header=&ini;
  uint8_t *picture_data;
  Header_Read(picture_header,picture);
  uint32_t Total_Pixel_picture=(picture_header->Width_Bitmap)*(picture_header->Height_Bitmap)*(picture_header->Bit_per_Pixel/8);
  picture_data = (uint8_t*)malloc(Total_Pixel_picture);
  picture_data = Pixel_Read(picture_header,picture);

  //picture_data=DFT(picture_header,picture_data);
  picture_data=Filter(picture_data);




  Pixel_Write(picture_header,picture_data,output);
}


void PSNR(uint8_t *Origin_Data,uint8_t *LPF_Data) {
  float temp=0;
  for (int row=0;row<512;row++) {
    for (int column=0;column<512;column++) {
      temp+=(Origin_Data[row*512+column]-LPF_Data[row*512+column])^2;
    }
  }
  temp=temp/512/512;
  temp=10*log(255*255/temp);
  printf("%f\n",temp);
}
