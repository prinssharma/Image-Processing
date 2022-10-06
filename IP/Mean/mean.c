#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    unsigned char r, g, b;
} RGB;

int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

RGB *readPPM(const char *file, int *width, int *height, int *max)
{
    FILE *fp;
    int numOfPix, i = 0, tempwidth, tempheight, tempmax;
    char garbage[3];
    if ((fp = fopen(file, "r")) == NULL)
        printf("No such file exists!");
    fgets(garbage, sizeof(garbage), fp);
    fscanf(fp, "%d %d %d", &tempwidth, &tempheight, &tempmax);
    int numofPix = tempwidth * tempheight;
    RGB *pixels = malloc(numofPix * sizeof(RGB));
    while (fscanf(fp, "%hhd %hhd %hhd", &pixels[i].r, &pixels[i].g, &pixels[i].b) == 3)
        i++;
    fclose(fp);
    *width = tempwidth;
    *height = tempheight;
    *max = tempmax;
    return pixels;
}

void writePPM(const char *file, int width, int height, int max, const RGB *image)
{
    FILE *fp;
    int i;
    int numofPix = width * height;
    fp = fopen(file, "w");
    fprintf(fp, "P3\n%d %d\n%d\n", width, height, max);
    for (i = 0; i < numofPix; i++)
        fprintf(fp, "%d %d %d ", image[i].r, image[i].g, image[i].b);
    fclose(fp);
}

RGB *filter(int width, int height, const RGB *image)
{
    int widthLow, widthHigh, currentWidth, heightLow, heightHigh, currentHeight, firstPos, lastPos, i, j, numOfPix, ext, append = 0, skipcount;
    int sumR = 0, sumG = 0, sumB = 0, n = 10;
    int changedPos[n * n], medianArrR[n * n], medianArrG[n * n], medianArrB[n * n];
    numOfPix = width * height;
    RGB *boundedpixels = malloc(numOfPix * sizeof(RGB));
    ext = (n - 1) / 2;
    for (i = 0; i < changedPos[n * n]; i++)
        changedPos[i] = 0;
    for (i = 0; i < numOfPix; i++)
    {
        skipcount = 0;
        firstPos = (i - width) - ext;
        lastPos = (i + width) + ext;
        currentWidth = i % width;
        currentHeight = i / width;
        heightLow = currentHeight - ext;
        heightHigh = currentHeight + ext;
        widthLow = currentWidth - ext;
        widthHigh = currentWidth + ext;
        if (widthLow < 0)
            widthLow = 0;
        if (widthHigh > width)
            widthHigh = width;
        if (heightLow < 0)
            heightLow = 0;
        if (heightHigh > height)
            heightHigh = height;
        for (j = firstPos; j <= lastPos; j++)
        {
            if ((j >= 0) && (j <= numOfPix))
            {
                currentWidth = j % width;
                currentHeight = j / width;
                if (((currentWidth >= widthLow) && (currentWidth <= widthHigh)) && ((currentHeight >= heightLow) && (currentHeight <= heightHigh)))
                {
                    changedPos[append] = j;
                    append++;
                }
            }
            skipcount++;
            if (skipcount == n)
            {
                j += (width - n);
                skipcount = 0;
            }
        }
        for (j = 0; j < append; j++)
        {
            sumR += image[changedPos[j]].r;
            sumG += image[changedPos[j]].g;
            sumB += image[changedPos[j]].b;
        }
        sumR = sumR / (append);
        sumG = sumG / (append);
        sumB = sumB / (append);
        for (j = 0; j < append; j++)
        {
            boundedpixels[changedPos[j]].r = sumR;
            boundedpixels[changedPos[j]].g = sumG;
            boundedpixels[changedPos[j]].b = sumB;
        }
        sumR = 0;
        sumG = 0;
        sumB = 0;
        append = 0;
    }
    return boundedpixels;
}

int main()
{
    int width, height, max;
    RGB *RGBValues;
    RGBValues = readPPM("threshold_img.pgm", &width, &height, &max);
    RGBValues = filter(width, height, RGBValues);
    writePPM("mean.pgm", width, height, max, RGBValues);
    printf("Mean filtering applied\n");
    return 0;
}
