#include "Entity.h"
#include "Vector2.h"
#include "Explosion.h"

Entity::Entity(Vector2 center, float radius, int max_health, EntityType type) {
	this->center = center;
	this->scircle.c = center;
	this->scircle.r = radius;
	this->type = type;
	hbar.max_health = max_health;
	hbar.health = max_health;
	active = true;
}

Entity::~Entity() {
}

bool Entity::hit() {
	hbar.health--;
	return (hbar.health <= 0);
}

bool Entity::move(Vector2 target, float deltaTime, Entity **entities) {
	return false;
}

void Entity::render(bool show_hbar) {
}

void Entity::setPosition(float x, float y) {
}

CollisionShape Entity::getCollisionShape() {
	return cshape;
}

Vector2 Entity::getCenter() {
	return center;
}

Circle Entity::getSpawnCircle() {
	return scircle;
}

bool Entity::isActive() {
	return active;
}

EntityType Entity::getType() {
	return type;
}

int Entity::getMaxHealth() {
	return hbar.max_health;
}

bool Entity::isPowerup() {
	return (type == ETYPE_POWERUP_SUPER
		|| type == ETYPE_POWERUP_SHIELD
		|| type == ETYPE_POWERUP_HEALTH);
}

void Entity::renderSpawnCircle(bool colliding) {
	if (colliding)
		CV::color(1, 0, 0);
	else
		CV::color(0, 1, 0);
	CV::circle(scircle.c.x, scircle.c.y, scircle.r, 32);
}

