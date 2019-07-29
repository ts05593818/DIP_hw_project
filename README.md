# DIP_hw_project
###### tags: `DIP`


## HW1
    
### Open/Read/Write BMP file
    開檔、讀區標頭、寫檔
    void Header_Read(bmp *Header_Data,FILE *image);
    uint8_t *Pixel_Read(bmp *Header_Data,FILE *image);
    void Pixel_Write(bmp *Header_Data,uint8_t *Image_Data,FILE *image);
### Histogram Equlization
    用灰階函式取得直方圖數值
    
    
### Mirrored
    鏡射練習
    uint8_t *Reverse(bmp *Header_Data,uint8_t *Data);
### GrayScale
    RGB轉灰階
    uint8_t *Grayscale(bmp *Header_Data,uint8_t *Data);
## HW2

### DFT
    離散傅立葉轉換，時域轉頻域分析
    採用二維方式計算，減少空間複雜度
    uint8_t *DFT(bmp *Header_Data,uint8_t *Data);
### DCT
    離散餘弦轉換，有多種型態之轉換式，此次只挑選一種
![DCT](https://i.imgur.com/VzTOEMY.png)
    //程式碼不見ㄌQQ




## HW3

### Connected component labeling    
    Segmentation常見之演算法，利用增長的方式進行區塊化分
    使用的model常見有4/8之分
<center class="half">
    <img src="https://i.imgur.com/6ECRNHf.png" width="200"/>
</center>
    
## FinalProject   

### Image Enhancement
    首先將BMP圖檔之RGB RAWDATA轉換到HSV域
    再利用Global/Local對影像處理 強化對比度
    最後反轉回RGB得到增強後之影像
<center class="half">
    <img src="https://i.imgur.com/K2L8hp6.png" width="200"/><img src="https://i.imgur.com/FVMZFTi.png" width="200"/>
</center>    


