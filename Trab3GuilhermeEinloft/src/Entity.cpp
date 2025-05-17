#include "Entity.h"
#include "Vector2.h"

Entity::Entity(Vector2 center, int max_health, EntityType type) {
	this->center = center;
	this->type = type;
	hbar.max_health = max_health;
	hbar.health = max_health;
	active = true;
}

bool Entity::hit() {
	hbar.health--;
	return (hbar.health <= 0);
}

bool Entity::move(Vector2 target, float deltaTime, Entity **entities) {
	return false;
}

void Entity::render() {
}

CollisionShape Entity::getCollisionShape() {
	return cshape;
}

Vector2 Entity::getCenter() {
	return center;
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
	return ((type & ETYPE_POWERUP) == ETYPE_POWERUP);
}
