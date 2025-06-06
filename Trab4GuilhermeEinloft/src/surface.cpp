#include <math.h>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Vector3.h"
#include "bezier.h"

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
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n - 1; j++) {
			CV::line(v[i][j], v[i][j + 1]);
			CV::line(v[i][j], v[(i + 1) % m][j]);
			CV::line(v[i][j], v[(i + 1) % m][j + 1]);
			CV::line(v[i][j + 1], v[(i + 1) % m][j + 1]);
		}
	}
}

void generate_surface(Vector3 out[MAX_RES][MAX_RES],
		Vector2 in[MAX_RES], int m, int n, float d)
{
	float step = PI_2 / m;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			out[i][j].set(in[j].x / d * cos(step * i),
					in[j].y / d,
					in[j].x / d * sin(step * i));
		}
	}
}
