#ifndef TANK__H
#define TANK__H

#include "Vector2.h"
#include "Poly.h"
#include "Mouse.h"
#include "Keys.h"
#include "Missile.h"
#include "Entity.h"
#include "Powerup.h"

class Tank : public Entity {
protected:
	Poly body;
	Poly cannon;
	float speed;
	float angle;
	float angle_inc;
	float c_angle;
	Vector2 m_pos;
	Vector2 cannon_pos;
	int cooldown;
	int invincibility;
	bool knockback;
	int knockback_dir;

	bool super;
	HealthBar sbar;

public:
	Tank(Vector2 center);
	void checkKeys(Keys keys);
	void checkMouse(Mouse mouse, Entity *entities[]);
	void render(bool show_hbar = true);
	bool move(Vector2 target, float deltaTime, Entity **entities);
	bool hit();
	bool hit_border(int dir);
	void hit_powerup(EntityType type);
	void setPosition(float x, float y);
};

#endif
