#include "vec3.h"

struct vec3 vset(float x, float y, float z)
{
	return (struct vec3){x, y, z};
}

struct vec3 vadd(struct vec3 v1, struct vec3 v2)
{
	return (struct vec3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}
