#ifndef BARREL_H
#define BARREL_H

#include "Vector2.h"
#include "Circle.h"
#include "Entity.h"

class Barrel : public Entity {
	public:
	Barrel(Vector2 center, float radius);
	void render(bool show_hbar = true);
	Circle getCollisionCircle();
	void setPosition(float x, float y);
};

#endif
