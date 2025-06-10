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

float tri_area(Vector3 a, Vector3 b, Vector3 c)
{
	Vector3 ac = c - a;
	Vector3 ab = b - a;
	Vector2 ac2(ac.x, ac.y);
	Vector2 ab2(ab.x, ab.y);
	ab2.perp();
	return ab2.dot(ac2) * 0.5;
}

float sign (Vector3 p1, Vector3 p2, Vector3 p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
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

	return (w1 > 0 && w2 > 0 && w3 > 0);
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
	if (a.x != 0) {
		p2.set(p.x, p.y * cos_ax - p.z * sin_ax,
				p.y * sin_ax + p.z * cos_ax);
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

void generate_normals(Vector3 in[MAX_RES][MAX_RES],
		Vector3 normals[MAX_RES][MAX_RES][2],
		int m, int n)
{
	Vector3 edge1, edge2;
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			edge1 = in[i][j] - in[i + 1][j];
			edge2 = in[i + 1][j + 1] - in[i + 1][j];
			normals[i][j][0] = edge1.cross(edge2);
			normals[i][j][0].normalize();
			edge1 = in[i][j] - in[i][j + 1];
			edge2 = in[i + 1][j + 1] - in[i][j + 1];
			normals[i][j][1] = edge2.cross(edge1);
			normals[i][j][1].normalize();
		}
	}
}

void project_normals(Vector3 out[MAX_RES][MAX_RES],
		Vector3 normals[MAX_RES][MAX_RES][2],
		Vector3 normals_proj[MAX_RES][MAX_RES][2],
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
				normals_proj[i][j][0] = proj_perp(
						midpoint1 + normals[i][j][0] * 0.1,
						d);
				normals_proj[i][j][1] = proj_perp(
						midpoint2 + normals[i][j][1] * 0.1,
						d);
			}
			else {
				normals_proj[i][j][0] = proj_ortho(
						midpoint1 + normals[i][j][0] * 0.1,
						d);
				normals_proj[i][j][1] = proj_ortho(
						midpoint2 + normals[i][j][1] * 0.1,
						d);
			}
		}
	}
}

void draw_normals_culled(Vector3 out_proj[MAX_RES][MAX_RES],
		Vector3 normals_proj[MAX_RES][MAX_RES][2], int m, int n)
{
	Vector3 midpoint;
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (!is_ccw(out_proj[i][j], out_proj[i + 1][j],
					out_proj[i + 1][j + 1])) {
				midpoint = (out_proj[i][j] +
						out_proj[i + 1][j] +
						out_proj[i + 1][j + 1]) *
						(1.0/3);
				CV::line(midpoint.x, midpoint.y,
						normals_proj[i][j][0].x,
						normals_proj[i][j][0].y);
			}
			if (!is_ccw(out_proj[i][j], out_proj[i + 1][j],
					out_proj[i + 1][j + 1])) {
				midpoint = (out_proj[i][j] +
						out_proj[i][j + 1] +
						out_proj[i + 1][j + 1]) *
						(1.0/3);
				CV::line(midpoint.x, midpoint.y,
						normals_proj[i][j][1].x,
						normals_proj[i][j][1].y);
			}
		}
	}
}

void draw_normals(Vector3 out_proj[MAX_RES][MAX_RES],
		Vector3 normals_proj[MAX_RES][MAX_RES][2], int m, int n)
{
	Vector3 midpoint;
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			midpoint = (out_proj[i][j] + out_proj[i + 1][j] +
					out_proj[i + 1][j + 1]) * (1.0/3);
			CV::line(midpoint.x, midpoint.y,
					normals_proj[i][j][0].x,
					normals_proj[i][j][0].y);
			midpoint = (out_proj[i][j] + out_proj[i][j + 1] +
					out_proj[i + 1][j + 1]) * (1.0/3);
			CV::line(midpoint.x, midpoint.y,
					normals_proj[i][j][1].x,
					normals_proj[i][j][1].y);
		}
	}
}

Vector2 find_min_point(Vector3 a, Vector3 b, Vector3 c)
{
	Vector2 point(fmin(a.x, fmin(b.x, c.x)), fmin(a.y, fmin(b.y, c.y)));
	return point;
}

Vector2 find_max_point(Vector3 a, Vector3 b, Vector3 c)
{
	Vector2 point(fmax(a.x, fmax(b.x, c.x)), fmax(a.y, fmax(b.y, c.y)));
	return point;
}

void update_tri_z_buffer(Vector3 a, Vector3 b, Vector3 c,
		float z_depth[MAX_SW][MAX_SH])
{
	Vector3 weights;
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
				float depth = depths.dot(weights);
				if (depth < z_depth[x_adj][y_adj])
					z_depth[x_adj][y_adj] = depth;
			}
		}
	}
}

void update_z_buffer(Vector3 v[MAX_RES][MAX_RES], int m, int n,
		float z_depth[MAX_SW][MAX_SH])
{
	memset(z_depth, 0x7f, sizeof(float) * MAX_SW * MAX_SH);
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			update_tri_z_buffer(v[i][j], v[i + 1][j],
					v[i + 1][j + 1], z_depth);
			update_tri_z_buffer(v[i][j], v[i + 1][j + 1],
					v[i][j + 1], z_depth);
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
