#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "image.h"

void image_close(struct image *self)
{
	if (self->img == NULL) return;
	free(self->img);
}

bool image_load(struct image *self, char *path)
{
	//open file
	FILE *file = fopen(path, "rb");
	if (file == NULL) return false;

	//image_close(self);

	int hsize;
	uint16_t bpp;
	uint32_t offset;
	//header size
	fseek(file, 0xe, SEEK_SET);
	fread(&hsize, sizeof(uint32_t), 1, file);

	//read x and y dimensions
	fseek(file, 0x12, SEEK_SET);
	fread(&self->w, sizeof(uint32_t), 1, file);
	fread(&self->h, sizeof(uint32_t), 1, file);
	//read bytes per pixel
	fseek(file, 0x1c, SEEK_SET);
	fread(&bpp, sizeof(uint32_t), 1, file);
	bpp /= 8;

	//read file offset
	fseek(file, 0xa, SEEK_SET);
	fread(&offset, sizeof(uint32_t), 1, file);

	//create and read data array
	self->img = (uint8_t*)malloc(sizeof(uint8_t) * self->w * self->h * 3);
	if (self->img == NULL) return false;
	fseek(file, offset, SEEK_SET);
	int padding = 4 - ((self->w * bpp) & 3) & 3;
	for (int i = self->h - 1; i > -1; i--) {
		for (uint32_t j = 0; j < self->w; j++) {
			fread(&self->img[(i * self->w + j) * 3 + 2], sizeof(uint8_t), 1, file);
			fread(&self->img[(i * self->w + j) * 3 + 1], sizeof(uint8_t), 1, file);
			fread(&self->img[(i * self->w + j) * 3 + 0], sizeof(uint8_t), 1, file);
			fseek(file, bpp - 3, SEEK_CUR);
		}
		fseek(file, padding, SEEK_CUR);
	}

	//close file
	fclose(file);
	return true;
}

