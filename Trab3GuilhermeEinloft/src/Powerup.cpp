#include "Vector2.h"
#include "gl_canvas2d.h"
#include "Powerup.h"
#include "Circle.h"

#define P_ANGLE_INC 0.0021816616f

Powerup::Powerup(Vector2 center, PowerupType type) : Entity(center, 1, ETYPE_POWERUP) {
	cshape.size = 2;
	cshape.poly = (Poly*)malloc(sizeof(Poly) * 2);
	cshape.poly[0].createShape(center, 24, 3);
	cshape.poly[1].createShape(center, -24, 3);
	this->type = type;
}

void Powerup::render() {
	switch (type) {
	case PWUP_SUPER:
		CV::color(0.75f, 0.75f, 0.3f);
		break;
	case PWUP_SHIELD:
		CV::color(0.3f, 0.3f, 0.75f);
		break;
	case PWUP_HEALTH:
		CV::color(0.3f, 0.75f, 0.3f);
		break;
	}
	cshape.poly[0].fill();
	cshape.poly[1].fill();
}

bool Powerup::move(Vector2 target, float deltaTime, Entity **entities) {
	cshape.poly[0].rot(P_ANGLE_INC * deltaTime, center);
	cshape.poly[1].rot(P_ANGLE_INC * deltaTime, center);
	return false;
}

PowerupType Powerup::getPowerupType() {
	return type;
}

bool Powerup::hit() {
	return false;
}
