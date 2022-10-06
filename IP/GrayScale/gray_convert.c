#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <math.h>
#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void Image_load(Image *img, const char *fname)
{
	
    img->data =  stbi_load(fname, &img->width, &img->height, &img->channels, 0);
   if(img->data != NULL){
		img->size = img->width*img->height*img->channels;
		img->allocation = STB_ALLOCATED;
		printf("%d\t\t %d\t\t %d\t\t", img->width, img->height, img->channels);
	}
}
void Image_create(Image *img, int w, int h, int ch, bool zeroed)
{
	size_t size = w*h*ch;
	if(zeroed){
		img->data =calloc(size,1);
	}else {
		img->data=malloc(size);
	}
	if(img->data != NULL)
	{
	 img->width = w;
	 img->height = h;
	 img->channels = ch;
	 img->allocation = SELF_ALLOCATED;	
	}
}

void Image_save(const Image *img, const char *fname)
{
		stbi_write_jpg(fname, img->width, img->height, img->channels, img->data, 100);
}

void Image_to_gray(const Image *orig, Image *gray)
{
	int channels = (orig->channels == 4) ? 2: 1;
	Image_create(gray, orig->width, orig->height, channels, false);
	unsigned char *p, *pg; 
   for(p= orig->data, pg = gray->data; p != (orig->data + orig->size); p += orig->channels, pg += gray->channels)
   {
   	 *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
     if(channels == 4)
      {
      *(pg + 1) = *(p+3);
	  }	  
   } 
  
}
void Image_free(Image *img)
{
  if(img->allocation != NO_ALLOCATION && img->data != NULL)
	{
		if(img->allocation == STB_ALLOCATED)
		{
			stbi_image_free(img->data);
		}else{
		    free(img->data);
		}
		img->data = NULL;
		img->width =0;
		img->height =0;
		img->size = 0;
		img->allocation == NO_ALLOCATION;
	}	
}
int main(void)
{
	Image Prins, profilephoto1;
	Image_load(&Prins, "Prins.jpeg");
	Image_save(&profilephoto1, "profilephoto1.jpg");
	Image_to_gray(&Prins, &profilephoto1);
	Image_save(&profilephoto1, "profilephoto1.jpg");
}
