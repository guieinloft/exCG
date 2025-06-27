struct meteor {
	struct vec3 pos;
	float scale;
	struct model *model;
};

struct meteor meteor_init(struct model *model);
void meteor_render(struct meteor meteor, struct vec3 cam_pos);
