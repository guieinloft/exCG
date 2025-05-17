#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "Poly.h"
#include "Circle.h"
#include "Vector2.h"
#include "Entity.h"
#include "Tank.h"
#include "Powerup.h"

int collideTankBorder(Tank *t, Vector2 b_out[], Vector2 b_in[]);
bool checkTankCollisions(Tank *tank, Entity **entities, int *score, int *entity_num);
bool checkCollision(Entity *e1, Entity *e2);
void checkCollisions(Entity **entities, int *score, int *entity_num);
bool checkCollisionBorder(Entity *e1, Vector2 b_out[], Vector2 b_in[]);
void checkCollisionsBorder(Entity **entities, Vector2 b_out[], Vector2 b_in[], int *score, int *entity_num);

#endif
