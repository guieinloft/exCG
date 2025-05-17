#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Missile.h"

#define M_TIMER 2000

Missile::Missile(Vector2 pos, float radius, int damage, float speed,
		float angle) : Entity(pos, damage, ETYPE_MISSILE) {
	c.c = pos;
	c.r = radius;
	this->damage = damage;
	pos_increment = (Vector2){speed * cos(angle), speed * sin(angle)};
	
	cshape.size = 1;
	cshape.poly = new Poly();
	cshape.poly[0].createShape(pos, radius, 8);
	timer = M_TIMER;
}

bool Missile::move(Vector2 target, float deltaTime, Entity **entities) {
	c.c += (pos_increment * deltaTime);
	cshape.poly[0].move(pos_increment * deltaTime);
	timer -= deltaTime;
	return (timer <= 0);
}

void Missile::render() {
	CV::color(1, 1, 0.8);
	CV::circleFill(c.c.x, c.c.y, c.r, 8);
}
