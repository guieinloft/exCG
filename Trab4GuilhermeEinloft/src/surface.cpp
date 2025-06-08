#include <math.h>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Vector3.h"
#include "bezier.h"

bool ccw(Vector2 v0, Vector2 v1, Vector2 v2)
{
	float a = (v1.x - v0.x) * (v2.y - v0.y);
	float b = (v2.x - v0.x) * (v1.y - v0.y);
	return (a >= b);
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

Vector2 proj_perp(Vector3 p, float d)
{
	Vector2 p2(p.x * d / p.z, p.y * d / p.z);
	return p2;
}

Vector2 proj_ortho(Vector3 p, float d)
{
	Vector2 p2(p.x * d, p.y * d);
	return p2;
}

void draw_surface(Vector2 v[MAX_RES][MAX_RES], int m, int n)
{
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			CV::line(v[i][j], v[i + 1][j]);
			CV::line(v[i + 1][j], v[i + 1][j + 1]);
			CV::line(v[i][j], v[i + 1][j + 1]);
			CV::line(v[i][j], v[i][j + 1]);
			CV::line(v[i][j + 1], v[i + 1][j + 1]);
		}
	}
}

void draw_surface_culled(Vector2 v[MAX_RES][MAX_RES], int m, int n)
{
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (ccw(v[i][j], v[i + 1][j], v[i + 1][j + 1])) {
				CV::line(v[i][j], v[i + 1][j]);
				CV::line(v[i + 1][j], v[i + 1][j + 1]);
				CV::line(v[i][j], v[i + 1][j + 1]);
			}
			if (ccw(v[i][j], v[i + 1][j + 1], v[i][j + 1])) {
				CV::line(v[i][j], v[i][j + 1]);
				CV::line(v[i][j + 1], v[i + 1][j + 1]);
				CV::line(v[i][j], v[i + 1][j + 1]);
			}
		}
	}
}

void generate_surface(Vector3 out[MAX_RES][MAX_RES],
		Vector2 in[MAX_RES], int m, int n, float d)
{
	float step = PI_2 / (m - 1);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			out[i][j].set(in[j].x / d * cos(step * i),
					in[j].y / d,
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
		Vector2 out[MAX_RES][MAX_RES],
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
		Vector2 normals_proj[MAX_RES][MAX_RES][2],
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

void draw_normals(Vector2 out_proj[MAX_RES][MAX_RES],
		Vector2 normals_proj[MAX_RES][MAX_RES][2], int m, int n)
{
	Vector2 midpoint;
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			midpoint = (out_proj[i][j] + out_proj[i + 1][j] +
					out_proj[i + 1][j + 1]) * (1.0/3);
			CV::line(midpoint, normals_proj[i][j][0]);
			midpoint = (out_proj[i][j] + out_proj[i][j + 1] +
					out_proj[i + 1][j + 1]) * (1.0/3);
			CV::line(midpoint, normals_proj[i][j][1]);
		}
	}
}
