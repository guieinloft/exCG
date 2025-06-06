#include <unistd.h>

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Vector3.h"
#include "bezier.h"
#include "surface.h"
#include "Keys.h"
#include "Button.h"

Vector3 t(0, 0, 2);
Vector3 ang(0, 0, 0);
Vector3 t_inc(0, 0, 0);
Vector3 ang_inc(0, 0, 0);
bool perp = 1;

Button bt_reset_ang(0, 0, 256, 32);
Button bt_reset_t(0, 0, 256, 32);
Button bt_perp(0, 0, 256, 32);

void prev_render(Vector3 in[MAX_RES][MAX_RES], Vector2 out[MAX_RES][MAX_RES],
		int m, int n, int d, int screenW, int screenH)
{
	CV::color(0, 0, 0);
	Vector3 p;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			p = in[i][j];
			p = rotate(p, ang);
			p = translate(p, t);
			if (perp)
				out[i][j] = proj_perp(p, d);
			else
				out[i][j] = proj_ortho(p, d);
		}
	}
	draw_surface(out, m, n);
	t += t_inc;
	ang += ang_inc;
	bt_reset_ang.changePosition(-screenW/2 + 4, screenH/2 - 36);
	bt_reset_ang.Render();
	bt_reset_t.changePosition(-screenW/2 + 4, screenH/2 - 72);
	bt_reset_t.Render();
	bt_perp.changePosition(-screenW/2 + 4, screenH/2 - 108);
	bt_perp.Render();
}

void prev_check_keys(Keys keys)
{
	ang_inc.set(0, 0, 0);
	t_inc.set(0, 0, 0);
	if (keys.shift) {
		if (keys.k_left) ang_inc.z -= 0.015625;
		if (keys.k_right) ang_inc.z += 0.015625;
		if (keys.k_up) ang_inc.x -= 0.015625;
		if (keys.k_down) ang_inc.x += 0.015625;
		if (keys.k_pgup) ang_inc.y -= 0.015625;
		if (keys.k_pgdown) ang_inc.y += 0.015625;
	} else {
		if (keys.k_left) t_inc.x -= 0.015625;
		if (keys.k_right) t_inc.x += 0.015625;
		if (keys.k_up) t_inc.y -= 0.015625;
		if (keys.k_down) t_inc.y += 0.015625;
		if (keys.k_pgup) t_inc.z -= 0.015625;
		if (keys.k_pgdown) t_inc.z += 0.015625;
	}
}

void prev_check_mouse(Mouse mouse)
{
	bt_reset_ang.checkClick(mouse);
	if (bt_reset_ang.isSelected())
		ang.set(0, 0, 0);
	bt_reset_t.checkClick(mouse);
	if (bt_reset_t.isSelected())
		t.set(0, 0, 2);
	bt_perp.checkClick(mouse);
	if (bt_perp.isSelected()) {
		perp = !perp;
		if (perp)
			bt_perp.changeText("PROJ. ORTOGRAFICA");
		else
			bt_perp.changeText("PROJ. PERSPECTIVA");
	}
}

void prev_init() {
	bt_reset_ang.changeText("RESETAR ANGULO");
	bt_reset_t.changeText("RESETAR TRANSLACAO");
	bt_perp.changeText("PROJ. ORTOGRAFICA");
}
