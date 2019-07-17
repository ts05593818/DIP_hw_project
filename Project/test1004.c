#include "openfile.h"


float v_hpf[512*512]={0};
float h_data[512*512]={0};
float s_data[512*512]={0};
float v_data[512*512]={0};
float v_new[512*512]={0};
float padding[514*514]={0};
float max=0,min=0;
float k=0.7;
float total=0;
uint8_t Offset[3]={2,1,0};
uint8_t Color[3]={0};
float Mask[3][3]={{0.0,-1.0,0.0},{-1.0,4.0,-1.0},{0.0,-1.0,0.0}};
float Mask1[3][3]={{1.0,2.0,1.0},{2.0,4.0,2.0},{1.0,2.0,1.0}};

int main (void) {
  FILE *lena=fopen("lena.bmp","rb");
  FILE *output=fopen("output1.bmp","wb");
  bmp ini1={0};
  bmp *lena_header=&ini1;
  float *temp=malloc(sizeof(float)*3);
  uint8_t *temp2=malloc(sizeof(uint8_t)*3);
  uint8_t *lena_data;
  Header_Read(lena_header,lena);
  uint32_t Total_Pixel_lena=(lena_header->Width_Bitmap)*(lena_header->Height_Bitmap)*(lena_header->Bit_per_Pixel/8);
  uint8_t byte=lena_header->Bit_per_Pixel/8;
  lena_data = malloc(Total_Pixel_lena);
  lena_data = Pixel_Read(lena_header,lena);

//RGBtoHSV
  for (int row=0;row<512;row++) {
    for (int column=0;column<512;column++) {
      Color[0]=lena_data[(row*512+column)*byte+Offset[0]];//R
      Color[1]=lena_data[(row*512+column)*byte+Offset[1]];//G
      Color[2]=lena_data[(row*512+column)*byte+Offset[2]];//B
      temp=RGB2HSV(Color);
      h_data[(row*512+column)]=temp[0];//H
      s_data[(row*512+column)]=temp[1];//S
      v_data[(row*512+column)]=temp[2];//V
    }
  }
//zero padding
  for (int row=0;row<512;row++) {
    for (int column=0;column<512;column++) {
      padding[(row+1)*514+column+1]=v_data[row*512+column];
    }
  }
//LPF and substract lpf from origin then find Max/min and mapping
  for (int row=1;row<513;row++) {
    for (int column=1;column<513;column++) {
      total=0;
      for (int v=0;v<3;v++) {
        for (int u=0;u<3;u++) {
          total+=padding[(row+v-1)*514+(column+u-1)]*Mask1[v][u];
        }
      }
     v_new[(row-1)*512+column-1]=v_data[(row-1)*512+column-1]-total/16;
     //v_new[(row-1)*512+column-1]=v_data[(row-1)*512+column-1]+k*v_hpf[(row-1)*512+column-1];
      if (max < v_new[(row-1)*512+column-1]) {max=v_new[(row-1)*512+column-1];}
      else {max=max;}
      if (min > v_new[(row-1)*512+column-1]) {min=v_new[(row-1)*512+column-1];}
      else {min=min;}
    }
  }
//nomorlization
printf("Max:%f min:%f\n",max,min);
  for (int row=0;row<512;row++) {
    for (int column=0;column<512;column++) {
      v_new[row*512+column]=(v_new[row*512+column]-min)/(max-min);
    }
  }
//HSV2RGB
  for (int row=0;row<512;row++) {
    for (int column=0;column<512;column++) {
      temp[0]=h_data[row*512+column];
      temp[1]=s_data[row*512+column];
      temp[2]=v_new[row*512+column];
      temp2=HSV2RGB(temp);
      lena_data[(row*512+column)*byte+Offset[0]]=temp2[0];
      lena_data[(row*512+column)*byte+Offset[1]]=temp2[1];
      lena_data[(row*512+column)*byte+Offset[2]]=temp2[2];
      // printf("%d %d %d \n",temp2[0],temp2[1],temp2[2]);
      //printf("%d %d %d \n",lena_data[(row*512+column)*byte+Offset[2]],lena_data[(row*512+column)*byte+Offset[1]],lena_data[(row*512+column)*byte+Offset[0]]);
    }
  }
  Pixel_Write(lena_header,lena_data,output);
}
