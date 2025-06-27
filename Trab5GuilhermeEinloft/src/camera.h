struct camera {
	struct vec3 pos;
	struct vec3 dir;
	struct vec3 up;
	float yaw, pitch, yaw_inc, pitch_inc;
	float speed, speed_inc;
	int speed_dir;
	float fov, znear, zfar, aspect;
};

void camera_init(struct camera *cam, struct vec3 pos, int screenW, int screenH);
void camera_display(struct camera *cam, int screenW, int screenH);
void camera_update(struct camera *cam, float deltatime);
void camera_check_key_press(struct camera *cam, unsigned char key);
void camera_check_key_release(struct camera *cam, unsigned char key);
void camera_check_mouse(struct camera *cam, int x, int y,
		int screenW, int screenH);
