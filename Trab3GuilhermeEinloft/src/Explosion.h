#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Vector2.h"
#include "Entity.h"

class Explosion : public Entity {
	protected:
		int timer;
	public:
		Explosion(Vector2 pos, float radius, int damage);
		bool move(Vector2 target, float deltaTime, Entity **entities);
		void render(bool show_hbar = true);
		bool hit();
};

#endif
