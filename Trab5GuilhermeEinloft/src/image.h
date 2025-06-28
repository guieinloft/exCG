struct image {
	unsigned char *img;
	unsigned int w;
	unsigned int h;
};

void image_close(struct image *self);
int image_load(struct image *self, char *path);
