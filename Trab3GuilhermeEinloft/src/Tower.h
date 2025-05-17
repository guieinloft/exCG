#ifndef TOWER__H
#define TOWER__H

#include "Vector2.h"
#include "Poly.h"
#include "Mouse.h"
#include "Keys.h"
#include "Missile.h"
#include "Entity.h"

class Tower : public Entity {
protected:
	Poly body;
	Poly cannon;
	float c_angle;
	Vector2 cannon_pos;
	int cooldown;

public:
	Tower(Vector2 center, bool hard);
	void render();
	bool move(Vector2 target, float deltaTime, Entity **entities);
	bool hit();
};

#endif
