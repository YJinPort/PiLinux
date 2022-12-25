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
	
	filePointer = fopen(argv[1], "rb");

	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePointer);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePointer);
	
	if(!bmpInfoHeader.SizeImage) {
		bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * bmpInfoHeader.biBitCount / 8;
	}

	inimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);
	outimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);

	fread(inimg, sizeof(ubyte), bmpInfoHeader.SizeImage, filePointer);
	fclose(filePointer);

	for(int i = 0; i < bmpInfoHeader.SizeImage; i++) {
		*(outimg+i) = *(inimg+i);
	}
	
	filePointer = fopen(argv[2], "w");

	fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePointer);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePointer);
	fwrite(outimg, sizeof(ubyte), bmpInfoHeader.SizeImage, filePointer);

	fclose(filePointer);

	return 0;
}
