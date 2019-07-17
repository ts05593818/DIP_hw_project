#include "openfile.h"

#include <math.h>
#define Mask_N 2
#define Mask_X 3
#define Mask_Y 3
uint8_t *Connected_component_labeling(bmp *Header_Data,uint8_t *Image_Data);
// int x=0;
// uint8_t Offset[3]={2,1,0};
uint8_t testmax[48]={0,0,0,0,0,0,0,0,0,0,0,0,
                     0,1,1,1,1,0,0,0,1,0,0,0,
                     0,1,1,1,0,0,1,1,1,0,1,0,
                     0,0,0,0,0,0,0,0,0,0,0,0};

int main (void) {

  FILE *lena=fopen("t.bmp","rb");
  FILE *output=fopen("DEMO2_2.bmp","wb");



  bmp ini1={0};
  bmp *lena_header=&ini1;
  uint8_t *lena_data;
  Header_Read(lena_header,lena);
  uint32_t Total_Pixel_lena=(lena_header->Width_Bitmap)*(lena_header->Height_Bitmap)*(lena_header->Bit_per_Pixel/8);
  uint16_t h=lena_header->Height_Bitmap,w=lena_header->Width_Bitmap;
  uint8_t byte=lena_header->Bit_per_Pixel/8;
  uint8_t *back_data=calloc((lena_header->Width_Bitmap)*(lena_header->Height_Bitmap),sizeof(uint8_t));
  lena_data = malloc(Total_Pixel_lena);
  lena_data = Pixel_Read(lena_header,lena);
  lena_data = Grayscale(lena_header,lena_data);
//Dilation
   lena_data=Sobel(lena_header,lena_data,100);
   // lena_data=Dilation(lena_header,lena_data,2);
   // lena_data=Erosion(lena_header,lena_data,1);
   // back_data=Connected_component_labeling(lena_header,lena_data);
   // uint16_t x=getMaximum(lena_header,back_data);
   // printf("%d\n",x);
   // int count=0;
   // for (int i=0;i<h;i++) {
   //   for (int j=0;j<w;j++) {
   //     count=(count+1)%3;
   //     switch(count) {
   //       case 0 :
   //         lena_data[(i*w+j)*3+0]=back_data[i*w+j]/x*255;
   //         lena_data[(i*w+j)*3+1]=back_data[i*w+j]/x*60;
   //         lena_data[(i*w+j)*3+2]=back_data[i*w+j]/x*100;
   //         break;
   //       case 1 :
   //         lena_data[(i*w+j)*3+0]=back_data[i*w+j]/x*100;
   //         lena_data[(i*w+j)*3+1]=back_data[i*w+j]/x*123;
   //         lena_data[(i*w+j)*3+2]=back_data[i*w+j]/x*255;
   //         break;
   //      case 2 :
   //         lena_data[(i*w+j)*3+0]=back_data[i*w+j]/x*123;
   //         lena_data[(i*w+j)*3+1]=back_data[i*w+j]/x*255;
   //         lena_data[(i*w+j)*3+2]=back_data[i*w+j]/x*100;
   //         break;
   //      default : break;
   //     }
   //   }
   // }
 lena_data=Unnormalized(lena_header,lena_data);
 Pixel_Write(lena_header,lena_data,output);
}




uint8_t *Connected_component_labeling(bmp *Header_Data,uint8_t *Image_Data) {
  uint16_t w=Header_Data->Width_Bitmap,h=Header_Data->Height_Bitmap;
  uint8_t  byte=Header_Data->Bit_per_Pixel/8;
 // h=4;w=12;byte=1;
  uint8_t *Label_Data=(uint8_t *)calloc(w*h,sizeof(uint8_t));
  uint8_t  temp1[8]={0},*temp2;
  uint32_t  Label_value=0;
  // for(int i=0;i<h;i++) {
  //   for (int j=0;j<w;j++) {
  //     printf("%d\t",Image_Data[i*w+j] );
  //   }printf("\n");
  // }
printf("==============\n");
for (int count=0;count<10;count++) {
  for (uint16_t row=1;row<h-1;row++) {
    for (uint16_t column=1;column<w-1;column++) {
      if(Image_Data[(row*w+column)*byte] != 1) {
        temp1[0]=Label_Data[(row+1)*w+column-1];
        temp1[1]=Label_Data[(row+1)*w+column+0];
        temp1[2]=Label_Data[(row+1)*w+column+1];
        temp1[3]=Label_Data[(row  )*w+column-1];
        temp1[4]=Label_Data[(row  )*w+column+1];
        temp1[5]=Label_Data[(row-1)*w+column-1];
        temp1[6]=Label_Data[(row-1)*w+column+0];
        temp1[7]=Label_Data[(row-1)*w+column+1];
        temp2=Sort(temp1);
        uint8_t value=0;
        for(int i=0;i<8;i++) if(temp2[i]!=0) {value=temp2[i];break;}
        if(value!=0) {Label_Data[row*w+column]=value;}
        else {Label_value++;Label_Data[row*w+column]=Label_value;}
        free(temp2);
      }
    }
  }
}
  // for(int i=0;i<h;i++) {
  //   for (int j=0;j<w;j++) {
  //     printf("%d\t",Label_Data[i*w+j] );
  //   }printf("\n");
  // }
/*-----------------------------------------------------*/
// uint8_t haveValue[256] = {0};
  //   uint8_t cnt = 0;
  //   for(uint32_t i=0;i<w*h;i++){
  //     if(Label_Data[i] != 0){
  //       int flag = 0;
  //       for(uint8_t j=0;j<cnt+1;j++){
  //         if((uint8_t)Label_Data[i] == haveValue[j]){
  //           flag = 1;
  //         }
  //       }
  //       if(!flag){
  //         haveValue[cnt] = (uint8_t)Label_Data[i];
  //         cnt++;
  //       }
  //       uint8_t cnt2 = 0;
  //       while((uint8_t)Label_Data[i] != haveValue[cnt2]){
  //         cnt2++;
  //       }
  //       Label_Data[i] = cnt2+1;
  //     }
  // }
  return Label_Data;
}
