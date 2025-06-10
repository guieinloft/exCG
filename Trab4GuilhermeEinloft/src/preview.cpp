#include <unistd.h>

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Vector3.h"
#include "bezier.h"
#include "surface.h"
#include "Keys.h"
#include "Button.h"

#define INIT_Z 1.5

Vector3 t(0, 0, INIT_Z);
Vector3 ang(0, 0, 0);
Vector3 light_source(0, -0.5, 0);
bool perp = 1;
bool culling = 0;
bool zbuffer = 0;
bool update = 1;
bool show_normals = 0;

Vector3 normals[MAX_RES][MAX_RES][2];
Vector3 normals_proj[MAX_RES][MAX_RES][2];
Vector3 v_normals[MAX_RES][MAX_RES];
Vector3 v_normals_proj[MAX_RES][MAX_RES];

Button bt_reset_ang(0, 0, 256, 32);
Button bt_reset_t(0, 0, 256, 32);
Button bt_perp(0, 0, 256, 32);
Button bt_normals(0, 0, 256, 32);
Button bt_culling(0, 0, 256, 32);
Button bt_zbuffer(0, 0, 256, 32);

float z_depth[MAX_SW][MAX_SH];
float colors[MAX_SW][MAX_SH];

void prev_render(Vector3 in[MAX_RES][MAX_RES],
		Vector3 out[MAX_RES][MAX_RES],
		Vector3 out_proj[MAX_RES][MAX_RES],
		int m, int n, int d, int screenW, int screenH)
{
	CV::color(0, 0, 0);
	if (update) {
		transform_surface(in, out, m, n, t, ang);
		project_surface(out, out_proj, m, n, d, perp);
		generate_face_normals(out, normals, m, n);
		project_face_normals(out, normals, normals_proj, m, n, d, perp);
	}
	if (zbuffer) {
		if (update) {
			generate_vertex_normals(normals, v_normals, m, n);
			update_z_buffer(out_proj, v_normals, m, n, z_depth,
					colors, light_source);
			update = 0;
			project_vertex_normals(out, v_normals, v_normals_proj, m, n, d, perp);
		}
		draw_light(colors, screenW, screenH);
	} else if (culling) {
		draw_surface_culled(out_proj, m, n);
	} else {
		draw_surface(out_proj, m, n);
	}
	CV::color(1, 0, 0);
	if (show_normals) {
		if (culling)
			draw_face_normals_culled(out_proj, normals_proj, m, n);
		else
			draw_face_normals(out_proj, normals_proj, m, n);
	}
	bt_reset_ang.changePosition(-screenW/2 + 4, screenH/2 - 36);
	bt_reset_ang.Render();
	bt_reset_t.changePosition(-screenW/2 + 4, screenH/2 - 72);
	bt_reset_t.Render();
	bt_perp.changePosition(-screenW/2 + 4, screenH/2 - 108);
	bt_perp.Render();
	bt_normals.changePosition(-screenW/2 + 4, screenH/2 - 144);
	bt_normals.Render();
	bt_culling.changePosition(-screenW/2 + 4, screenH/2 - 180);
	bt_culling.Render();
	bt_zbuffer.changePosition(-screenW/2 + 4, screenH/2 - 216);
	bt_zbuffer.Render();
	update = 0;
}

void prev_check_keypress(int key, bool shift, bool ctrl)
{
	switch (key) {
	case 200: // left
		if (shift) ang.z -= 0.1;
		else t.x -= 0.1;
		update = 1;
		break;
	case 201: // up
		if (shift) ang.x -= 0.1;
		else t.y -= 0.1;
		update = 1;
		break;
	case 202: // right
		if (shift) ang.z += 0.1;
		else t.x += 0.1;
		update = 1;
		break;
	case 203: // down
		if (shift) ang.x += 0.1;
		else t.y += 0.1;
		update = 1;
		break;
	case 204: // pgup
		if (shift) ang.y -= 0.1;
		else t.z -= 0.1;
		update = 1;
		break;
	case 205: // pgdown
		if (shift) ang.y += 0.1;
		else t.z += 0.1;
		update = 1;
		break;
	case 'p':
		perp = !perp;
		update = 1;
		if (perp)
			bt_perp.changeText("PROJ. ORTOGRAFICA");
		else
			bt_perp.changeText("PROJ. PERSPECTIVA");
		break;
	case 'r':
		t.set(0, 0, INIT_Z);
		update = 1;
		break;
	case 'R':
		ang.set(0, 0, 0);
		update = 1;
		break;
	}
}

void prev_check_mouse(Mouse mouse)
{
	bt_reset_ang.checkClick(mouse);
	if (bt_reset_ang.isSelected()) {
		ang.set(0, 0, 0);
		update = 1;
	}
	bt_reset_t.checkClick(mouse);
	if (bt_reset_t.isSelected()) {
		t.set(0, 0, INIT_Z);
		update = 1;
	}
	bt_perp.checkClick(mouse);
	if (bt_perp.isSelected()) {
		perp = !perp;
		if (perp)
			bt_perp.changeText("PROJ. ORTOGRAFICA");
		else
			bt_perp.changeText("PROJ. PERSPECTIVA");
		update = 1;
	}
	bt_normals.checkClick(mouse);
	if (bt_normals.isSelected()) {
		show_normals = !show_normals;
		if (show_normals)
			bt_normals.changeText("OCULTAR NORMAIS");
		else
			bt_normals.changeText("MOSTRAR NORMAIS");
	}
	bt_culling.checkClick(mouse);
	if (bt_culling.isSelected()) {
		culling = !culling;
		if (culling)
			bt_culling.changeText("DESATIVAR CULLING");
		else
			bt_culling.changeText("ATIVAR CULLING");
	}
	bt_zbuffer.checkClick(mouse);
	if (bt_zbuffer.isSelected()) {
		zbuffer = !zbuffer;
		if (zbuffer)
			bt_zbuffer.changeText("DESATIVAR Z-BUFFER");
		else
			bt_zbuffer.changeText("ATIVAR Z-BUFFER");
		update = 1;
	}
}

void prev_init()
{
	bt_reset_ang.changeText("RESETAR ANGULO");
	bt_reset_t.changeText("RESETAR TRANSLACAO");
	bt_perp.changeText("PROJ. ORTOGRAFICA");
	bt_normals.changeText("MOSTRAR NORMAIS");
	bt_culling.changeText("ATIVAR CULLING");
	bt_zbuffer.changeText("ATIVAR Z-BUFFER");
}

void prev_update()
{
	update = 1;
}
