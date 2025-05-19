#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Powerup.h"
#include "Circle.h"

#define P_ANGLE_INC 0.0021816616f

Powerup::Powerup(Vector2 center, EntityType type) : Entity(center, 24, 1, type) {
	cshape.size = 2;
	cshape.poly = (Poly*)malloc(sizeof(Poly) * 2);
	cshape.poly[0].createShape(center, 24, 3);
	cshape.poly[1].createShape(center, -24, 3);
}

void Powerup::render(bool show_hbar) {
	switch (type) {
	case ETYPE_POWERUP_SUPER:
		CV::color(0.75f, 0.75f, 0.3f);
		break;
	case ETYPE_POWERUP_SHIELD:
		CV::color(0.3f, 0.3f, 0.75f);
		break;
	case ETYPE_POWERUP_HEALTH:
		CV::color(0.3f, 0.75f, 0.3f);
		break;
	default:
		CV::color(1, 1, 1);
	}
	cshape.poly[0].fill();
	cshape.poly[1].fill();
}

bool Powerup::move(Vector2 target, float deltaTime, Entity **entities) {
	cshape.poly[0].rot(P_ANGLE_INC * deltaTime, center);
	cshape.poly[1].rot(P_ANGLE_INC * deltaTime, center);
	return false;
}

bool Powerup::hit() {
	return false;
}

void Powerup::setPosition(float x, float y) {
	Vector2 new_center(x, y);
	Vector2 poschange = new_center - center;
	cshape.poly[0].move(poschange);
	cshape.poly[1].move(poschange);
	center = new_center;
	scircle.c = new_center;
}
