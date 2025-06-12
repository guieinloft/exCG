#include "gl_canvas2d.h"
#include "Vector3.h"
#include "bezier.h"
#include "surface.h"
#include "Keys.h"
#include "Button.h"

#define INIT_Z 1.5

Vector3 t(0, 0, INIT_Z);
Vector3 ang(0, 0, 0);
Vector3 lightdir(0, 0, -1);
float light_y = 0;
float light_x = 0;
bool perp = 1;
bool culling = 0;
bool zbuffer = 0;
bool update = 1;
bool show_normals = 0;

Vector3 normals[MAX_RES][MAX_RES][2];
Vector3 normals_proj[MAX_RES][MAX_RES][2][2];
Vector3 v_normals[MAX_RES][MAX_RES];
Vector3 v_normals_proj[MAX_RES][MAX_RES];

Button bt_reset_ang(0, 0, 256, 32);
Button bt_reset_t(0, 0, 256, 32);
Button bt_perp(0, 0, 256, 32);
Button bt_normals(0, 0, 256, 32);
Button bt_culling(0, 0, 256, 32);
Button bt_zbuffer(0, 0, 256, 32);
Button bt_reset_light(0, 0, 256, 32);

float z_depth[MAX_SW][MAX_SH];
float colors[MAX_SW][MAX_SH];

void prev_render_buttons(int screenW, int screenH)
{
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
	char info[60];
	if (zbuffer) {
		bt_reset_light.changePosition(-screenW/2 + 4, screenH/2 - 252);
		bt_reset_light.Render();
		sprintf(info, "DIRECAO DA LUZ (ctrl + setas): (%+5.2f, %+5.2f, %+5.2f)",
				lightdir.x, lightdir.y, lightdir.z);
		CV::text(-screenW/2 + 264, screenH/2 - 32, info);
	}
	sprintf(info, "POSICAO DO OBJETO (setas): (%+5.2f, %+5.2f, %+5.2f)",
			t.x, t.y, t.z);
	CV::text(-screenW/2 + 264, screenH/2 - 18, info);
	sprintf(info, "ANGULO DO OBJETO (shift + setas): (%+5.2f, %+5.2f, %+5.2f)",
			ang.x, ang.y, ang.z);
	CV::text(-screenW/2 + 264, screenH/2 - 4, info);
}

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
		if (zbuffer) {
			generate_vertex_normals(normals, v_normals, m, n);
			update_z_buffer(out_proj, v_normals, m, n, z_depth,
					colors, lightdir);
			update = 0;
		}
	}
	if (zbuffer) {
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
	update = 0;
	prev_render_buttons(screenW, screenH);
}

void prev_check_keypress(int key, bool shift, bool ctrl)
{
	switch (key) {
	case 200: // left
		if (ctrl) lightdir = rotate(lightdir, Vector3(0, 0, -0.1));
		else if (shift) ang.z -= 0.1;
		else t.x -= 0.1;
		update = 1;
		break;
	case 201: // up
		if (ctrl) lightdir = rotate(lightdir, Vector3(-0.1, 0, 0));
		else if (shift) ang.x -= 0.1;
		else t.y -= 0.1;
		update = 1;
		break;
	case 202: // right
		if (ctrl) lightdir = rotate(lightdir, Vector3(0, 0, +0.1));
		else if (shift) ang.z += 0.1;
		else t.x += 0.1;
		update = 1;
		break;
	case 203: // down
		if (ctrl) lightdir = rotate(lightdir, Vector3(+0.1, 0, 0));
		else if (shift) ang.x += 0.1;
		else t.y += 0.1;
		update = 1;
		break;
	case 204: // pgup
		if (ctrl) lightdir = rotate(lightdir, Vector3(0, -0.1, 0));
		else if (shift) ang.y -= 0.1;
		else t.z -= 0.1;
		update = 1;
		break;
	case 205: // pgdown
		if (ctrl) lightdir = rotate(lightdir, Vector3(0, +0.1, 0));
		else if (shift) ang.y += 0.1;
		else t.z += 0.1;
		update = 1;
		break;
	}
}

void prev_check_mouse(Mouse mouse)
{
	bt_reset_ang.checkClick(mouse);
	if (bt_reset_ang.isPressed()) {
		ang.set(0, 0, 0);
		update = 1;
	}
	bt_reset_t.checkClick(mouse);
	if (bt_reset_t.isPressed()) {
		t.set(0, 0, INIT_Z);
		update = 1;
	}
	bt_perp.checkClick(mouse);
	if (bt_perp.isPressed()) {
		perp = !perp;
		if (perp)
			bt_perp.changeText("PROJ. ORTOGRAFICA");
		else
			bt_perp.changeText("PROJ. PERSPECTIVA");
		update = 1;
	}
	bt_normals.checkClick(mouse);
	if (bt_normals.isPressed()) {
		show_normals = !show_normals;
		if (show_normals)
			bt_normals.changeText("OCULTAR NORMAIS");
		else
			bt_normals.changeText("MOSTRAR NORMAIS");
	}
	bt_culling.checkClick(mouse);
	if (bt_culling.isPressed()) {
		culling = !culling;
		if (culling)
			bt_culling.changeText("DESATIVAR CULLING");
		else
			bt_culling.changeText("ATIVAR CULLING");
	}
	bt_zbuffer.checkClick(mouse);
	if (bt_zbuffer.isPressed()) {
		zbuffer = !zbuffer;
		if (zbuffer)
			bt_zbuffer.changeText("DESATIVAR Z-BUFFER");
		else
			bt_zbuffer.changeText("ATIVAR Z-BUFFER");
		update = 1;
	}
	if (zbuffer) {
		bt_reset_light.checkClick(mouse);
		if (bt_reset_light.isPressed()) {
			lightdir.set(0, 0, -1);
			update = 1;
		}
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
	bt_reset_light.changeText("RESETAR DIRECAO DA LUZ");
}

void prev_update()
{
	update = 1;
}
