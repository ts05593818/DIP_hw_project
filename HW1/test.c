#include "openfile.h"


int main (void) {
  int c=1;
  
  FILE *lena=fopen("test.bmp","rb");
  FILE *output=fopen("oral4.bmp","wb");
  FILE * histogram_data = fopen("histogram_oral.txt","w");
  uint32_t w,h;
  uint16_t byte;
  uint16_t histogram[256]={0};
  //Initial
  bmp ini1={0};
  bmp *lena_header=&ini1;
  //
  uint8_t *lena_data;
  Header_Read(lena_header,lena);
  //Calculate data pixel
  uint32_t Total_Pixel_lena=(lena_header->Width_Bitmap)*(lena_header->Height_Bitmap)*(lena_header->Bit_per_Pixel/8);
  //memory allocate
  lena_data = malloc(Total_Pixel_lena);
  //read pixel data
  lena_data = Pixel_Read(lena_header,lena);
  w=lena_header->Width_Bitmap;
  h=lena_header->Height_Bitmap;
  byte=lena_header->Bit_per_Pixel/8;
//origin histogram
  // for (int row=0;row<h;row++) {
  //   for (int column=0;column<w;column++) {
  //     histogram[lena_data[(row*w+column)*byte]]++;
  //   }
  // }
  //c=1 gamma=0.5
    for (int row=0;row<h;row++) {
      for (int column=0;column<w;column++) {
        lena_data[(row*w+column)*byte]=(uint8_t)255*c*pow((lena_data[(row*w+column)*byte]/255.0),0.5);
        // histogram[lena_data[(row*w+column)*byte]]++;
      }
    }
  //c=1 gamma=2
    for (int row=0;row<h;row++) {
      for (int column=0;column<w;column++) {
        lena_data[(row*w+column)*byte]=(uint8_t)255*c*pow((lena_data[(row*w+column)*byte]/255.0),2);
        histogram[lena_data[(row*w+column)*byte]]++;
      }
    }
//c=1 gamma=4
  for (int row=0;row<h;row++) {
    for (int column=0;column<w;column++) {
      lena_data[(row*w+column)*byte]=(uint8_t)255*c*pow((lena_data[(row*w+column)*byte]/255.0),4);
      // histogram[lena_data[(row*w+column)*byte]]++;
    }
  }
// // //c=1 gamma=0.25
  for (int row=0;row<h;row++) {
    for (int column=0;column<w;column++) {
      lena_data[(row*w+column)*byte]=(uint8_t)255*c*pow((lena_data[(row*w+column)*byte]/255.0),0.25);
      // histogram[lena_data[(row*w+column)*byte]]++;
    }
  }

  for (int c=0;c<256;c++) {
    fprintf(histogram_data,"%d\n",histogram[c]);
  }
  fclose(histogram_data);


  Pixel_Write(lena_header,lena_data,output);

}
