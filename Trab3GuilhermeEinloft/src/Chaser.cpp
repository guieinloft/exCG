#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Chaser.h"

#define C_SPEED 0.25f
#define C_SPEED_INC 0.00390625f
#define C_ANGLE_INC 0.0245436926
#define C_TGT_DISTANCE 131072
#define C_BIRTH_DISTANCE 512

Chaser::Chaser(Vector2 pos, bool hard) :
	Entity(pos, 40, 4 + hard * 2, ETYPE_CHASER) {
	cshape.size = 1;
	cshape.poly = new Poly();
	cshape.poly[0].createShape(pos, 32, 3);

	birth = pos;

	speed = 0.f;
	angle = 0.f;
	knockback = false;
}

bool Chaser::move(Vector2 target, float deltaTime, Entity **entities) {
	if (hbar.health == hbar.max_health)
		return false;
	Vector2 delta(target.x - scircle.c.x, target.y - scircle.c.y);
	float t_angle;
	t_angle = atan2(delta.y, delta.x);
	float angle_inc = t_angle - angle;
	cshape.poly[0].rot(angle_inc, scircle.c);
	angle = t_angle;

	if (knockback)
		speed = -C_SPEED * deltaTime;
	else
		speed += C_SPEED_INC * deltaTime * (speed < C_SPEED * deltaTime);
	knockback = false;

	Vector2 pos_change(cos(angle) * speed, sin(angle) * speed);
	scircle.c += (pos_change);
	center += (pos_change);
	cshape.poly[0].move(pos_change);

	return false;
}

void Chaser::render(bool show_hbar) {
	CV::color(0.9, 0.8, 0);
	cshape.poly[0].fill();
	CV::color(0.2, 1, 1);
	CV::circleFill(scircle.c.x, scircle.c.y, 12, 16);
	hbar.render((Vector2){scircle.c.x, scircle.c.y - 32});
}

bool Chaser::hit() {
	knockback = true;
	hbar.health--;
	return (hbar.health <= 0);
}

void Chaser::setPosition(float x, float y) {
	Vector2 new_center(x, y);
	Vector2 pos_change = new_center - center;
	scircle.c += (pos_change);
	center += (pos_change);
	cshape.poly[0].move(pos_change);
}
