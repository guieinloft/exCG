#include "Vector2.h"
#include "gl_canvas2d.h"
#include "b_spline.h"

#include <math.h>

Vector2 b_spline_point(Vector2 points[], int n, float t, int i) {
	Vector2 res(0, 0);
	res += points[(i + 0) % n] * (pow(1 - t, 3) / 6); 
	res += points[(i + 1) % n] * ((3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6); 
	res += points[(i + 2) % n] * ((-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6); 
	res += points[(i + 3) % n] * (t * t * t / 6); 
	return res;
}

void b_spline_generate(Vector2 points[], int n_points, Vector2 b_spline[B_SEG]) {
	for (int i = 0; i < B_SEG; i++) {
		float jt = (float)i * n_points / B_SEG;
		int j = (int)jt;
		float t = fmod(jt, 1.f);
		b_spline[i] = b_spline_point(points, n_points, t, j);
	}
}

void b_spline_draw(Vector2 b_out[B_SEG], Vector2 b_in[B_SEG]) {
	for (int i = 0; i < B_SEG; i++) {
		CV::color(0, 0, 0);
		float t1x[3] = {b_out[i].x, b_out[(i + 1) & B_MASK].x, b_in[i].x};
		float t1y[3] = {b_out[i].y, b_out[(i + 1) & B_MASK].y, b_in[i].y};
		float t2x[3] = {b_in[i].x, b_in[(i + 1) & B_MASK].x, b_out[(i + 1) & B_MASK].x};
		float t2y[3] = {b_in[i].y, b_in[(i + 1) & B_MASK].y, b_out[(i + 1) & B_MASK].y};
		CV::polygonFill(t1x, t1y, 3);
		CV::polygonFill(t2x, t2y, 3);
		CV::color(1, 1, 0);
		if (i & 1) {
			CV::line((b_out[i].x + b_in[i].x) * 0.5f,
				(b_out[i].y + b_in[i].y) * 0.5f, 
				(b_out[(i + 1) & B_MASK].x + b_in[(i + 1) & B_MASK].x) * 0.5f,
				(b_out[(i + 1) & B_MASK].y + b_in[(i + 1) & B_MASK].y) * 0.5f);
		}
	}
}

void b_spline_points_draw(Vector2 p_out[12], Vector2 p_in[12]) {
	CV::color(0, 0, 1);
	for (int i = 0; i < 12; i++) {
		CV::line(p_out[i].x, p_out[i].y,
			p_out[(i + 1) % 12].x, p_out[(i + 1) % 12].y);
		CV::line(p_in[i].x, p_in[i].y,
			p_in[(i + 1) % 12].x, p_in[(i + 1) % 12].y);
		CV::line(p_in[i].x, p_in[i].y, p_out[i].x, p_out[i].y);
	}
}
