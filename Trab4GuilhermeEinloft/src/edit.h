/*
 * edit.h
 * possui as funções principais do menu de edição
 */

#define SNAP_RES 20

/* render do menu de edição, mostra curva, objeto e botões */
void edit_render(Vector3 in[MAX_RES][MAX_RES], Vector3 out[MAX_RES][MAX_RES],
		int m, int n, int d, int screenW, int screenH);
/* verifica a posição do mouse e estado das teclas shift e ctrl */
void edit_check_mouse(Mouse mouse, Vector3 in[MAX_RES][MAX_RES],
		int *m, int *n, int d, bool shift, bool ctrl);
/* inicializa curva, botões e sliders */
void edit_init(Vector3 in[MAX_RES][MAX_RES],
		int m, int n, int d);
