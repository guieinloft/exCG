#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Chaser.h"

#define C_SPEED 0.25f
#define C_SPEED_INC 0.015625f
#define C_ANGLE_INC 0.0245436926
#define C_TGT_DISTANCE 131072
#define C_BIRTH_DISTANCE 512

Chaser::Chaser(Vector2 pos, bool hard) :
	Entity(pos, 4 + hard * 2, ETYPE_CHASER) {
	cshape.size = 1;
	cshape.poly = new Poly();
	cshape.poly[0].createShape(pos, 32, 3);

	birth = pos;

	speed = 0.f;
	angle = 0.f;
}

bool Chaser::move(Vector2 target, float deltaTime, Entity **entities) {
	if (hbar.health == hbar.max_health)
		return false;
	Vector2 delta(target.x - center.x, target.y - center.y);
	float t_angle;
	t_angle = atan2(delta.y, delta.x);
	float angle_inc = t_angle - angle;
	cshape.poly[0].rot(angle_inc, center);
	angle = t_angle;

	speed += C_SPEED_INC * (speed < C_SPEED);

	Vector2 pos_change(cos(angle) * speed, sin(angle) * speed);
	center += (pos_change * deltaTime);
	cshape.poly[0].move(pos_change * deltaTime);

	return false;
}

void Chaser::render() {
	CV::color(0.9, 0.8, 0);
	cshape.poly[0].fill();
	CV::color(0.2, 1, 1);
	CV::circleFill(center.x, center.y, 12, 16);
	hbar.render((Vector2){center.x, center.y - 32});
}

bool Chaser::hit() {
	speed = -C_SPEED;
	hbar.health--;
	return (hbar.health <= 0);
}
