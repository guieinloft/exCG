struct image {
	uint8_t *img;
	int w;
	int h;
};

void image_close(struct image *self);
bool image_load(struct image *self, char *path);
