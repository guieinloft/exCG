#ifndef POWERUP_H
#define POWERUP_H

#include "Vector2.h"
#include "Circle.h"
#include "Entity.h"

enum PowerupType {
	PWUP_SUPER,
	PWUP_SHIELD,
	PWUP_HEALTH
};

class Powerup : public Entity {
	protected:
	PowerupType type;

	public:
	Powerup(Vector2 center, PowerupType type);
	void render();
	PowerupType getPowerupType();
	bool hit();
	bool move(Vector2 target, float deltaTime, Entity **entities);
};

#endif
