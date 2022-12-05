#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>                     /* USHRT_MAX 상수를 위해서 사용한다. */
#include <unistd.h>

#include "bmpHeader.h"

/* 이미지 데이터의 경계 검사를 위한 매크로 */
#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

typedef unsigned char ubyte;

ubyte clip(int value, int min, int max) {
	return (value > max ? max : value < min ? min : value);
}

int main(int argc, char** argv)
{
    FILE* fp;
    BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
    BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
    RGBQUAD *palrgb;
    ubyte *inimg, *outimg, *padimg;
    int x, y, z, imageSize, paddingSize;

    if(argc != 3) {
        fprintf(stderr, "usage : %s input.bmp output.bmp\n", argv[0]);
        return -1;
    }
   
    /***** read bmp *****/
    if((fp=fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "Error : Failed to open file...₩n");
        return -1;
    }

    /* BITMAPFILEHEADER 구조체의 데이터 */
    fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    /* 트루 컬러를 지원하면 변환할 수 없다. */
    if(bmpInfoHeader.biBitCount != 24) {
        perror("This image file doesn't supports 24bit color\n");
        fclose(fp);
        return -1;
    }
   
    int elemSize = bmpInfoHeader.biBitCount/8;
    int size = bmpInfoHeader.biWidth*elemSize;
	int padRow = (bmpInfoHeader.biWidth+2)*elemSize;
    imageSize = size * bmpInfoHeader.biHeight;
	paddingSize = ((bmpInfoHeader.biWidth + bmpInfoHeader.biHeight + 2)*2)*elemSize;
	int totSize = imageSize + paddingSize;

    /* 이미지의 해상도(넓이 × 깊이) */
    printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);     /* 픽셀당 비트 수(색상) */
    printf("Image Size : %d\n", imageSize);
	printf("Padding Size : %d\n", paddingSize);
	printf("Total Size : %d\n", totSize);

    inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
    outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
	padimg = (ubyte*)malloc(sizeof(ubyte)*totSize);

    fread(inimg, sizeof(ubyte), imageSize, fp);
	//fread(padimg, sizeof(ubyte), totSize, fp);
   
    fclose(fp);

    // define the kernel
    float kernel[3][3] = { {0, -1, 0},
                           {-1, 5, -1},
                           {0, -1, 0} };

    memset(outimg, 0, sizeof(ubyte)*imageSize);
	memset(padimg, 0, sizeof(ubyte)*totSize);

	for(y = 1; y < bmpInfoHeader.biHeight + 1; y++) {
		for(x = 1; x < (bmpInfoHeader.biWidth + 1)*elemSize; x+=elemSize) {
			padimg[x+y*padRow+0] = inimg[(x-1)+(y-1)*size+0];
			padimg[x+y*padRow+1] = inimg[(x-1)+(y-1)*size+1];
			padimg[x+y*padRow+2] = inimg[(x-1)+(y-1)*size+2];
		}
	}

	for(y = 1; y < bmpInfoHeader.biHeight+1; y++) {
		padimg[0+y*padRow+0] = inimg[0+(y-1)*size+0];
		padimg[0+y*padRow+1] = inimg[0+(y-1)*size+1];
		padimg[0+y*padRow+2] = inimg[0+(y-1)*size+2];

		padimg[((bmpInfoHeader.biWidth+1)*elemSize)+y*padRow+0] = inimg[((bmpInfoHeader.biWidth-1)*elemSize)+(y-1)*size+0];
		padimg[((bmpInfoHeader.biWidth+1)*elemSize)+y*padRow+1] = inimg[((bmpInfoHeader.biWidth-1)*elemSize)+(y-1)*size+1];
		padimg[((bmpInfoHeader.biWidth+1)*elemSize)+y*padRow+2] = inimg[((bmpInfoHeader.biWidth-1)*elemSize)+(y-1)*size+2];
	}

	for(x = elemSize; x < (bmpInfoHeader.biWidth+1)*elemSize; x+=elemSize) {
		padimg[x+0] = inimg[x-elemSize+0];
		padimg[x+1] = inimg[x-elemSize+1];
		padimg[x+2] = inimg[x-elemSize+2];

		padimg[x+(bmpInfoHeader.biHeight+1)*padRow+0] = inimg[x-elemSize+(bmpInfoHeader.biHeight-1)*size+0];
		padimg[x+(bmpInfoHeader.biHeight+1)*padRow+1] = inimg[x-elemSize+(bmpInfoHeader.biHeight-1)*size+1];
		padimg[x+(bmpInfoHeader.biHeight+1)*padRow+2] = inimg[x-elemSize+(bmpInfoHeader.biHeight-1)*size+2];
	}

    for(y = 1; y < bmpInfoHeader.biHeight+1; y++) {
        for(x = 1; x < (bmpInfoHeader.biWidth+1) * elemSize; x+=elemSize) {
            for(z = 0; z < elemSize; z++) {
                float sum = 0.0;
                for(int i = -1; i < 2; i++) {
                    for(int j = -1; j < 2; j++) {
                        sum += kernel[i+1][j+1]*padimg[(x-i*elemSize)+(y-j)*padRow+z];
                    }
                }
                outimg[(x-1)+(y-1)*size+z] = clip(sum, 0, 255);
            }
        }
    }        
     
    /***** write bmp *****/
    if((fp=fopen(argv[2], "wb"))==NULL) {
        fprintf(stderr, "Error : Failed to open file...₩n");
        return -1;
    }

    /* BITMAPFILEHEADER 구조체의 데이터 */
    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    //fwrite(inimg, sizeof(ubyte), imageSize, fp);
    fwrite(outimg, sizeof(ubyte), imageSize, fp);

    fclose(fp);
   
    //free(inimg);
    free(outimg);
	free(padimg);
   
    return 0;
}
