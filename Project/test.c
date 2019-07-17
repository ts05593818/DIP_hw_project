#include "openfile.h"
#include <math.h>


// float v_hpf[512*512]={0};
float h_data[512*512]={0};
float s_data[512*512]={0};
float v_data[512*512]={0};
float v_new[512*512]={0};
float cdf[256]={0.0};
uint32_t histogram[256]={0};
float max=0,min=0;
float k=0.2;
float total=0;
int x=0;
uint8_t Offset[3]={2,1,0};
uint8_t Color[3]={0};
float Mask[3][3]={{0.0,-1.0,0.0},{-1.0,4.0,-1.0},{0.0,-1.0,0.0}};
float Mask1[3][3]={{1.0,2.0,1.0},{2.0,4.0,2.0},{1.0,2.0,1.0}};

int main (void) {
  FILE *origin=fopen("lena.BMP","rb");
  FILE *output=fopen("output_unsharp_k0.9.bmp","wb");
  bmp ini1={0};
  bmp *origin_header=&ini1;
  float *temp=malloc(sizeof(float)*3);
  uint8_t *temp2=malloc(sizeof(uint8_t)*3);
  uint8_t *origin_data,*new_data;
  Header_Read(origin_header,origin);
  uint16_t w=origin_header->Width_Bitmap,h=origin_header->Height_Bitmap;
  uint32_t Total_Pixel_origin=w*h*(origin_header->Bit_per_Pixel/8);
  uint8_t byte=origin_header->Bit_per_Pixel/8;
  origin_data = malloc(Total_Pixel_origin);
  new_data    = malloc(Total_Pixel_origin);
  origin_data = Pixel_Read(origin_header,origin);
//RGBtoHSV
  for (int row=0;row<h;row++) {
    for (int column=0;column<w;column++) {
      Color[0]=origin_data[(row*w+column)*byte+Offset[0]];//R
      Color[1]=origin_data[(row*w+column)*byte+Offset[1]];//G
      Color[2]=origin_data[(row*w+column)*byte+Offset[2]];//B
      temp=RGB2HSV(Color);
      h_data[(row*w+column)]=temp[0];//H
      s_data[(row*w+column)]=temp[1];//S
      v_data[(row*w+column)]=temp[2];//V
    }
  }
//Local enhanced Unsharp
//LPF and substract lpf from origin then find Max/min
  for (int row=0;row<h;++row) {
    for (int column=0;column<w;++column) {
      total=0;
      for (int v=-1;v<2;++v) {
        for (int u=-1;u<2;++u) {
          if ( (row+v >= 0) && (column+u >= 0) && (row+v < h) && (column+u < w) ) {
          total+=v_data[(row+v)*w+(column+u)]*Mask1[v+1][u+1];
          }
        }
      }
     // unsharp
     v_new[row*w+column]=v_data[(row*w+column)]+k*(v_data[(row*w+column)]-total/16);//v_hpf[row*w+column]=v_data[(row*w+column)]-total/16;
      if (max < v_new[row*w+column]) {max=v_new[row*w+column];}
      if (min > v_new[row*w+column]) {min=v_new[row*w+column];}
    }
  }
//nomorlization histogram
printf("Max:%f min:%f\n",max,min);
  for (int row=0;row<h;row++) {
    for (int column=0;column<w;column++) {
      //mapping from 0 to 255
      v_new[row*w+column]=255*(v_new[row*w+column]-min)/(max-min);
      x=round(v_new[row*512+column]);
      histogram[x]++;
    }
  }
  /*
//CDF
  for (int i=0;i<256;i++) {
    if(i==0) {cdf[i]=(float)histogram[i]/(w*h);}
    else     {cdf[i]=cdf[i-1]+(float)histogram[i]/(w*h);}
  }
//Global enhanced
 for (int row=0;row<h;row++) {
   for (int column=0;column<w;column++) {
     // printf("%f\t",v_new[row*512+column]/255);
     x=round(v_new[row*w+column]);
     v_new[row*w+column]=cdf[x];
     // printf("%f\n",v_new[row*512+column]);
   }
 }
 */
//HSV2RGB
  for (int row=0;row<h;row++) {
    for (int column=0;column<w;column++) {
      temp[0]=h_data[row*w+column];
      temp[1]=s_data[row*w+column];
      temp[2]=v_new[row*w+column];
      temp2=HSV2RGB(temp);
      new_data[(row*w+column)*byte+Offset[0]]=temp2[0];
      new_data[(row*w+column)*byte+Offset[1]]=temp2[1];
      new_data[(row*w+column)*byte+Offset[2]]=temp2[2];
    }
  }
  PSNR(origin_data,new_data,origin_header);
  Pixel_Write(origin_header,new_data,output);
}
