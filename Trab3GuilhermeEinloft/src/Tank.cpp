#include "Tank.h"
#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Poly.h"
#include "Mouse.h"
#include "Keys.h"
#include "Entity.h"
#include "Powerup.h"

#define T_SPEED 0.0625f
#define T_SPEED_INC 0.0078125f
#define T_ANGLE_INC 0.0021816616f
#define COOLDOWN 500
#define INVINCIBILITY 500

Tank::Tank(Vector2 center) : Entity(center, 8, ETYPE_TANK) {
	this->angle = 0;
	this->c_angle = 0;

	this->body.size = 4;
	this->body.v = (Vector2*)malloc(sizeof(Vector2) * this->body.size);
	this->body.v[0].set(center.x - 32, center.y - 24);
	this->body.v[1].set(center.x - 32, center.y + 24);
	this->body.v[2].set(center.x + 32, center.y + 24);
	this->body.v[3].set(center.x + 32, center.y - 24);

	this->cannon.size = 4;
	this->cannon.v = (Vector2*)malloc(sizeof(Vector2) * this->cannon.size);
	this->cannon.v[0].set(center.x, center.y - 8);
	this->cannon.v[1].set(center.x, center.y + 8);
	this->cannon.v[2].set(center.x + 48, center.y + 8);
	this->cannon.v[3].set(center.x + 48, center.y - 8);

	this->cannon_pos.set(center.x + 48, center.y);

	this->cshape.size = 1;
	this->cshape.poly = &body;

	cooldown = 0;
	
	invincibility = INVINCIBILITY;

	super = false;
	sbar.health = 0;
	sbar.max_health = 4;
}

void Tank::checkKeys(Keys keys) {
	angle_inc = 0;
	if (keys.k_left)
		angle_inc -= T_ANGLE_INC;
	if (keys.k_right)
		angle_inc += T_ANGLE_INC;
}

void Tank::checkMouse(Mouse mouse, Entity *entities[]) {
	m_pos.set(mouse.x, mouse.y);

	if (mouse.state == 0 && cooldown <= 0) {
		for (int i = ENTITY_FIXED_NUM; i < ENTITY_NUM; i++) {
			if (entities[i] == NULL) {
				entities[i] = new Missile(cannon_pos, 4.f,
					1, 4 * T_SPEED, c_angle);
				cooldown = (COOLDOWN >> super);
				break;
			}
		}
	}
}

void Tank::render() {
	CV::color(1.f, 1.f, 1.f);
	if (invincibility <= 0)
		if (super)
			CV::color(0.5f, 0.5f, 0.2f);
		else
			CV::color(0.2f, 0.5f, 0.2f);
	body.fill();
	if (invincibility <= 0)
		if (super)
			CV::color(0.25f, 0.25f, 0.1f);
		else
			CV::color(0.1f, 0.25f, 0.1f);
	CV::circleFill(center.x, center.y, 20, 16);
	if (invincibility <= 0)
		if (super)
			CV::color(0.75f, 0.75f, 0.3f);
		else
			CV::color(0.3f, 0.75f, 0.3f);
	cannon.fill();

	CV::color(0.3f, 0.3f, 0.75f);
	for (int i = 0; i < sbar.health; i++)
		CV::circle(center.x, center.y, 48 + i, 16);

	hbar.render((Vector2){136, 24}, false, 32, 0.3f, 0.75f, 0.3f);
	sbar.render((Vector2){328, 24}, false, 32, 0.3f, 0.3f, 0.75f);
}

bool Tank::move(Vector2 target, float deltaTime, Entity **entities) {
	float m_angle = atan2(m_pos.y - center.y, m_pos.x - center.x);
	cannon.rot(m_angle - c_angle, center);
	c_angle = m_angle;

	speed += T_SPEED_INC * (speed < T_SPEED) - T_SPEED_INC * (speed > T_SPEED);
	float rspeed = speed * deltaTime;
	Vector2 poschange(cos(angle) * rspeed, sin(angle) * rspeed);
	center += poschange;
	body.move(poschange);
	cannon.move(poschange);
	cannon_pos = center + (Vector2){48 * cos(c_angle), 48 * sin(c_angle)};
	body.rot(angle_inc * deltaTime, center);
	angle += angle_inc * deltaTime;
	cooldown -= deltaTime * (cooldown > 0);
	invincibility -= deltaTime * (invincibility > 0);
	return false;
}

bool Tank::hit() {
	speed = 0;
	if (invincibility <= 0) {
		if (sbar.health > 0)
			sbar.health--;
		else
			hbar.health--;
		invincibility = INVINCIBILITY;
	}
	return (hbar.health <= 0);
}

bool Tank::hit_border(int dir) {
	speed = -2 * dir * T_SPEED;
	if (invincibility <= 0) {
		if (sbar.health > 0)
			sbar.health--;
		else
			hbar.health--;
		invincibility = INVINCIBILITY;
	}
	return (hbar.health <= 0);
}

void Tank::hit_powerup(PowerupType type) {
	switch(type) {
	case PWUP_SUPER:
		super = true;
		break;
	case PWUP_SHIELD:
		sbar.health = 4;
		break;
	case PWUP_HEALTH:
		hbar.health += 3;
		if (hbar.health > hbar.max_health)
			hbar.health = hbar.max_health;
		break;
	}
}
