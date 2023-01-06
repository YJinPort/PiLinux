#include "bmpHeader.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	/*Declaration of Variables*/
	FILE *filePointer;
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned char *img; 

	filePointer = fopen(argv[1], "rb");	//File Open

	/*Read File*/
	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePointer);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePointer);

	/*Run Program Exception Handling*/
	if(argc > 2) {
		perror("Only run file name & image\n");
		return -1;	
	}

	/*Image Type Exception Handling*/
	if(bmpInfoHeader.biBitCount != 24) {
		perror("You can use only 24bit image\n");
		return -1;	
	}
	
	/*Image Size Exception Handling*/
	if (bmpInfoHeader.SizeImage == 0) {
		bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * bmpInfoHeader.biBitCount / 8;
	}

	//Create Image Memory
	img = (unsigned char*)malloc(sizeof(unsigned char) * bmpInfoHeader.SizeImage);

	//Read Image
	fread(img, sizeof(unsigned char), bmpInfoHeader.SizeImage, filePointer);
	
	/*Print Image Pixel Value*/
	for(int i=0; i < bmpInfoHeader.SizeImage; i+=bmpInfoHeader.biBitCount/8) {
		printf("(%d, %d, %d)\n", img[i], img[i+1], img[i+2]);
	}

	fclose(filePointer);	//File Close

	return 0;
}
