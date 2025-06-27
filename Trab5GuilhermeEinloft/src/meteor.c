#include <stdlib.h>
#include <stdio.h> /* debug */
#include <time.h>
#include <GL/glut.h>
#include "vec3.h"
#include "model.h"
#include "meteor.h"

struct meteor meteor_init(struct model *model)
{
	struct meteor meteor;
	meteor.pos.x = ((rand() & 0xfff) - (rand() & 0xfff)) * 0.125;
	meteor.pos.y = ((rand() & 0xff) - (rand() & 0xff)) * 0.125;
	meteor.pos.z = ((rand() & 0xfff) - (rand() & 0xfff)) * 0.125;
	meteor.scale = ((rand() & 0xff) - (rand() & 0xff)) / 127.5 + 3;
	meteor.model = model;
	return meteor;
}

void meteor_render(struct meteor meteor, struct vec3 cam_pos)
{
	struct vec3 dif = vset(cam_pos.x - meteor.pos.x,
			cam_pos.y - meteor.pos.y,
			cam_pos.z - meteor.pos.z);
	float distance = dif.x * dif.x + dif.y * dif.y + dif.z * dif.z;
	if (distance > 0xffff)
		return;
	int res = (8 * (1 / (distance / 0xfff + 1))) + 8;
	glPushMatrix();
	glTranslatef(meteor.pos.x, meteor.pos.y, meteor.pos.z);
	glutSolidSphere(meteor.scale, res, res);
	glPopMatrix();
}
