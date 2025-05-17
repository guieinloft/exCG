#ifndef CHASER_H
#define CHASER_H

#include "Vector2.h"
#include "Entity.h"
#include "Circle.h"

class Chaser : public Entity {
	protected:
		float speed;
		float angle;
		Vector2 birth;
	public:
		Chaser(Vector2 pos, bool hard);
		bool move(Vector2 target, float deltaTime, Entity **entities);
		void render();
		bool hit();
};

#endif
