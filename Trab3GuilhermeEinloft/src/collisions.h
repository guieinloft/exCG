#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "Poly.h"
#include "Circle.h"
#include "Vector2.h"
#include "Entity.h"
#include "Tank.h"
#include "Powerup.h"

void entity_remove(Entity **entities, int i, int *score, int *entity_num);
int collideTankBorder(Tank *t, Vector2 b_out[], Vector2 b_in[]);
bool checkTankCollisions(Tank *tank, Entity **entities, int *score, int *entity_num);
bool checkCollision(Entity *e1, Entity *e2);
bool checkSpawnCollision(Entity *e1, Entity *e2);
void checkSpawnCollisions(Entity **entities, bool *collisions);
void checkCollisions(Entity **entities, int *score, int *entity_num);
bool checkCollisionBorder(Entity *e1, Vector2 b_out[], Vector2 b_in[]);
bool checkSpawnCollisionBorder(Entity *e1, Vector2 b_out[], Vector2 b_in[]);
void checkCollisionsBorder(Entity **entities, Vector2 b_out[], Vector2 b_in[], int *score, int *entity_num);
void checkSpawnCollisionsBorder(Entity **entities, bool *collisions, Vector2 b_out[], Vector2 b_in[]);
bool collide_point_c(Vector2 p, Circle c);

#endif
