#ifndef ENTITY_H
#define ENTITY_H

#include "CollisionShape.h"
#include "HealthBar.h"

#define ENTITY_NUM 64
#define ENTITY_FIXED_NUM 32

enum EntityType {
	ETYPE_TANK,
	ETYPE_BARREL,
	ETYPE_MISSILE,
	ETYPE_CHASER,
	ETYPE_TOWER,
	ETYPE_POWERUP = 8,
	ETYPE_POWERUP_SUPER,
	ETYPE_POWERUP_SHIELD,
	ETYPE_POWERUP_HEALTH,
};

class Entity {
protected:
	Vector2 center;
	CollisionShape cshape;
	HealthBar hbar;
	bool active;
	EntityType type;

public:
	Entity(Vector2 center, int max_health, EntityType type);
	virtual bool hit();
	virtual bool move(Vector2 target, float deltaTime, Entity **entities);
	virtual void render();
	CollisionShape getCollisionShape();
	Vector2 getCenter();
	bool isActive(); 
	EntityType getType();
	int getMaxHealth();
	bool isPowerup();
};

#endif
