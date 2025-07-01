/*
 * modelo
 * guarda os vertices, as normais dos vertices e as coordenadas da textura
 */
struct model {
	struct vec3 *v;
	struct vec3 *vn;
	struct vec2 *vt;
	int size;
	unsigned int texture;
};

/* carrega um arquivo obj */
/* OBS: o modelo deve ter apenas faces triangulares */
void model_load_obj(struct model *model, char *path);
/* carrega uma textura (bmp) */
void model_load_texture(struct model *model, char *path);
/* renderiza o modelo */
void model_render(struct model *model);
/* libera a memoria alocada */
void model_close(struct model *model);
