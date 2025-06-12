#include <stdlib.h>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include "bezier.h"

inline int sign(float a)
{
	return (a > 0) - (a < 0);
}

void plot_curve(Vector2 points[MAX_RES], int size)
{
	CV::color(1, 0, 0);
	for (int i = 0; i < size - 1; i++) {
		CV::line(points[i].x, points[i].y,
				points[i + 1].x, points[i + 1].y);
	}
}

void plot_points(Vector2 points[MAX_RES], int size)
{
	for (int i = 0; i < size - 1; i++) {
		if (i == 0)
			CV::color(0, 0.75, 0.75);
		CV::rectFill(points[i].x - 4, points[i].y - 4,
				points[i].x + 4, points[i].y + 4);
		CV::color(0, 0.75, 0);
		CV::line(points[i].x, points[i].y,
				points[i + 1].x, points[i + 1].y);
	}
	CV::color(0.75, 0.75, 0);
	CV::rectFill(points[size - 1].x - 4, points[size - 1].y - 4,
			points[size - 1].x + 4, points[size - 1].y + 4);
}

void change_point(Vector2 *point, float x, float y)
{
	point->set(x, y);
}

Vector2 evaluate_point(Vector2 points[MAX_RES], int pnum, float t)
{
    Vector2 *tmp = (Vector2*)malloc(sizeof(Vector2) * pnum);
    memcpy(tmp, points, pnum * sizeof(Vector2));
    for (int i = pnum - 1; i > 0; i--) {
        for (int k = 0; k < i; k++)
            tmp[k] = tmp[k] + (tmp[k+1] - tmp[k]) * t;
    }
    Vector2 res = tmp[0];
    free(tmp);
    return res;
}

void evaluate_curve(Vector2 points[MAX_RES], int pnum,
		Vector2 bezier[MAX_RES], int bnum)
{
	bnum--;
	for (int i = 0; i <= bnum; i++) {
		float t = (float)i / bnum;
		bezier[i] = evaluate_point(points, pnum, t);
	}
}

void save_curve(Vector2 points[MAX_RES], int pnum)
{
	FILE *file = fopen("Trab4GuilhermeEinloft/curva.txt", "w");
	if (file == NULL) return;
	fprintf(file, "%d\n", pnum);
	for (int i = 0; i < pnum; i++) {
		fprintf(file, "%f, %f\n", points[i].x, points[i].y);
	}
	fclose(file);
}

void load_curve(Vector2 points[MAX_RES], int *pnum)
{
	FILE *file = fopen("Trab4GuilhermeEinloft/curva.txt", "r");
	if (file == NULL) return;
	fscanf(file, "%d", pnum);
	for (int i = 0; i < *pnum; i++) {
		fscanf(file, "%f, %f", &points[i].x, &points[i].y);
	}
	fclose(file);
}
