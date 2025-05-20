#ifndef CHASER_H
#define CHASER_H

#include "Vector2.h"
#include "Entity.h"
#include "Circle.h"

class Chaser : public Entity {
	protected:
		//velocidade
		float speed;
		//angulo
		float angle;
		//define se eh arremessado pra tras pos hit
		bool knockback;
	public:
		Chaser(Vector2 pos, bool hard);
		bool move(Vector2 target, float deltaTime, Entity **entities);
		void render(bool show_hbar = true);
		bool hit();
		void setPosition(float x, float y);
};

#endif
