#ifndef B_SPLINE__H
#define B_SPLINE__H

#include "gl_canvas2d.h"
#include "Vector2.h"

Vector2 b_spline_points[4] = {
	(Vector2){200, 400},
	(Vector2){300, 600},
	(Vector2){400, 200},
	(Vector2){500, 400}
};

Vector2 b_spline_cpoints[101];

Vector2 function_points[4][101];

void calculate_functions() {
	for (int i = 0; i <= 100; i++) {
		float t = i * 0.01f;
		function_points[0][i].set(t * 100, (pow(1 - t, 3)) / 6 * 100);
		function_points[1][i].set(t * 100, (3 * pow(t, 3) - 6 * t * t + 4) / 6 * 100);
		function_points[2][i].set(t * 100, (-3 * pow(t, 3) + 3 * t * t + 3 * t + 1) / 6 * 100);
		function_points[3][i].set(t * 100, (t * t * t) / 6 * 100);
	}
}

void plot_functions() {
	CV::color(0, 0, 0);
	for (int i = 0; i < 100; i++) {
		CV::line(function_points[0][i].x, function_points[0][i].y,
			function_points[0][i+1].x, function_points[0][i+1].y);
		CV::line(function_points[1][i].x, function_points[1][i].y,
			function_points[1][i+1].x, function_points[1][i+1].y);
		CV::line(function_points[2][i].x, function_points[2][i].y,
			function_points[2][i+1].x, function_points[2][i+1].y);
		CV::line(function_points[3][i].x, function_points[3][i].y,
			function_points[3][i+1].x, function_points[3][i+1].y);
	}
	CV::color(1, 1, 1);
}

void plot_curve() {
	CV::color(0, 0, 0);
	for (int i = 0; i < 100; i++) {
		CV::line(b_spline_cpoints[i].x, b_spline_cpoints[i].y,
			b_spline_cpoints[i+1].x, b_spline_cpoints[i+1].y);
	}
}

void plot_points() {
	CV::color(1, 0, 0);
	for (int i = 0; i < 3; i++) {
		CV::line(b_spline_points[i].x, b_spline_points[i].y,
			b_spline_points[i+1].x, b_spline_points[i+1].y);
	}
}

void change_point(int point, float x, float y) {
	b_spline_points[point].set(x, y);
}

void calculate_points() {
	for (int i = 0; i <= 100; i++) {
		float t = i * 0.01f;
		Vector2 p = (Vector2){0, 0};
		p += b_spline_points[0] * (pow(1 - t, 3) / 6); 
		p += b_spline_points[1] * ((3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6); 
		p += b_spline_points[2] * ((-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6); 
		p += b_spline_points[3] * (t * t * t / 6); 
		b_spline_cpoints[i] = p;
	}
}

#endif
