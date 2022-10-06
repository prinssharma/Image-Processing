#include<stdio.h>
#include<stdlib.h>

void main()
{
	FILE *fp;		
	int i, j, temp = 0;
	int width = 0, height = 0;
	char buffer[4];

	int **image;
	
	fp = fopen("Negative.txt", "r");
	
	fscanf(fp, "%s", buffer);
	
	width = atoi(buffer);
	printf("%d\n", width);
	
	fscanf(fp, "%s", buffer);
	
	height = atoi(buffer);
	printf("%d\n", height);
	
	image = (int **)malloc(height*sizeof(int *));
	
	for(i = 0;i<height;i++)
	{
		image[i] = (int *)malloc(width*sizeof(int));
		for(j = 0; j<width;j++)
		{
			fscanf(fp, "%s", buffer);
	
			image[i][j] = atoi(buffer);
			
			//APPLYING IMAGE NEGATIVES IN THE IMAGE.
			image[i][j] = 255 - image[i][j];
		
			printf("%d\t", image[i][j]); 
		}
		printf("\n");
	}
	
	fclose(fp);

	FILE *img;
	img = fopen("Negative_image_Convertor.pgm", "wb");

	// Writing Magic Number to the File
	fprintf(img, "P2\n"); 

	// Writing Width and Height
	fprintf(img, "%d %d\n", width, height); 

	// Writing the maximum gray value
	fprintf(img, "255\n"); 
	int count = 0;
	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
	    		temp = image[i][j];

         // Writing the gray values in the 2D array to the file
		        fprintf(img, "%d ", temp);
		}
		fprintf(img, "\n");
	}
	fclose(img);
}
