#ifndef POWERUP_H
#define POWERUP_H

#include "Vector2.h"
#include "Circle.h"
#include "Entity.h"

class Powerup : public Entity {
	public:
	Powerup(Vector2 center, EntityType type);
	void render(bool show_hbar = true);
	bool hit();
	bool move(Vector2 target, float deltaTime, Entity **entities);
	void setPosition(float x, float y);
};

#endif
