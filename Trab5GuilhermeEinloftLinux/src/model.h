struct model {
	struct vec3 *v;
	struct vec3 *vn;
	struct vec2 *vt;
	int size;
	unsigned int texture;
};

void model_load_obj(struct model *model, char *path);
void model_load_texture(struct model *model, char *path);
void model_render(struct model *model);
