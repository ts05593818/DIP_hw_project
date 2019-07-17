#include "openfile.h"


float Re[512][512];
float Im[512][512];
float F_Re[512][512];
float F_Im[512][512];
float sqrt_data[512*512];
float c=0;
int m[2]={1,-1};
int d[512*512]={0};
int count=0;
// uint8_t *DFT(bmp *Header_Data,uint8_t *Data) ;

int main (void) {
  int i=0;
  FILE *picture=fopen("Goldhill.bmp","rb");
  FILE *output=fopen("output.bmp","wb");
  bmp  ini={0};
  bmp *picture_header=&ini;
  uint8_t *picture_data;
  Header_Read(picture_header,picture);
  uint32_t Total_Pixel_picture=(picture_header->Width_Bitmap)*(picture_header->Height_Bitmap)*(picture_header->Bit_per_Pixel/8);
  picture_data = malloc(Total_Pixel_picture);
  picture_data = Pixel_Read(picture_header,picture);

  // picture_data=DFT(picture_header,picture_data);
  uint16_t h=picture_header->Height_Bitmap;
  uint16_t w=picture_header->Width_Bitmap;
  uint16_t b=picture_header->Bit_per_Pixel/8;
  uint32_t t=h*w;

  for(uint32_t r=0;r<h;r++) {
    for (uint32_t c=0;c<w;c++) {
      d[(r*w+c)*b]=picture_data[r*w+c];
      //printf("%d\n",d[(r*w+c)*b]);
    }
  }
  //
  for (uint32_t u=0;u<h;u++) {
    for (uint32_t v=0;v<w;v++) {
      d[(u*w+v)*b]*=m[(u+v)%2];
      // printf("%d\n",d[(u*w+v)*b]);
    }
  }
//printf("%f\n",cos(60*M_PI/180));
//1-D integrator
  for (int x=0;x<h;x++) {
    for (int v=0;v<w;v++) {
      for (int count=0;count<w;count++) {
        Re[x][v]+=d[x*w+count]*cos(2*M_PI*v*count/w);
        Im[x][v]-=d[x*w+count]*sin(2*M_PI*v*count/w);
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
  c=255/log(1+c);
  printf("255/log(1+c)=%f\n",c);
  //
  for (count=0;count<t;count++) {
    picture_data[count]=(uint8_t)(c*log(1+sqrt_data[count]));
    printf("%d\t",picture_data[count]);

  }
printf("------------\n");
  Pixel_Write(picture_header,picture_data,output);
}

/*
uint8_t *DFT(bmp *Header_Data,uint8_t *Data) {
  int m[2]={1,-1};
  int d[512*512]={0};
  float Re[512][512];
  float Im[512][512];
  float F_Re[512][512];
  float F_Im[512][512];
  float sqrt_data[512*512];
  float c=0;
  uint8_t *out;
  uint16_t h=Header_Data->Height_Bitmap;
  uint16_t w=Header_Data->Width_Bitmap;
  uint16_t b=Header_Data->Bit_per_Pixel/8;
  uint16_t t=h*w*b/8;
  out=malloc(t);
printf("1\n");
  for(uint32_t r=0;r<h;r++) {
    for (uint32_t c=0;c<w;c++) {
      d[r*w+c]=Data[r*w+c];
    }
  }
printf("2\n");
  for (uint32_t u=0;u<h;u++) {
    for (uint32_t v=0;v<w;v++) {
      d[(u*w+v)*b]*=m[(u+v)%2];
    }
  }
printf("3\n");
//1-D integrator
  for (uint32_t x=0;x<h;x++) {
    for (uint32_t v=0;v<w;v++) {
      for (uint32_t count=0;count<w;count++) {
        Re[x][v]+=d[x*w+count]*cos(2*M_PI*v*count/w);
        Im[x][v]-=d[x*w+count]*sin(2*M_PI*v*count/w);
      }
    }
  }
printf("4\n");
//2-D integrator
  for (uint32_t v=0;v<w;v++) {
    for (uint32_t u=0;u<h;u++) {
      for (uint32_t count=0;count<h;count++) {
        F_Re[u][v]+=(Re[count][v]*cos(2*M_PI*u*count/h)+Im[count][v]*sin(2*M_PI*u*count/h));
        F_Im[u][v]+=(Im[count][v]*cos(2*M_PI*u*count/h)-Re[count][v]*sin(2*M_PI*u*count/h));
      }
    }
  }
printf("5\n");
//absolute value
  for (uint32_t row=0;row<h;row++) {
    for (uint32_t column=0;column<w;column++) {
      sqrt_data[row*w+column]=sqrt(F_Re[row][column]*F_Re[row][column]+F_Im[row][column]*F_Im[row][column]);
    }
  }
//
printf("6\n");
    for (uint32_t count=0;count<t;count++) {
      if(c > sqrt_data[count]) {
        c=sqrt_data[count];
      }
    }
    c=255/log(1+c);
//
printf("7\n");
    for (uint32_t count=0;count<t;count++) {
      out[count]=(uint8_t)(c*log(1+sqrt_data[count]));
    }
    return out;
}
*/
