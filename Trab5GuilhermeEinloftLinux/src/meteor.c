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
	glPushMatrix();
	glTranslatef(meteor.pos.x, meteor.pos.y, meteor.pos.z);
	model_render(meteor.model);
	glPopMatrix();
}
