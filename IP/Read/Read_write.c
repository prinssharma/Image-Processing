#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	clock_t start, stop;        /*For time*/

	start=clock(); 

	FILE *fo = fopen("Write.bmp","wb"); /* Output File name*/

	int i;

	FILE *streamIn; 
        streamIn = fopen("Read.bmp", "r"); /*Input file name*/
   
        if (streamIn == (FILE *)0) // check if the input file has not been opened succesfully.
	{
            printf("There is an error in opening the file\n");
            exit(0);
 	}

 	unsigned char header[54];         /*image header*/
	unsigned char colorTable[1024]; // to store the colorTable, if exists.
	
 	int count = 0;
 	for(i=0;i<54;i++) 
 	{
 		header[i] = getc(streamIn);  // strip out BMP header
 		
 	}
 	int width = *(int*)&header[18]; // read the width from the image header
 	int height = *(int*)&header[22]; // read the height from the image header
	int bitDepth = *(int*)&header[28]; // read the bitDepth from the image header

	if(bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), 1024, streamIn);

	fwrite(header, sizeof(unsigned char), 54, fo); // write the image header to output file
  	
 	unsigned char buf[height * width]; // to store the image data

	fread(buf, sizeof(unsigned char), (height * width), streamIn);
	
	if(bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), 1024, fo);	

	fwrite(buf, sizeof(unsigned char), (height * width), fo);
 	
	fclose(fo);
 	fclose(streamIn);

	stop = clock(); 
	
	printf("Time: %lf ms\n",((double)(stop - start) * 1000.0 )/ CLOCKS_PER_SEC);

}
