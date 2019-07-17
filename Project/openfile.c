#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include "openfile.h"
uint8_t  s8=sizeof(uint8_t);
uint16_t s16=sizeof(uint16_t);
uint32_t s32=sizeof(uint32_t);
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

// uint8_t *Reverse(bmp *Header_Data,uint8_t *Data) {
//   uint32_t w,h;
//   uint16_t p;
//   w=Header_Data->Width_Bitmap;
//   h=Header_Data->Height_Bitmap;
//   p=Header_Data->Bit_per_Pixel;
//   // memory use
//   uint8_t *Reverse_Data=malloc((w)*(h)*(p/8));
//   for (uint32_t row=0;row<h;row++) {
//     for (uint32_t column=0;column<w;column++) {
//     //R
//     Reverse_Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[0])]
//     =Data[(row*w+w)*(p/8)-column*(p/8)-(1+RGB_off[0])];
//     //G
//     Reverse_Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[1])]
//     =Data[(row*(w)+w)*(p/8)-column*(p/8)-(1+RGB_off[1])];
//     // B
//     Reverse_Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[2])]
//     =Data[(row*w+w)*(p/8)-column*(p/8)-(1+RGB_off[2])];
//   }
// }
// return Reverse_Data;
// }
//
// uint8_t *Grayscale(bmp *Header_Data,uint8_t *Data) {
//   uint32_t w,h;
//   uint16_t p;
//   uint8_t graygrid;
//   FILE * histogram_data = fopen("histogram.txt","w");
//   uint16_t histogram[256]={0};
//   w=Header_Data->Width_Bitmap;
//   h=Header_Data->Height_Bitmap;
//   p=Header_Data->Bit_per_Pixel;
//   for (uint32_t row=0;row<h;row++) {
//     for(uint32_t column=0;column<w;column++) {
//       graygrid=(299*Data[row*w*(p/8)+column*(p/8)+RGB_off[0]]+
//                 587*Data[row*w*(p/8)+column*(p/8)+RGB_off[1]]+
//                 114*Data[row*w*(p/8)+column*(p/8)+RGB_off[2]])/1000;
//       // graygrid=(Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[0])]+
//       //           Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[1])]+
//       //           Data[row*w*(p/8)+column*(p/8)+(2-RGB_off[2])])/3;
//       histogram[graygrid]++;
//       Data[row*w*(p/8)+column*(p/8)+RGB_off[0]]=graygrid;
//       Data[row*w*(p/8)+column*(p/8)+RGB_off[1]]=graygrid;
//       Data[row*w*(p/8)+column*(p/8)+RGB_off[2]]=graygrid;
//     }
//   }
//   for (int c=0;c<256;c++) {
//     fprintf(histogram_data,"%d\n",histogram[c]);
//   }
//   fclose(histogram_data);
// return Data;
// }
//
// uint8_t *Cover(bmp *lena_Header_Data,bmp *water_Header_Data,uint8_t *lena,uint8_t *water) {
//   uint32_t w_l,h_l,w_w,h_w;
//   uint16_t b_l,b_w;
//   w_l=lena_Header_Data->Width_Bitmap;
//   h_l=lena_Header_Data->Height_Bitmap;
//   w_w=water_Header_Data->Width_Bitmap;
//   h_w=water_Header_Data->Height_Bitmap;
//   b_l=(lena_Header_Data->Bit_per_Pixel)/8;
//   b_w=(lena_Header_Data->Bit_per_Pixel)/8;
//   for (uint32_t row=10;row<164;row++)  {
//     for (uint32_t column=112;column<190;column++) {
//       lena[(row*w_l+column)*b_l+(2-RGB_off[0])]=water[((row+4)*w_w+column-60)*b_w+(2-RGB_off[0])];
//       lena[(row*w_l+column)*b_l+(2-RGB_off[1])]=water[((row+4)*w_w+column-60)*b_w+(2-RGB_off[1])];
//       lena[(row*w_l+column)*b_l+(2-RGB_off[2])]=water[((row+4)*w_w+column-60)*b_w+(2-RGB_off[2])];
//     }
//   }
//   return lena;
// }

uint8_t Max_min(uint8_t mode,uint8_t a,uint8_t b) {
    if(mode==1) {//Max
      return a>=b?a:b;
    }
    else {//min
      return a<=b?a:b;
    }
}
float *RGB2HSV(uint8_t *color) {
  float delta=0,h=0,s=0,v=0;
  float *hsv=malloc(sizeof(float)*3);
  uint8_t M=0,m=0;
  M=Max_min(1,Max_min(1,color[0],color[1]),color[2]);
  m=Max_min(0,Max_min(0,color[0],color[1]),color[2]);
  delta=(float)(M-m);
  v=(float)M;
    if(v==0) {
      s=0;
    }
    else {
      s=delta/v;
    }
    if(s==0) {
      h=0;
    }
    else {
      if      (M==color[0])    h=(float)((color[1]-color[2])/delta);
      else if (M==color[1])    h=(float)(2+(color[2]-color[0])/delta);
      else if (M==color[2])    h=(float)(4+(color[0]-color[1])/delta);
      h*=60;
      if(h<0) h=h+360;
    }
    hsv[0]=h;hsv[1]=s;hsv[2]=v/255;
    h=0;s=0,v=0;
    return hsv;
}

uint8_t *HSV2RGB(float *input) {
  float h_data=input[0];
  float s_data=input[1];
  float v_data=input[2];
  //printf("h=%.2f s=%.2f v=%.2f\n",h_data,s_data,v_data);
  float r=0,g=0,b=0,f=0,p=0,q=0,t=0;
  int i=0;
  uint8_t *temp=malloc(sizeof(uint8_t)*3);
  if(s_data==0) {
    r=v_data;
    g=v_data;
    b=v_data;
  }
  else {
    if(h_data==360) {h_data=0.0;}
    else {h_data=h_data/60.0;}
    i=(int)trunc(h_data);
    // printf("%d\n",i);
    f=(float)(h_data-i);
    // printf("%.2f\n",f);
    p=v_data*(1.0-s_data);
    // printf("%.2f\n",p);
    q=v_data*(1.0-(s_data*f));
    // printf("%.2f\n",q);
    t=v_data*(1.0-(s_data*(1.0-f)));
    // printf("%.2f\n",t);
    switch (i) {
      case 0 :
        r=v_data;
        g=t;
        b=p;break;
      case 1 :
        r=q;
        g=v_data;
        b=p;break;
      case 2 :
        r=p;
        g=v_data;
        b=t;break;
      case 3 :
        r=p;
        g=q;
        b=v_data;break;
      case 4 :
        r=t;
        g=p;
        b=v_data;break;
      default :
        r=v_data;
        g=p;
        b=q;
        break;
    }
    // printf("r=%.2f g=%.2f b=%.2f\n",r,g,b);
  }
  temp[0]=(uint8_t)(r*255)&255;
  temp[1]=(uint8_t)(g*255)&255;
  temp[2]=(uint8_t)(b*255)&255;
  // printf("--%d %d %d \n",temp[0],temp[1],temp[2]);
  return temp;
}
void PSNR (uint8_t *Data,uint8_t *New_Data,bmp *Header_Data) {
  float MSE=0;
  float psnr=0;
  uint32_t Image_Size=Header_Data->Width_Bitmap*Header_Data->Height_Bitmap*Header_Data->Bit_per_Pixel/8;

  for (int row=0;row<Header_Data->Height_Bitmap;row++) {
    for (int column=0;column<Header_Data->Width_Bitmap;column++) {
      MSE+=pow((float)(Data[row*Header_Data->Width_Bitmap+column]-New_Data[row*Header_Data->Width_Bitmap+column]),2.0)/Image_Size;
    }
  }
  psnr=10*log(255*255/MSE);
  printf("%f\n",psnr);
}
