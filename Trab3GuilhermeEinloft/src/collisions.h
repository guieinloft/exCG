#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "Poly.h"
#include "Circle.h"
#include "Vector2.h"
#include "Entity.h"
#include "Tank.h"
#include "Powerup.h"

//helper que remove entidade (e spawna explosao no lugar)
void entity_remove(Entity **entities, int i, int *score, int *entity_num);
//colisao tanque x parede
int collideTankBorder(Tank *t, Vector2 b_out[], Vector2 b_in[]);
//colisao tanque x entidades
bool checkTankCollisions(Tank *tank, Entity **entities, int *score, int *entity_num);
//colisao entre duas entidades
bool checkCollision(Entity *e1, Entity *e2);
//colisao entre os circulos de spawn de duas entidades
bool checkSpawnCollision(Entity *e1, Entity *e2);
//colisao entre os circulos de spawn entre todas entidades
void checkSpawnCollisions(Entity **entities, bool *collisions);
//colisao entre todas as entidades
void checkCollisions(Entity **entities, int *score, int *entity_num);
//colisao entre uma entidade e a borda
bool checkCollisionBorder(Entity *e1, Vector2 b_out[], Vector2 b_in[]);
//colisao entre um circulo de spawn de uma entidade e a borda
bool checkSpawnCollisionBorder(Entity *e1, Vector2 b_out[], Vector2 b_in[]);
//colisao entre todas as entidades e a borda
void checkCollisionsBorder(Entity **entities, Vector2 b_out[], Vector2 b_in[], int *score, int *entity_num);
//colisao entre todos os circulos de spawn e a borda
void checkSpawnCollisionsBorder(Entity **entities, bool *collisions, Vector2 b_out[], Vector2 b_in[]);
//colisao ponto x circulo (para o editor)
bool collide_point_c(Vector2 p, Circle c);

#endif
