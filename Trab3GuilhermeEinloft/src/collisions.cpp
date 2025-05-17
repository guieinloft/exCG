#include "Vector2.h"
#include "Poly.h"
#include "Circle.h"
#include "b_spline.h"
#include "collisions.h"
#include "Entity.h"

#include <stdlib.h>
#include <math.h>

bool collide(Poly p, Poly q) {
	bool colliding = true;
	Vector2 *axis = (Vector2*)malloc(sizeof(Vector2) * (p.size + q.size));
	// vetores axisendiculares às arestas
	for (int i = 0; i < p.size; i++) {
		axis[i] = p.v[(i + 1) % p.size] - p.v[i];
		axis[i] = axis[i].perp();
	}
	for (int i = 0; i < q.size; i++) {
		axis[i + p.size] = q.v[(i + 1) % q.size] - q.v[i];
		axis[i + p.size] = axis[i + p.size].perp();
	}
	// projetando os vertices nos vetores perpendiculares
	for (int i = 0; i < (p.size + q.size); i++) {
		float amax = -HUGE_VALF;
		float amin = HUGE_VALF;
		float bmax = -HUGE_VALF;
		float bmin = HUGE_VALF;
		for (int j = 0; j < p.size; j++) {
			float dot = axis[i].dot(p.v[j]);
			if (dot > amax) amax = dot;
			if (dot < amin) amin = dot;
		}
		for (int j = 0; j < q.size; j++) {
			float dot = axis[i].dot(q.v[j]);
			if (dot > bmax) bmax = dot;
			if (dot < bmin) bmin = dot;
		
		}
		if (!((amin < bmax && amin > bmin) || (bmin < amax && bmin > amin))) {
			colliding = false;
			break;
		}
	}
	free(axis);
	return colliding;
}

bool collide_ll(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4) {
	float ua = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x))
		/ ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));
	float ub = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x))
		/ ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));

	if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1) return true;
	return false;
}

bool collide_pl(Poly p, Vector2 p1, Vector2 p2) {
	for (int i = 0; i < p.size; i++) {
		if (collide_ll(p.v[i], p.v[(i + 1) % p.size], p1, p2))
			return true;
	}
	return false;
}

int collideTankBorder(Tank *t, Vector2 b_out[B_SEG], Vector2 b_in[B_SEG]) {
	Poly p = t->getCollisionShape().poly[0];
	int dir = 0;
	for (int i = 0; i < B_SEG; i++) {
		dir -= (int)collide_ll(p.v[0], p.v[1], b_out[i], b_out[(i + 1) & B_MASK]);
		dir += (int)collide_ll(p.v[2], p.v[3], b_out[i], b_out[(i + 1) & B_MASK]);
		dir -= (int)collide_ll(p.v[0], p.v[1], b_in[i], b_in[(i + 1) & B_MASK]);
		dir += (int)collide_ll(p.v[2], p.v[3], b_in[i], b_in[(i + 1) & B_MASK]);
		if (dir != 0)
			return dir;
	}
	for (int i = 0; i < B_SEG; i++) {
		dir += (int)collide_ll(p.v[1], p.v[2], b_out[i], b_out[(i + 1) & B_MASK]);
		dir += (int)collide_ll(p.v[3], p.v[0], b_out[i], b_out[(i + 1) & B_MASK]);
		dir += (int)collide_ll(p.v[1], p.v[2], b_in[i], b_in[(i + 1) & B_MASK]);
		dir += (int)collide_ll(p.v[3], p.v[0], b_in[i], b_in[(i + 1) & B_MASK]);
		if (dir != 0)
			return 1;
	}
	return 0;
}

bool checkCollision(Entity *e1, Entity *e2) {
	CollisionShape s1 = e1->getCollisionShape();
	CollisionShape s2 = e2->getCollisionShape();
	for (int i = 0; i < s1.size; i++) {
		for (int j = 0; j < s2.size; j++) {
			if (collide(s1.poly[i], s2.poly[j])) {
				return true;
			}
		}
	}
	return false;
}

void checkCollisions(Entity **entities, int *score, int *entity_num) {
	for (int i = 1; i < ENTITY_FIXED_NUM; i++) {
		if (entities[i] == nullptr)
			continue;
		for (int j = i + 1; j < ENTITY_NUM; j++) {
			if (entities[j] == nullptr)
				continue;
			if (checkCollision(entities[i], entities[j])) {
				if (entities[j]->hit()) {
					if (j < ENTITY_FIXED_NUM) {
						*score += entities[j]->getMaxHealth();
						(*entity_num)--;
					}
					delete entities[j];
					entities[j] = nullptr;
				}
				if (entities[i]->hit()) {
					*score += entities[i]->getMaxHealth();
					(*entity_num)--;
					delete entities[i];
					entities[i] = nullptr;
					break;
				}
			}
		}
	}
}

bool checkTankCollisions(Tank *tank, Entity **entities, int *score, int *entity_num) {
	for (int j = 1; j < ENTITY_NUM; j++) {
		if (entities[j] == nullptr)
			continue;
		if (checkCollision(tank, entities[j])) {
			if (entities[j]->isPowerup()) {
				tank->hit_powerup(((Powerup*)entities[j])->getPowerupType());
				*score += entities[j]->getMaxHealth();
				(*entity_num)--;
				delete entities[j];
				entities[j] = nullptr;
				continue;
			}
			else if (tank->hit()) {
				return true;
			}
			if (entities[j]->hit()) {
				if (j < ENTITY_FIXED_NUM) {
					*score += entities[j]->getMaxHealth();
					(*entity_num)--;
				}
				delete entities[j];
				entities[j] = nullptr;
			}
		}
	}
	return false;
}

bool checkCollisionBorder(Entity *e1, Vector2 b_out[], Vector2 b_in[]) {
	CollisionShape s1 = e1->getCollisionShape();
	for (int i = 0; i < s1.size; i++) {
		for (int j = 0; j < B_SEG; j++) {
			if (collide_pl(s1.poly[i], b_in[j], b_in[(j + 1) & B_MASK]))
				return true;
			if (collide_pl(s1.poly[i], b_out[j], b_out[(j + 1) & B_MASK]))
				return true;
		}
	}
	Poly tri;
	tri.size = 3;
	tri.v = (Vector2*)malloc(sizeof(Vector2) * 3);
	for (int i = 0; i < s1.size; i++) {
		for (int j = 0; j < B_SEG; j++) {
			tri.v[0] = b_out[j];
			tri.v[1] = b_out[(j + 1) & B_MASK];
			tri.v[2] = b_in[j];
			if (collide(s1.poly[i], tri)) {
				free(tri.v);
				return false;
			}
			tri.v[0] = b_in[(j + 1) & B_MASK];
			if (collide(s1.poly[i], tri)) {
				free(tri.v);
				return false;
			}
		}
	}
	return true;
}

void checkCollisionsBorder(Entity **entities, Vector2 b_out[], Vector2 b_in[], int *score, int *entity_num) {
	for (int i = 1; i < ENTITY_NUM; i++) {
		if (entities[i] == nullptr)
			continue;
		if (checkCollisionBorder(entities[i], b_out, b_in)) {
			if (entities[i]->hit()) {
				if (i < ENTITY_FIXED_NUM) {
					*score += entities[i]->getMaxHealth();
					(*entity_num)--;
				}
				delete entities[i];
				entities[i] = nullptr;
			}
		}
	}
}
