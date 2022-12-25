#include "bmpHeader.h"
#include <stdio.h>
#include <stdlib.h>

#define ubyte unsigned char

int main(int argc, char **argv)
{
	FILE *filePointer;
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	ubyte *inimg, *outimg;
	RGBQUAD *palette;

	filePointer = fopen(argv[1], "rb");

	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePointer);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePointer);
	
	if(bmpInfoHeader.biBitCount != 24) {
		perror("You can use only 24 bits BITMAP images");
		return -1;
	}

	if(!bmpInfoHeader.SizeImage) {
		bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * bmpInfoHeader.biBitCount / 8;
	}
	
	inimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);
	outimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);
	palette = (RGBQUAD*)malloc(sizeof(RGBQUAD) * 256);

	fread(inimg, sizeof(ubyte), bmpInfoHeader.SizeImage, filePointer);

	fclose(filePointer);

	for(int i = 0; i < 256; i++){
		palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = i;
	}

	for(int j = 0; j < bmpInfoHeader.biHeight; j++){
		for (int i = 0; i < bmpInfoHeader.biWidth; i++){
			for(int z = 0; z < 3; z++){
				outimg[(i*3)+(j*bmpInfoHeader.biWidth*3)+z] = inimg[(i*3) + (j*bmpInfoHeader.biWidth*3) + z];
			}
		}
	}

	bmpInfoHeader.biClrUsed = 256;
	bmpFileHeader.bfSize += sizeof(RGBQUAD) * bmpInfoHeader.biClrUsed;
	bmpFileHeader.bfOffBits += sizeof(RGBQUAD) * bmpInfoHeader.biClrUsed;

	filePointer = fopen(argv[2], "w");

	fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePointer);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePointer);
	fwrite(palette, sizeof(RGBQUAD), bmpInfoHeader.biClrUsed, filePointer);
	fwrite(outimg, sizeof(ubyte), bmpInfoHeader.SizeImage, filePointer);

	fclose(filePointer);

	free(inimg);
	free(outimg);
	free(palette);

	return 0;
}
