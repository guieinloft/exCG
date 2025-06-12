#include <math.h>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Vector3.h"

Vector3 rotY(Vector3 p, float ang) {
	float sin_ang = sin(ang);
	float cos_ang = cos(ang);
	Vector3 p2( p.x * cos_ang + p.z * sin_ang, p.y,
			-p.x * sin_ang + p.z * cos_ang);
	return p2;
}

Vector3 translate(Vector3 p, float dx, float dy, float dz) {
	Vector3 p2(p.x + dx, p.y + dy, p.z + dz);
	return p2;
}

Vector2 proj(Vector3 p, float d) {
	Vector2 p2(p.x * d / p.z, p.y * d / p.z);
	//Vector2 p2(p.x * d, p.y * d);
	return p2;
}

void cube_draw(Vector2 v[8]) {
	for (int i = 0; i < 4; i++) {
		CV::color(i);
		CV::line(v[i].x, v[i].y, v[(i + 1) & 3].x, v[(i + 1) & 3].y);
		CV::color(i + 4);
		CV::line(v[i + 4].x, v[i + 4].y,
				v[((i + 1) & 3) + 4].x, v[((i + 1) & 3) + 4].y);
		CV::color(i + 8);
		CV::line(v[i].x, v[i].y, v[i + 4].x, v[i + 4].y);
	}
}
