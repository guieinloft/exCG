#include "Tower.h"
#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Poly.h"
#include "Mouse.h"
#include "Keys.h"
#include "Entity.h"

#define TW_COOLDOWN 1000
#define T_SPEED 0.0625f

Tower::Tower(Vector2 center, bool hard) :
	Entity(center, 64, 8 + hard * 4, ETYPE_TOWER) {
	this->c_angle = 0;

	body.createShape(center, 48, 6);

	this->cannon.size = 4;
	this->cannon.v = (Vector2*)malloc(sizeof(Vector2) * this->cannon.size);
	this->cannon.v[0].set(center.x, center.y - 8);
	this->cannon.v[1].set(center.x, center.y + 8);
	this->cannon.v[2].set(center.x + 64, center.y + 8);
	this->cannon.v[3].set(center.x + 64, center.y - 8);

	this->cshape.size = 1;
	this->cshape.poly = &body;

	this->cannon_pos.set(center.x + 64, center.y);

	cooldown = TW_COOLDOWN;
}

void Tower::render(bool show_hbar) {
	CV::color(0.8f, 0.2f, 0.2f);
	body.fill();
	CV::color(0.6f, 0.15f, 0.15f);
	CV::circleFill(center.x, center.y, 24, 16);
	CV::color(1.0f, 0.25f, 0.25f);
	cannon.fill();

	hbar.render((Vector2){center.x, center.y - 48});
}

bool Tower::move(Vector2 target, float deltaTime, Entity **entities) {
	Vector2 delta(target.x - center.x, target.y - center.y);
	float t_angle = atan2(delta.y, delta.x);
	float angle_inc = t_angle - c_angle;
	cannon.rot(angle_inc, center);
	c_angle = t_angle;

	cannon_pos = center + (Vector2){64 * cos(c_angle), 64 * sin(c_angle)};
	cooldown -= deltaTime * (cooldown > 0);

	if (cooldown <= 0) {
		for (int i = ENTITY_FIXED_NUM; i < ENTITY_NUM; i++) {
			if (entities[i] == nullptr) {
				entities[i] = new Missile(cannon_pos, 4.f, 1, 4 * T_SPEED, c_angle);
				cooldown = TW_COOLDOWN;
				break;
			}
		}
	}
	return false;
}

bool Tower::hit() {
	hbar.health--;
	return (hbar.health <= 0);
}

void Tower::setPosition(float x, float y) {
	Vector2 new_center(x, y);
	Vector2 poschange = new_center - center;
	body.move(poschange);
	cannon.move(poschange);
	scircle.c = new_center;
	center = new_center;
}
