#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Barrel.h"
#include "Circle.h"

Barrel::Barrel(Vector2 center, float radius) : Entity(center, radius, 1, ETYPE_BARREL) {
	cshape.size = 1;
	cshape.poly = new Poly();
	cshape.poly[0].createShape(scircle.c, scircle.r, 8);
}

void Barrel::render(bool show_hbar) {
	CV::color(0.7f, 0.5f, 0.2f);
	CV::circleFill(scircle.c.x, scircle.c.y,
		scircle.r, 16);
	CV::color(0.5f, 0.3f, 0.1f);
	CV::circleFill(scircle.c.x, scircle.c.y,
		scircle.r * 0.75f, 16);
}

void Barrel::setPosition(float x, float y) {
	Vector2 new_center(x, y);
	cshape.poly[0].move(new_center - center);
	center.set(x, y);
	scircle.c.set(x, y);
}
