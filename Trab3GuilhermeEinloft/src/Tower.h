#ifndef TOWER__H
#define TOWER__H

#include "Vector2.h"
#include "Poly.h"
#include "Mouse.h"
#include "Keys.h"
#include "Missile.h"
#include "Entity.h"

class Tower : public Entity {
protected:
	//base da torre
	Poly body;
	//canhao
	Poly cannon;
	//angulo do canhao
	float c_angle;
	//posicao de spawn dos misseis
	Vector2 cannon_pos;
	//tempo entre misseis
	int cooldown;

public:
	Tower(Vector2 center, bool hard);
	void render(bool show_hbar = true);
	bool move(Vector2 target, float deltaTime, Entity **entities);
	bool hit();
	void setPosition(float x, float y);
};

#endif
