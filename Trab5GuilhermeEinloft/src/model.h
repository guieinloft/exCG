struct model {
	float *v;
	float *vn;
	float *vt;
	int size;
	GLuint vbuffer;
	GLuint vnbuffer;
	GLuint vtbuffer;
};

void model_load(struct model *model, char *path);
void model_render(struct model *model);
