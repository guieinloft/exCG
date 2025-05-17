#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Barrel.h"
#include "Circle.h"

Barrel::Barrel(Vector2 center, float radius) : Entity(center, 1, ETYPE_BARREL) {
	c.c = center;
	c.r = radius;
	cshape.size = 1;
	cshape.poly = new Poly();
	cshape.poly[0].createShape(c.c, c.r, 8);
}

void Barrel::render() {
	CV::color(0.7f, 0.5f, 0.2f);
	CV::circleFill(c.c.x, c.c.y, c.r, 16);
	CV::color(0.5f, 0.3f, 0.1f);
	CV::circleFill(c.c.x, c.c.y, c.r * 0.75f, 16);
}

Circle Barrel::getCollisionCircle() {
	return c;
}
