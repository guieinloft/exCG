#define SNAP_RES 20

void edit_render(Vector3 in[MAX_RES][MAX_RES], Vector3 out[MAX_RES][MAX_RES],
		int m, int n, int d, int screenW, int screenH);
void edit_check_mouse(Mouse mouse, Vector3 in[MAX_RES][MAX_RES],
		int *m, int *n, int d, bool shift, bool ctrl);
void edit_init(Vector3 in[MAX_RES][MAX_RES],
		int m, int n, int d);
