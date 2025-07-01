/*
 * struct da camera
 * guarda posicao, direcao, vetor up, angulos, velocidade, etc.
 */
struct camera {
	struct vec3 pos;
	struct vec3 dir;
	struct vec3 up;
	float yaw, pitch, yaw_inc, pitch_inc;
	float speed, speed_inc;
	int speed_dir;
	float fov, znear, zfar, aspect;
};

/* inicializa a camera */
void camera_init(struct camera *cam, struct vec3 pos, int screenW, int screenH);
/* faz a projecao na tela */
void camera_display(struct camera *cam, int screenW, int screenH);
/* atualiza a posicao da camera apos a renderizacao */
void camera_update(struct camera *cam, float deltatime);
/* verifica tecla pressionada */
void camera_check_key_press(struct camera *cam, int key);
/* verifica tecla solta */
void camera_check_key_release(struct camera *cam, int key);
/* verifica posicao do mouse */
void camera_check_mouse(struct camera *cam, int x, int y,
		int screenW, int screenH);
