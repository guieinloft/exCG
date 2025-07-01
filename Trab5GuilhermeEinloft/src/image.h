/*
 * imagem
 * guarda pixels (rgb), largura e altura
 */
struct image {
	unsigned char *img;
	unsigned int w;
	unsigned int h;
};

/* libera o array de pixels */
void image_close(struct image *self);
/* abre uma imagem localizada no path */
int image_load(struct image *self, char *path);
