#include <GL/glut.h>
#include <math.h>
#include <stdio.h> /* debug */

#include "vec3.h"
#include "camera.h"

#define SPEED 16.0
#define SPEED_INC 0.125
#define ANGLE_INC 2.0
#define MAX_PITCH 1.553343 /* 90 graus */

void camera_init(struct camera *cam, struct vec3 pos, int screenW, int screenH)
{
	cam->fov = 45.0;
	cam->znear = 1;
	cam->zfar = 100000;
	cam->aspect = (float)screenW / screenH;
	cam->yaw = -M_PI/2;
	cam->pitch = 0;
	cam->pitch_inc = cam->yaw_inc = 0;
	cam->speed = cam->speed_inc = 0;
	cam->speed_dir = 0;
	/* vetor posicao */
	cam->pos = pos;
	/* vetor direcao */
	cam->dir = vset(0, 0, -1);
	/* vetor up */
	cam->up = vset(0, 1, 0);

	/* init opengl */
	glViewport(0, 0, (GLsizei)screenW, (GLsizei)screenH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(cam->fov, cam->aspect, cam->znear, cam->zfar);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0, 0, 0, 1);
}

void camera_display(struct camera *cam, int screenW, int screenH)
{
	cam->aspect = (float)screenW / screenH;
	glViewport(0, 0, (GLsizei)screenW, (GLsizei)screenH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(cam->fov, cam->aspect, cam->znear, cam->zfar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity( );
	gluLookAt(cam->pos.x,
			cam->pos.y,
			cam->pos.z,
			cam->pos.x + cam->dir.x,
			cam->pos.y + cam->dir.y,
			cam->pos.z + cam->dir.z,
			cam->up.x,
			cam->up.y,
			cam->up.z);
	glPopMatrix();
}

void camera_update(struct camera *cam, float deltatime)
{
	if (cam->speed_dir != 0)
		cam->speed_inc = SPEED_INC * cam->speed_dir;
	else
		cam->speed_inc = SPEED_INC *
				((cam->speed < 0) -
				(cam->speed > 0));
	cam->speed += cam->speed_inc *
			((cam->speed < SPEED * deltatime
			&& cam->speed > -SPEED * deltatime)
			|| cam->speed_dir == 0) * deltatime;
	if (cam->speed_inc == 0)
		cam->speed = 0;
	cam->yaw += cam->yaw_inc * deltatime;
	cam->pitch += cam->pitch_inc * deltatime;
	if (cam->pitch > MAX_PITCH)
		cam->pitch = MAX_PITCH;
	else if (cam->pitch < -MAX_PITCH)
		cam->pitch = -MAX_PITCH;
	cam->dir = vset(cos(cam->yaw) * cos(cam->pitch),
			sin(cam->pitch),
			sin(cam->yaw) * cos(cam->pitch));
	cam->pos = vadd(cam->pos, vset(cam->dir.x * cam->speed,
			cam->dir.y * cam->speed,
			cam->dir.z * cam->speed));
}

void camera_check_key_press(struct camera *cam, int key)
{
	switch (key) {
	case 'w':
		cam->speed_dir += 1;
		break;
	case 's':
		cam->speed_dir -= 1;
		break;
	}
}

void camera_check_key_release(struct camera *cam, int key)
{
	switch (key) {
	case 'w':
		cam->speed_dir -= 1;
		break;
	case 's':
		cam->speed_dir += 1;
		break;
	}
}

void camera_check_mouse(struct camera *cam, int x, int y,
		int screenW, int screenH)
{
	cam->yaw_inc = ((x * ANGLE_INC / screenW) - ANGLE_INC * 0.5)
			* cam->aspect;
	cam->pitch_inc = -(y * ANGLE_INC / screenH) + ANGLE_INC * 0.5;
}
