/*
 * preview.h
 * possui as funções principais do menu de visualização
 */

/* render do menu de visualização */
void prev_render(Vector3 in[MAX_RES][MAX_RES], Vector3 out[MAX_RES][MAX_RES],
		Vector3 out_proj[MAX_RES][MAX_RES],
		int m, int n, int d, int screenW, int screenH);
/* verifica tecla pressionada e estado do shift e ctrl */
void prev_check_keypress(int key, bool shift, bool ctrl);
/* verifica posição e estado do mouse */
void prev_check_mouse(Mouse mouse);
/* inicializa botões */
void prev_init();
/* atualiza o objeto, as normais e o z-buffer */
void prev_update(Vector3 in[MAX_RES][MAX_RES], Vector3 out[MAX_RES][MAX_RES],
		Vector3 out_proj[MAX_RES][MAX_RES], int m, int n, int d);
