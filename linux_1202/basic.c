#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>   
#include <unistd.h>

#include "bmpHeader.h"

#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

typedef unsigned char ubyte;

int main(int argc, char** argv)
{
    FILE* fp;
    BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
    BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
    RGBQUAD *palrgb;
    ubyte *inimg, *outimg;
    int x, y, z, imageSize;
	char input[128];

	if(argc != 4) {
    	fprintf(stderr, "usage : %s -[h/v] input.bmp output.bmp\n", argv[0]);
        return -1;
    }
  
    /***** read bmp *****/
    if((fp=fopen(argv[2], "rb")) == NULL) {
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
   
    int elemSize = bmpInfoHeader.biBitCount/8;
    int size = bmpInfoHeader.biWidth*elemSize;
    imageSize = size * bmpInfoHeader.biHeight;

    printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);   
    printf("Image Size : %d\n", imageSize);

    inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
    outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
    fread(inimg, sizeof(ubyte), imageSize, fp);
   
    fclose(fp);
   
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {
		int index = y * size;
        for(x = 0; x < size; x++) {
			outimg[index+3*x+0] = ~inimg[size*y+3*x+0];
			outimg[index+3*x+1] = ~inimg[size*y+3*x+1];
			outimg[index+3*x+2] = ~inimg[size*y+3*x+2];
        }
    }        
     
    /***** write bmp *****/
    if((fp=fopen(argv[3], "wb"))==NULL) {
        fprintf(stderr, "Error : Failed to open file...₩n");
        return -1;
    }

    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    //fwrite(inimg, sizeof(ubyte), imageSize, fp);
    fwrite(outimg, sizeof(ubyte), imageSize, fp);

    fclose(fp);
   
    free(inimg);
    free(outimg);
   
    return 0;
}
