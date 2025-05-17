#ifndef BARREL_H
#define BARREL_H

#include "Vector2.h"
#include "Circle.h"
#include "Entity.h"

class Barrel : public Entity {
	public:
	Circle c;

	//public:
	Barrel(Vector2 center, float radius);
	void render();
	Circle getCollisionCircle();
};

#endif
