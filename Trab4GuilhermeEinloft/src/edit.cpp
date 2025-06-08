#include <unistd.h>

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Vector3.h"
#include "bezier.h"
#include "surface.h"
#include "Mouse.h"
#include "Slider.h"
#include "edit.h"

#define MAX_POINTS 20

Vector2 bezier[MAX_RES];
Vector2 b_points[20];
int n_points = 4;
int selpoint = -1;

Slider sl_m(0, 0);
Slider sl_n(0, 0);
Button bt_add(0, 0, 256, 32, true);
Button bt_rm(0, 0, 256, 32, true);

bool snap = 0, add = 0, rm = 0;

inline float get_snap_position(float a)
{
	return round(a / SNAP_RES) * SNAP_RES;
}

void find_selected_point(float x, float y)
{
	selpoint = -1;
	for (int i = 0; i < n_points; i++) {
		if (x >= b_points[i].x - 4 &&
				x <= b_points[i].x + 4 &&
				y >= b_points[i].y - 4 &&
				y <= b_points[i].y + 4) {
			selpoint = i;
			return;
		}
	}
}

void add_point(float x, float y, bool shift)
{
	if (shift)
		b_points[n_points].set(get_snap_position(x),
				get_snap_position(y));
	else
		b_points[n_points].set(x, y);
	
	selpoint = n_points++;
	add = 0;
}

void remove_point()
{
	for (int i = selpoint; i < n_points; i++)
		b_points[i] = b_points[i+1];
	
	n_points--;
	selpoint = -1;
	rm = 0;
}


void edit_point(float x, float y, bool shift)
{
	if (selpoint == -1)
		return;
	if (shift)
		change_point(&b_points[selpoint],
				get_snap_position(x),
				get_snap_position(y));
	else
		change_point(&b_points[selpoint], x, y);
}

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
	plot_points(b_points, n_points);
	bt_add.changePosition(-screenW/2 + 8, screenH/2 - 164);
	bt_add.Render();
	bt_rm.changePosition(-screenW/2 + 8, screenH/2 - 128);
	bt_rm.Render();
	sl_m.Render();
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
		int *m, int *n, int d, bool shift)
{
	bt_add.checkClick(mouse);
	bt_rm.checkClick(mouse);
	if (bt_add.isPressed()) {
		add = 1;
		return;
	}
	if (bt_rm.isPressed()) {
		rm = 1;
		return;
	}
	sl_m.changeParam((*m - 1) * 2);
	sl_n.changeParam((*n - 1) * 2);
	sl_m.checkMouse(mouse);
	*m = sl_m.getParam() / 2 + 1;
	*m += 1 * (*m < 2);
	sl_n.checkMouse(mouse);
	*n = sl_n.getParam() / 2 + 1;
	*n += 1 * (*n < 2);

	if (mouse.button == 0 && mouse.state == 0) {
		if (add && n_points < MAX_POINTS)
			add_point(mouse.x, mouse.y, shift);
		else {
			find_selected_point(mouse.x, mouse.y);
			if (rm && n_points > 2)
				remove_point();
		}
	}
	if (mouse.state == 1)
		selpoint = -1;
	if (mouse.l) {
		edit_point(mouse.x, mouse.y, shift);
		evaluate_curve(b_points, n_points, bezier, *n);
		generate_surface(in, bezier, *m, *n, d);
	}
}

void edit_init(Vector3 in[MAX_RES][MAX_RES],
		int m, int n, int d)
{
	b_points[0].set(120, -200);
	b_points[1].set(  0, -100);
	b_points[2].set(300,  100);
	b_points[3].set(120,  200);
	bt_add.changeText("ADICIONAR PONTO");
	bt_rm.changeText("REMOVER PONTO");
	evaluate_curve(b_points, 4, bezier, n);
	generate_surface(in, bezier, m, n, d);
}
