#ifndef POLY__H
#define POLY__H

#include "Vector2.h"
#include "gl_canvas2d.h"

class Poly {
public:
	Vector2 *v;
	int size;
	void fill();
	void rot(double angle, Vector2 pivot);
	void move(Vector2 dir);
	void createShape(Vector2 center, float radius, int size);
};


#endif
