#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <GL/glut.h>

#include "vec3.h"
#include "camera.h"
#include "model.h"
#include "meteor.h"
#include "timer.h"

#define METEOR_MAX_NUMBER 256

int screenW = 640, screenH = 480;

struct camera cam;
struct timer timer;
struct model meteor_model;
struct model sun_model;
struct meteor meteors[METEOR_MAX_NUMBER];

int light = 1;
int texture = 1;
int fill = 1;
int smooth = 1;

void display();
void reshape(int w, int h);
void motionFunc(int x, int y);
void passiveMotionFunc(int x, int y);
void mouseFunc(int button, int state, int x, int y);
void keyboardFunc(unsigned char key, int x, int y);
void keyboardUpFunc(unsigned char key, int x, int y);

/* audio */

int gl_init(char *name) 
{
	/* glut */
	int argc = 0;
	glutInit(&argc, NULL);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenW, screenH);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(name);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMotionFunc(motionFunc);
	glutPassiveMotionFunc(motionFunc);
	glutMouseFunc(mouseFunc);
	glutKeyboardFunc(keyboardFunc);
	glutKeyboardUpFunc(keyboardUpFunc);
	glutIdleFunc(display);

	/* gl */
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glShadeModel(GL_TEXTURE);
	glShadeModel(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	/* camera */
	camera_init(&cam, vset(0, 10, 100), screenW, screenH);

	/* lights */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat[]){0.2, 0.2, 0.5, 1});
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat[]){1, 1, 0.5, 1});
	
	return 0;
}

void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera_display(&cam, screenW, screenH);
	
	if (light)
		glEnable(GL_LIGHTING);
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION,
			(GLfloat[]){cam.pos.x, cam.pos.y, cam.pos.z, 1});
	glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat[]){0, 0, 0, 1});
	glPopMatrix();

	glColor3f(1, 1, 1);
	for (int i = 0; i < METEOR_MAX_NUMBER; i++)
		meteor_render(meteors[i], cam.pos);
	
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(1, 1, 1);
	model_render(&sun_model);
	glPopMatrix();
	
	glFlush();
	glutSwapBuffers();

	camera_update(&cam, timer.deltatime);
	timer_update(&timer);
	printf("DeltaTime: %f\r", timer.deltatime);
}

void reshape(int w, int h)
{
	screenW = w;
	screenH = h;
}

void motionFunc(int x, int y)
{
	camera_check_mouse(&cam, x, y, screenW, screenH);
}

void mouseFunc(int button, int state, int x, int y)
{
}

void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		if (fill)
			glPolygonMode(GL_FRONT, GL_LINE);
		else
			glPolygonMode(GL_FRONT, GL_FILL);
		fill = !fill;
		break;
	case '2':
		if (smooth)
			glShadeModel(GL_FLAT);
		else
			glShadeModel(GL_SMOOTH);
		smooth = !smooth;
		break;
	case '3':
		if (texture)
			glDisable(GL_TEXTURE_2D);
		else
			glEnable(GL_TEXTURE_2D);
		texture = !texture;
		break;
	case '4':
		light = !light;
		break;
	}
	camera_check_key_press(&cam, key);
}

void keyboardUpFunc(unsigned char key, int x, int y)
{
	camera_check_key_release(&cam, key);
}

int main(int argc, char **argv)
{
	srand(time(0));
	if (gl_init("TESTE") < 0)
		exit(-1);
	timer_init(&timer);
	model_load_obj(&meteor_model,
			"./Trab5GuilhermeEinloft/models/meteor.obj");
	model_load_texture(&meteor_model,
			"./Trab5GuilhermeEinloft/images/meteor_texture.bmp");
	model_load_obj(&sun_model,
			"./Trab5GuilhermeEinloft/models/sun.obj");
	model_load_texture(&sun_model,
			"./Trab5GuilhermeEinloft/images/sun_texture.bmp");
	for (int i = 0; i < METEOR_MAX_NUMBER; i++)
		meteors[i] = meteor_init(&meteor_model);
	glutMainLoop();
	return 0;
}

