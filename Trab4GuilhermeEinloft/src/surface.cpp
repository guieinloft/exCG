#include <math.h>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Vector3.h"
#include "bezier.h"
#include "surface.h"

bool is_ccw(Vector3 v0, Vector3 v1, Vector3 v2)
{
	float a = (v1.x - v0.x) * (v2.y - v0.y);
	float b = (v2.x - v0.x) * (v1.y - v0.y);
	return (a < b);
}

bool point_in_triangle(Vector3 a, Vector3 b, Vector3 c,
		Vector3 p, Vector3 *weights)
{
	float w1, w2, w3;
	w1 = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) /
			((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
	w2 = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) /
			((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
	w3 = 1 - w1 - w2;

	weights->set(w1, w2, w3);

	return (w1 >= 0 && w2 >= 0 && w3 >= 0);
}

Vector3 rotate(Vector3 p, Vector3 a)
{
	float sin_ax = sin(a.x);
	float cos_ax = cos(a.x);
	float sin_ay = sin(a.y);
	float cos_ay = cos(a.y);
	float sin_az = sin(a.z);
	float cos_az = cos(a.z);
	Vector3 p2;
	if (a.x != 0) {
		p2.set(p.x, p.y * cos_ax - p.z * sin_ax,
				p.y * sin_ax + p.z * cos_ax);
		p = p2;
	}
	if (a.y != 0) {
		p2.set(p.x * cos_ay + p.z * sin_ay, p.y,
				-p.x * sin_ay + p.z * cos_ay);
		p = p2;
	}
	if (a.z != 0) {
		p2.set(p.x * cos_az - p.y * sin_az,
				p.x * sin_az + p.y * cos_az, p.z);
		p = p2;
	}
	return p;
}

Vector3 translate(Vector3 p, Vector3 d)
{
	Vector3 p2(p.x + d.x, p.y + d.y, p.z + d.z);
	return p2;
}

Vector3 proj_perp(Vector3 p, float d)
{
	Vector3 p2(p.x * d / p.z, p.y * d / p.z, p.z);
	return p2;
}

Vector3 proj_ortho(Vector3 p, float d)
{
	Vector3 p2(p.x * d, p.y * d, p.z);
	return p2;
}

void draw_surface(Vector3 v[MAX_RES][MAX_RES], int m, int n)
{
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			CV::line(v[i][j].x, v[i][j].y,
					v[i + 1][j].x, v[i + 1][j].y);
			CV::line(v[i + 1][j].x, v[i + 1][j].y,
					v[i + 1][j + 1].x, v[i + 1][j + 1].y);
			CV::line(v[i][j].x, v[i][j].y,
					v[i + 1][j + 1].x, v[i + 1][j + 1].y);
			CV::line(v[i][j].x, v[i][j].y,
					v[i][j + 1].x, v[i][j + 1].y);
			CV::line(v[i][j + 1].x, v[i][j + 1].y,
					v[i + 1][j + 1].x, v[i + 1][j + 1].y);
		}
	}
}

void draw_surface_culled(Vector3 v[MAX_RES][MAX_RES], int m, int n)
{
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (!is_ccw(v[i][j], v[i + 1][j], v[i + 1][j + 1])) {
				CV::line(v[i][j].x, v[i][j].y,
						v[i + 1][j].x, v[i + 1][j].y);
				CV::line(v[i + 1][j].x, v[i + 1][j].y,
						v[i + 1][j + 1].x, v[i + 1][j + 1].y);
				CV::line(v[i][j].x, v[i][j].y,
						v[i + 1][j + 1].x, v[i + 1][j + 1].y);
			}
			if (!is_ccw(v[i][j], v[i + 1][j + 1], v[i][j + 1])) {
				CV::line(v[i][j].x, v[i][j].y,
						v[i][j + 1].x, v[i][j + 1].y);
				CV::line(v[i][j + 1].x, v[i][j + 1].y,
						v[i + 1][j + 1].x, v[i + 1][j + 1].y);
				CV::line(v[i][j].x, v[i][j].y,
						v[i + 1][j + 1].x, v[i + 1][j + 1].y);
			}
		}
	}
}

void generate_surface(Vector3 out[MAX_RES][MAX_RES],
		Vector2 in[MAX_RES], int m, int n, float d, float offset)
{
	m--; //para facilitar calculos da funcao
	float step = PI_2 / m;
	for (int i = 0; i <= m; i++) {
		for (int j = 0; j < n; j++) {
			out[i][j].set(in[j].x / d * cos(step * i),
					in[j].y / d + ((double)i / m - 0.5) * offset,
					in[j].x / d * sin(step * i));
		}
	}
}

void transform_surface(Vector3 in[MAX_RES][MAX_RES],
		Vector3 out[MAX_RES][MAX_RES],
		int m, int n, Vector3 t, Vector3 ang)
{
	Vector3 p;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			p = in[i][j];
			p = rotate(p, ang);
			out[i][j] = translate(p, t);
		}
	}
}

void project_surface(Vector3 in[MAX_RES][MAX_RES],
		Vector3 out[MAX_RES][MAX_RES],
		int m, int n, int d, bool perp)
{
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (perp)
				out[i][j] = proj_perp(in[i][j], d);
			else
				out[i][j] = proj_ortho(in[i][j], d);
		}
	}
}

void generate_face_normals(Vector3 in[MAX_RES][MAX_RES],
		Vector3 normals[MAX_RES][MAX_RES][2],
		int m, int n)
{
	Vector3 edge1, edge2;
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (in[i][j] == in[i + 1][j]) {
				normals[i][j][0].set(0, 0, 0);
			} else {
				edge1 = in[i][j] - in[i + 1][j];
				edge2 = in[i + 1][j + 1] - in[i + 1][j];
				normals[i][j][0] = edge1.cross(edge2);
				normals[i][j][0].normalize();
			}
			if (in[i][j + 1] == in[i + 1][j + 1]) {
				normals[i][j][1].set(0, 0, 0);
			} else {
				edge1 = in[i][j] - in[i][j + 1];
				edge2 = in[i + 1][j + 1] - in[i][j + 1];
				normals[i][j][1] = edge2.cross(edge1);
				normals[i][j][1].normalize();
			}
		}
	}
}

void project_face_normals(Vector3 out[MAX_RES][MAX_RES],
		Vector3 normals[MAX_RES][MAX_RES][2],
		Vector3 normals_proj[MAX_RES][MAX_RES][2][2],
		int m, int n, int d, bool perp)
{
	Vector3 midpoint1;
	Vector3 midpoint2;
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			midpoint1 = (out[i][j] + out[i + 1][j] +
					out[i + 1][j + 1]) * (1.0/3);
			midpoint2 = (out[i][j] + out[i][j + 1] +
					out[i + 1][j + 1]) * (1.0/3);
			if (perp) {
				normals_proj[i][j][0][0] = proj_perp(midpoint1, d);
				normals_proj[i][j][0][1] = proj_perp(
						midpoint1 + (normals[i][j][0] * 0.1),
						d);
				normals_proj[i][j][1][0] = proj_perp(midpoint2, d);
				normals_proj[i][j][1][1] = proj_perp(
						midpoint2 + (normals[i][j][1] * 0.1),
						d);
			}
			else {
				normals_proj[i][j][0][0] = proj_ortho(midpoint1, d);
				normals_proj[i][j][0][1] = proj_ortho(
						midpoint1 + (normals[i][j][0] * 0.1),
						d);
				normals_proj[i][j][1][0] = proj_ortho(midpoint2, d);
				normals_proj[i][j][1][1] = proj_ortho(
						midpoint2 + (normals[i][j][1] * 0.1),
						d);
			}
		}
	}
}

void draw_face_normals_culled(Vector3 out_proj[MAX_RES][MAX_RES],
		Vector3 normals_proj[MAX_RES][MAX_RES][2][2], int m, int n)
{
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (!is_ccw(out_proj[i][j], out_proj[i + 1][j],
					out_proj[i + 1][j + 1])) {
				CV::line(normals_proj[i][j][0][0].x,
						normals_proj[i][j][0][0].y,
						normals_proj[i][j][0][1].x,
						normals_proj[i][j][0][1].y);
			}
			if (!is_ccw(out_proj[i][j], out_proj[i + 1][j],
					out_proj[i + 1][j + 1])) {
				CV::line(normals_proj[i][j][1][0].x,
						normals_proj[i][j][1][0].y,
						normals_proj[i][j][1][1].x,
						normals_proj[i][j][1][1].y);
			}
		}
	}
}

void draw_face_normals(Vector3 out_proj[MAX_RES][MAX_RES],
		Vector3 normals_proj[MAX_RES][MAX_RES][2][2], int m, int n)
{
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			CV::line(normals_proj[i][j][0][0].x,
					normals_proj[i][j][0][0].y,
					normals_proj[i][j][0][1].x,
					normals_proj[i][j][0][1].y);
			CV::line(normals_proj[i][j][1][0].x,
					normals_proj[i][j][1][0].y,
					normals_proj[i][j][1][1].x,
					normals_proj[i][j][1][1].y);
		}
	}
}

void generate_vertex_normals(Vector3 normals[MAX_RES][MAX_RES][2],
		Vector3 v_normals[MAX_RES][MAX_RES], int m, int n)
{
	for (int i = 0; i < m; i++) {
		v_normals[i][0] =
				normals[i % (m - 1)][0][0] +
				normals[i % (m - 1)][0][1] +
				normals[(i - 2 + m) % (m - 1)][0][0] +
				normals[(i - 2 + m) % (m - 1)][0][1];
		v_normals[i][0].normalize();
		for (int j = 1; j < (n - 1); j++) {
			v_normals[i][j] =
					normals[i % (m - 1)][j][0] +
					normals[i % (m - 1)][j][1] +
					normals[i % (m - 1)][(j - 2 + n) % (n - 1)][0] +
					normals[i % (m - 1)][(j - 2 + n) % (n - 1)][1] +
					normals[(i - 2 + m) % (m - 1)][j][0] +
					normals[(i - 2 + m) % (m - 1)][j][1] +
					normals[(i - 2 + m) % (m - 1)][(j - 2 + n) % (n - 1)][0] +
					normals[(i - 2 + m) % (m - 1)][(j - 2 + n) % (n - 1)][1];
			v_normals[i][j].normalize();
		}
		v_normals[i][n - 1] =
				normals[i % (m - 1)][n - 2][0] +
				normals[i % (m - 1)][n - 2][1] +
				normals[(i - 2 + m) % (m - 1)][n - 2][0] +
				normals[(i - 2 + m) % (m - 1)][n - 2][1];
		v_normals[i][n - 1].normalize();
	}
}

Vector2 find_min_point(Vector3 a, Vector3 b, Vector3 c)
{
	Vector2 point(fmax(fmin(a.x, fmin(b.x, c.x)), -MAX_SW / 2),
			fmax(fmin(a.y, fmin(b.y, c.y)), -MAX_SH / 2));
	return point;
}

Vector2 find_max_point(Vector3 a, Vector3 b, Vector3 c)
{
	Vector2 point(fmin(fmax(a.x, fmax(b.x, c.x)), MAX_SW / 2), 
			fmin(fmax(a.y, fmax(b.y, c.y)), MAX_SH / 2));
	return point;
}

float calc_light(Vector3 normal, Vector3 light_source)
{
	normal.normalize();
	float light_intensity = (normal.dot(light_source) + 1) * 0.5;
	return light_intensity;
}

void update_tri_z_buffer(Vector3 a, Vector3 b, Vector3 c,
		Vector3 an, Vector3 bn, Vector3 cn, Vector3 light_source,
		float z_depth[MAX_SW][MAX_SH], float colors[MAX_SW][MAX_SH])
{
	Vector3 weights;
	Vector3 normal;
	Vector3 depths(a.z, b.z, c.z);
	Vector2 p1, p2;
	p1 = find_min_point(a, b, c);
	p2 = find_max_point(a, b, c);
	for (int y = p1.y; y <= p2.y; y++) {
		for (int x = p1.x; x <= p2.x; x++) {
			int x_adj = x + MAX_SW / 2;
			int y_adj = y + MAX_SH / 2;
			if (x_adj < 0 || x_adj >= MAX_SW ||
					y_adj < 0 || y_adj >= MAX_SH)
				continue;
			if (point_in_triangle(a, b, c, Vector3(x, y, 0),
					&weights)) {
				normal.set(0, 0, 0);
				float depth = depths.dot(weights);
				if (depth > z_depth[x_adj][y_adj])
					continue;
				z_depth[x_adj][y_adj] = depth;
				normal += an * weights.x;
				normal += bn * weights.y;
				normal += cn * weights.z;
				colors[x_adj][y_adj] = calc_light(normal,
						light_source);
			}
		}
	}
}

void update_z_buffer(Vector3 v[MAX_RES][MAX_RES], Vector3 vn[MAX_RES][MAX_RES],
		int m, int n, float z_depth[MAX_SW][MAX_SH],
		float colors[MAX_SW][MAX_SH], Vector3 light_source)
{
	memset(z_depth, 0x7f, sizeof(float) * MAX_SW * MAX_SH);
	memset(colors, 0x7f, sizeof(float) * MAX_SW * MAX_SH);
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			update_tri_z_buffer(v[i][j], v[i + 1][j],
					v[i + 1][j + 1],
					vn[i][j], vn[i + 1][j],
					vn[i + 1][j + 1],
					light_source, z_depth, colors);
			update_tri_z_buffer(v[i][j], v[i + 1][j + 1],
					v[i][j + 1],
					vn[i][j], vn[i + 1][j + 1],
					vn[i][j + 1],
					light_source, z_depth, colors);
		}
	}
	
}

void draw_z_buffer(float z_depth[MAX_SW][MAX_SH], int screenW, int screenH)
{
	float depth, color;
	int screenW_2 = screenW * 0.5;
	int screenH_2 = screenH * 0.5;
	for (int y = 0; y < screenH; y++) {
		for (int x = 0; x < screenW; x++) {
			depth = z_depth[x - screenW_2 + MAX_SW / 2]
					[y - screenH_2 + MAX_SH / 2];
			if (depth > 100000000)
				continue;
			color = 1 - depth / 2;
			CV::color(color, color, color);
			CV::point(x - screenW_2, y - screenH_2);
		}
	}
}

void draw_light(float colors[MAX_SW][MAX_SH], int screenW, int screenH)
{
	float color;
	int screenW_2 = screenW * 0.5;
	int screenH_2 = screenH * 0.5;
	for (int y = 0; y < screenH; y++) {
		for (int x = 0; x < screenW; x++) {
			color = colors[x - screenW_2 + MAX_SW / 2]
					[y - screenH_2 + MAX_SH / 2];
			if (color > 100000000)
				continue;
			CV::color(color * 0.5, color * 0.75, color);
			CV::point(x - screenW_2, y - screenH_2);
		}
	}
}
