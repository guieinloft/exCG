#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "image.h"

void image_close(struct image *self)
{
	if (self->img == NULL) return;
	free(self->img);
}

int image_load(struct image *self, char *path)
{
	//open file
	FILE *file = fopen(path, "rb");
	if (file == NULL) return 0;

	//image_close(self);

	int hsize;
	unsigned short bpp;
	unsigned int offset;
	//header size
	fseek(file, 0xe, SEEK_SET);
	fread(&hsize, sizeof(unsigned int), 1, file);

	//read x and y dimensions
	fseek(file, 0x12, SEEK_SET);
	fread(&self->w, sizeof(unsigned int), 1, file);
	fread(&self->h, sizeof(unsigned int), 1, file);
	//read bytes per pixel
	fseek(file, 0x1c, SEEK_SET);
	fread(&bpp, sizeof(unsigned int), 1, file);
	bpp /= 8;

	//read file offset
	fseek(file, 0xa, SEEK_SET);
	fread(&offset, sizeof(unsigned int), 1, file);

	//create and read data array
	self->img = (unsigned char*)malloc(sizeof(unsigned char)
			* self->w * self->h * 3);
	if (self->img == NULL) return 0;
	fseek(file, offset, SEEK_SET);
	int padding = 4 - ((self->w * bpp) & 3) & 3;
	for (int i = self->h - 1; i > -1; i--) {
		for (unsigned int j = 0; j < self->w; j++) {
			fread(&self->img[(i * self->w + j) * 3 + 2],
					sizeof(unsigned char), 1, file);
			fread(&self->img[(i * self->w + j) * 3 + 1],
					sizeof(unsigned char), 1, file);
			fread(&self->img[(i * self->w + j) * 3 + 0],
					sizeof(unsigned char), 1, file);
			fseek(file, bpp - 3, SEEK_CUR);
		}
		fseek(file, padding, SEEK_CUR);
	}

	//close file
	fclose(file);
	return 1;
}

