#ifndef MISSILE_H
#define MISSILE_H

#include "Vector2.h"
#include "Entity.h"
#include "Circle.h"

class Missile : public Entity {
	protected:
		int damage;
		Vector2 pos_increment;
		Circle c;
		int timer;
	public:
		Missile(Vector2 pos, float radius, int damage, float speed, float angle);
		bool move(Vector2 target, float deltaTime, Entity **entities);
		void render();
};

#endif
