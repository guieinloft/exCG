#ifndef BEZIER__H
#define BEZIER__H

#include "gl_canvas2d.h"
#include "Vector2.h"

Vector2 bezier_points[4] = {
	(Vector2){200, 400},
	(Vector2){300, 600},
	(Vector2){400, 200},
	(Vector2){500, 400}
};

Vector2 bezier_cpoints[101];

Vector2 function_points[4][101];

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

void calculate_functions() {
	for (int i = 0; i <= 100; i++) {
		float t = i * 0.01f;
		function_points[0][i].set(t * 100, (pow(1 - t, 3)) * 100);
		function_points[1][i].set(t * 100, (3 * t * pow(1 - t, 2)) * 100);
		function_points[2][i].set(t * 100, (3 * t * t * (1 - t)) * 100);
		function_points[3][i].set(t * 100, (t * t * t) * 100);
	}
}

void plot_curve() {
	CV::color(0, 0, 0);
	for (int i = 0; i < 100; i++) {
		CV::line(bezier_cpoints[i].x, bezier_cpoints[i].y,
			bezier_cpoints[i+1].x, bezier_cpoints[i+1].y);
	}
}

void plot_points() {
	CV::color(1, 0, 0);
	for (int i = 0; i < 3; i++) {
		CV::line(bezier_points[i].x, bezier_points[i].y,
			bezier_points[i+1].x, bezier_points[i+1].y);
	}
}

void change_point(int point, float x, float y) {
	bezier_points[point].set(x, y);
}

void calculate_points() {
	for (int i = 0; i <= 100; i++) {
		float t = i * 0.01f;
		Vector2 p = (Vector2){0, 0};
		p += bezier_points[0] * pow(1 - t, 3); 
		p += bezier_points[1] * 3 * t * pow(1 - t, 2); 
		p += bezier_points[2] * 3 * t * t * (1 - t); 
		p += bezier_points[3] * t * t * t;
		bezier_cpoints[i] = p;
	}
}

#endif
