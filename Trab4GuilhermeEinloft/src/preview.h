void prev_render(Vector3 in[MAX_RES][MAX_RES], Vector3 out[MAX_RES][MAX_RES],
		Vector3 out_proj[MAX_RES][MAX_RES],
		int m, int n, int d, int screenW, int screenH);
void prev_check_keypress(int key, bool shift, bool ctrl);
void prev_check_mouse(Mouse mouse);
void prev_init();
void prev_update_zbuffer();
