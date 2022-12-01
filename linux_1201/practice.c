#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>  
#include "bmpHeader.h"

#define widthBytes(bits) (((bits)+31)/32*4)
#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

typedef unsigned char ubyte;

int main(int argc, char** argv)
{
    FILE* fp;
    BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
    BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
    RGBQUAD palrgb[256];
    ubyte *inimg, *outimg;
    int x, y, z, elemSize, imageSize;
    int xFactor = 2, yFactor = 2;

    if(argc != 3) {
        fprintf(stderr, "usage : %s input.bmp output.bmp\n", argv[0]);
        return -1;
    }
   
    /***** read bmp *****/
    if((fp=fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "Error : Failed to open file...₩n");
        return -1;
    }

    fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    if(bmpInfoHeader.biBitCount != 24) {
        perror("This image file doesn't supports 24bit color\n");
        fclose(fp);
        return -1;
    }
   
    imageSize = widthBytes(bmpInfoHeader.biBitCount * bmpInfoHeader.biWidth) *
                           bmpInfoHeader.biHeight;

    printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);     /* 픽셀당 비트 수(색상) */
    printf("Image Size : %d\n", imageSize);
   
    inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
    outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize*xFactor*yFactor);
    fread(inimg, sizeof(ubyte), imageSize, fp);
   
    fclose(fp);
   
    elemSize = bmpInfoHeader.biBitCount / 8;
    for(y = 0; y < bmpInfoHeader.biHeight*elemSize; y+=elemSize) {
        for(x = 0; x < bmpInfoHeader.biWidth*elemSize; x+=elemSize) {
            for(z = 0; z < elemSize; z++) {
                int e1 = inimg[x+(y*bmpInfoHeader.biWidth)+z];
                int e2 = inimg[x+(y*bmpInfoHeader.biWidth)+z+elemSize];
                int e3 = inimg[x+((y+elemSize)*bmpInfoHeader.biWidth)+z];
                int e4 = inimg[x+((y+elemSize)*bmpInfoHeader.biWidth)+z+elemSize];
                //outimg[(x)+(bmpInfoHeader.biWidth*y)+z]=e;
                outimg[(x+(bmpInfoHeader.biWidth*y*yFactor))*xFactor+z]=e1;
                outimg[(x+(bmpInfoHeader.biWidth*y*yFactor))*xFactor+z+elemSize]=(e1+e2)>>1;
                outimg[(x+(bmpInfoHeader.biWidth*(y*yFactor+elemSize)))*xFactor+z]=(e1+e3)>>1;
                outimg[(x+(bmpInfoHeader.biWidth*(y*yFactor+elemSize)))*xFactor+z+elemSize]=(e1+e2+e3+e4)>>2;
            }  
        }  
     }  
     
    /***** write bmp *****/
    if((fp=fopen(argv[2], "wb"))==NULL) {
        fprintf(stderr, "Error : Failed to open file...₩n");
        return -1;
    }

    bmpInfoHeader.biWidth*=xFactor;
    bmpInfoHeader.biHeight*=yFactor;
    bmpInfoHeader.SizeImage = imageSize * xFactor * yFactor;
   
    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    fwrite(outimg, sizeof(unsigned char), bmpInfoHeader.SizeImage, fp);
   
    free(inimg);
    free(outimg);
   
    fclose(fp);
   
    return 0;
}
