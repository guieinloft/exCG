#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Explosion.h"

#define E_TIMER 125

Explosion::Explosion(Vector2 pos, float radius, int damage) : Entity(pos, radius, 0, ETYPE_EXPLOSION) {
	cshape.size = 0;
	cshape.poly = nullptr;
	timer = E_TIMER * damage;
}

bool Explosion::move(Vector2 target, float deltaTime, Entity **entities) {
	timer -= deltaTime;
	return (timer <= 0);
}

void Explosion::render(bool show_hbar) {
	for (int i = 0; i < 5; i++) {
		CV::color(1, (double)(rand() & 255)/255, 0);
		float pos_x = scircle.c.x - scircle.r
			+ rand() % (2 * (int)scircle.r);
		float pos_y = scircle.c.y - scircle.r
			+ rand() % (2 * (int)scircle.r);
		CV::circleFill(pos_x, pos_y, scircle.r * 0.5, 16);
	}
}

bool Explosion::hit() {
	return false;
}
