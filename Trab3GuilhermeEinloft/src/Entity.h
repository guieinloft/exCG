#ifndef ENTITY_H
#define ENTITY_H

#include "CollisionShape.h"
#include "Circle.h"
#include "HealthBar.h"

#define ENTITY_NUM 64
#define ENTITY_FIXED_NUM 32

enum EntityType {
	ETYPE_BARREL,
	ETYPE_CHASER,
	ETYPE_TOWER,
	ETYPE_POWERUP_SHIELD,
	ETYPE_POWERUP_HEALTH,
	ETYPE_POWERUP_SUPER,
	ETYPE_FIXED_NUM,
	ETYPE_MISSILE,
	ETYPE_EXPLOSION,
	ETYPE_TANK = -1
};

class Entity {
protected:
	Vector2 center;
	Circle scircle;
	CollisionShape cshape;
	HealthBar hbar;
	bool active;
	EntityType type;

public:
	Entity(Vector2 center, float radius, int max_health, EntityType type);
	virtual ~Entity();
	virtual bool hit();
	virtual bool move(Vector2 target, float deltaTime, Entity **entities);
	virtual void render(bool show_hbar = true);
	virtual void setPosition(float x, float y);
	CollisionShape getCollisionShape();
	Vector2 getCenter();
	Circle getSpawnCircle();
	bool isActive(); 
	EntityType getType();
	int getMaxHealth();
	bool isPowerup();
	virtual void renderSpawnCircle(bool colliding);
};

void remove_entity(Entity **entities, int i, int *score, int *entity_num);

#endif
