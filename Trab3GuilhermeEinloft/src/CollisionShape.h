#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

#include "Poly.h"

typedef struct collision_shape {
	int size;
	Poly *poly;
} CollisionShape;

#endif
