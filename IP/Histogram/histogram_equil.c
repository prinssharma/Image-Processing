#include<stdio.h>
#include<stdlib.h>
#include <math.h>

void main()
{
	FILE *fp;		//file is used to read intensity_value.txt
	int i, j, temp = 0;
	int width = 0, height = 0;
	char buffer[4];

	int **image;
	
	long int frequency[256], cu_freq[256], round_value[256], total;
	
	double cu_prob[256], s_k[256];
	
	fp = fopen("Histog.txt", "r");
	
	fscanf(fp, "%s", buffer);
	
	width = atoi(buffer);
	printf("%d\n", width);
	
	fscanf(fp, "%s", buffer);
	
	height = atoi(buffer);
	printf("%d\n", height);
	
	image = (int **)malloc(height*sizeof(int *));
	
	//Initializing the values
	for (i = 0; i<256; i++)
	{
		frequency[i] = 0;
		cu_freq[i] = 0;
		round_value[i] = 0;
		cu_prob[i] = 0.0;
		s_k[i] = 0.0;
		total = 0;
	}
	
	for(i = 0;i<height;i++)
	{
		image[i] = (int *)malloc(width*sizeof(int));
		for(j = 0; j<width;j++)
		{
			fscanf(fp, "%s", buffer);
	
			image[i][j] = atoi(buffer);
			
			//Finding the frequency of each grey label
			frequency[image[i][j]] += 1;
		}
	}
	
	//Printing the frequency labels and calculating the cu frequency, cu probability
	
	printf("Grey label\tFrequency\tCu Freq\t\tCu probability\t\ts_k\t\t\tround(s_k)\n");
	for (i = 0; i<256; i++)
	{
		total = total + frequency[i];
		cu_freq[i] = total;
		cu_prob[i] = (double)total / (double)(width * height);
		s_k[i] = (double)255 * cu_prob[i];
		round_value[i] = (long int)round(s_k[i]);
		printf("%d\t\t%ld\t\t%ld\t\t%f\t\t%f\t\t%ld\n", i, frequency[i], cu_freq[i], cu_prob[i], s_k[i], round_value[i]);
	}
	
	fclose(fp);
	
	//Applying histrogram equilization
	for(i = 0;i<height;i++)
	{
		for(j = 0; j<width;j++)
		{
	
			image[i][j] = (int)round_value[image[i][j]];
		}
	}

	FILE *img;
	img = fopen("histogram_equalization_image.bmp", "wb");

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
