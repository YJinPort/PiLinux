#include "bmpHeader.h"
#include <stdio.h>
#include <stdlib.h>

#define ubyte unsigned char

int main(int argc, char **argv)
{
	/*Declaration of Variables*/
	FILE *filePointer;
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	ubyte *inimg, *outimg;
	
	filePointer = fopen(argv[1], "rb");	//File Open

	/*Read File*/
	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePointer);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePointer);
	
	/*BMP_Header Size Exception Handling*/
	if(!bmpInfoHeader.SizeImage) {
		bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * bmpInfoHeader.biBitCount / 8;
	}

	/*Create Image Memory*/
	inimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);
	outimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);

	//Read Image
	fread(inimg, sizeof(ubyte), bmpInfoHeader.SizeImage, filePointer);

	fclose(filePointer);	//File Close

	/*Set Out Image*/
	for(int i = 0; i < bmpInfoHeader.SizeImage; i++) {
		*(outimg+i) = *(inimg+i);
	}
	
	filePointer = fopen(argv[2], "w");	//File Open

	/*Create Out Image*/
	fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePointer);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePointer);
	fwrite(outimg, sizeof(ubyte), bmpInfoHeader.SizeImage, filePointer);

	fclose(filePointer);	//File Close

	return 0;
}
