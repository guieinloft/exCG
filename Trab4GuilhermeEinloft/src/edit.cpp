#include <unistd.h>

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Vector3.h"
#include "bezier.h"
#include "surface.h"
#include "Mouse.h"
#include "Slider.h"

Vector2 bezier[MAX_RES];
Vector2 b_points[4] = {
	(Vector2) {150, -200},
	(Vector2) {  0, -100},
	(Vector2) {300,  100},
	(Vector2) {150,  200}
};

int selpoint = -1;

Slider sl_m(0, 0);
Slider sl_n(0, 0);

void edit_render(Vector3 in[MAX_RES][MAX_RES], Vector2 out[MAX_RES][MAX_RES],
		int m, int n, int d, int screenW, int screenH)
{	
	CV::color(0.5, 0.5, 0.5);
	Vector3 p;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			out[i][j] = proj_ortho(in[i][j], d);
		}
	}
	draw_surface(out, m, n);
	CV::color(1, 0, 0);
	plot_curve(bezier, n);
	plot_points(b_points, 4);
	sl_m.changePosition(-screenW/2 + 8, screenH/2 - 66);
	sl_m.Render();
	sl_n.changePosition(-screenW/2 + 8, screenH/2 - 20);
	sl_n.Render();
	CV::color(0, 0, 0);
	CV::text(-screenW/2 + 4, screenH/2 - 82, "RESOLUCAO HORIZONTAL");
	CV::text(-screenW/2 + 4, screenH/2 - 36, "RESOLUCAO VERTICAL");
	CV::text(-screenW/2, -screenH/2 + 10, "EDICAO");
}

void edit_check_mouse(Mouse mouse, Vector3 in[MAX_RES][MAX_RES],
		int *m, int *n, int d)
{
	if (mouse.button == 0 && mouse.state == 0) {
		selpoint = -1;
		for (int i = 0; i < 4; i++) {
			if (mouse.x >= b_points[i].x - 4 &&
					mouse.x <= b_points[i].x + 4 &&
					mouse.y >= b_points[i].y - 4 &&
					mouse.y <= b_points[i].y + 4) {
				selpoint = i;
				printf("\nPONTO SELECIONADO: %d", selpoint);
				break;
			}
		}
	}
	if (mouse.state == 1)
		selpoint = -1;
	
	sl_m.changeParam((*m - 1) * 2);
	sl_n.changeParam((*n - 1) * 2);
	sl_m.checkMouse(mouse);
	*m = sl_m.getParam() / 2 + 1;
	*m += 1 * (*m < 2);
	sl_n.checkMouse(mouse);
	*n = sl_n.getParam() / 2 + 1;
	*n += 1 * (*n < 2);

	if (mouse.l) {
		change_point(&b_points[selpoint], mouse.x, mouse.y);
		evaluate_curve(b_points, 4, bezier, *n);
		generate_surface(in, bezier, *m, *n, d);
	}
}

void edit_init(Vector3 in[MAX_RES][MAX_RES], int m, int n, int d) {
	evaluate_curve(b_points, 4, bezier, n);
	generate_surface(in, bezier, m, n, d);
}
