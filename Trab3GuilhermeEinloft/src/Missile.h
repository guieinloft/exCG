#ifndef MISSILE_H
#define MISSILE_H

#include "Vector2.h"
#include "Entity.h"
#include "Circle.h"

class Missile : public Entity {
	protected:
		//dano do missil
		int damage;
		//incremento de posicao a cada move
		Vector2 pos_increment;
		//tempo de vida do missil
		int timer;
	public:
		Missile(Vector2 pos, float radius, int damage, float speed, float angle);
		bool move(Vector2 target, float deltaTime, Entity **entities);
		void render(bool show_hbar = true);
};

#endif
