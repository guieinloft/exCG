/*
 * meteoro
 * guarda a posicao e o modelo
 */
struct meteor {
	struct vec3 pos;
	struct model *model;
};

/* inicializa a posicao do meteoro e guarda o modelo */
struct meteor meteor_init(struct model *model);
/* renderiza o meteoro */
void meteor_render(struct meteor meteor, struct vec3 cam_pos);
