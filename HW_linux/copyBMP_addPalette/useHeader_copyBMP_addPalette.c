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
	RGBQUAD *palette;

	filePointer = fopen(argv[1], "rb");	//File Open

	/*Read File*/
	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePointer);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePointer);
	
	/*Image Type Exception Handling*/
	if(bmpInfoHeader.biBitCount != 24) {
		perror("You can use only 24 bits BITMAP images");
		return -1;
	}

	/*Image Size Exception Handling*/
	if(!bmpInfoHeader.SizeImage) {
		bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * bmpInfoHeader.biBitCount / 8;
	}
	
	/*Create Image * Palette Memory*/
	inimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);
	outimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);
	palette = (RGBQUAD*)malloc(sizeof(RGBQUAD) * 256);

	//Read Image
	fread(inimg, sizeof(ubyte), bmpInfoHeader.SizeImage, filePointer);

	fclose(filePointer);	//Close File

	/*Set Palette Color*/
	for(int i = 0; i < 256; i++){
		palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = i;
	}

	/*Set Out Image*/
	for(int j = 0; j < bmpInfoHeader.biHeight; j++){
		for (int i = 0; i < bmpInfoHeader.biWidth; i++){
			for(int z = 0; z < 3; z++){
				outimg[(i*3)+(j*bmpInfoHeader.biWidth*3)+z] = inimg[(i*3) + (j*bmpInfoHeader.biWidth*3) + z];
			}
		}
	}

	/*Set BMP_Header*/
	bmpInfoHeader.biClrUsed = 256;
	bmpFileHeader.bfSize += sizeof(RGBQUAD) * bmpInfoHeader.biClrUsed;
	bmpFileHeader.bfOffBits += sizeof(RGBQUAD) * bmpInfoHeader.biClrUsed;

	filePointer = fopen(argv[2], "w");	//File Open

	/*Create Out Image*/ 
	fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePointer);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePointer);
	fwrite(palette, sizeof(RGBQUAD), bmpInfoHeader.biClrUsed, filePointer);
	fwrite(outimg, sizeof(ubyte), bmpInfoHeader.SizeImage, filePointer);

	fclose(filePointer);	//File Close

	/*Management Memory*/
	free(inimg);
	free(outimg);
	free(palette);

	return 0;
}
