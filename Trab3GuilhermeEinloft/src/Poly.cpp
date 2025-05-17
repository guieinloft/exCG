#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Poly.h"

void Poly::fill() {
	float *vx = (float*)malloc(sizeof(float) * size);
	float *vy = (float*)malloc(sizeof(float) * size);
	for (int i = 0; i < size; i++) {
		vx[i] = v[i].x;
		vy[i] = v[i].y;
	}
	CV::polygonFill(vx, vy, size);
}

void Poly::rot(double angle, Vector2 vot) {
	Vector2 temp;
	Vector2 temp2;
	double cos_angle = cos(angle);
	double sin_angle = sin(angle);
	for (int i = 0; i < size; i++) {
		temp = v[i] - vot;
		temp2.x = temp.x * cos_angle - temp.y * sin_angle;
		temp2.y = temp.x * sin_angle + temp.y * cos_angle;
		v[i] = temp2 + vot;
	}
}

void Poly::move(Vector2 dir) {
	for (int i = 0; i < size; i++)
		v[i] += dir;
}

void Poly::createShape(Vector2 center, float radius, int size) {
	this->size = size;
	this->v = (Vector2*)malloc(sizeof(Vector2) * size);
	for (int i = 0; i < size; i++) {
		v[i].x = center.x + radius * cos(PI_2*i/size);
		v[i].y = center.y + radius * sin(PI_2*i/size);
	}
}
