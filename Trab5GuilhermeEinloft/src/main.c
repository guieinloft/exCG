#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <GL/glut.h>

#include "vec3.h"
#include "camera.h"
#include "timer.h"
#include "model.h"
#include "meteor.h"

int screenW = 640, screenH = 480;

struct camera cam;
struct timer timer;
struct model meteor_model;
struct meteor meteors[400];

void display();
void reshape(int w, int h);
void motionFunc(int x, int y);
void passiveMotionFunc(int x, int y);
void mouseFunc(int button, int state, int x, int y);
void keyboardFunc(unsigned char key, int x, int y);
void keyboardUpFunc(unsigned char key, int x, int y);

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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	/* camera */
	camera_init(&cam, vset(0, 0, 0), screenW, screenH);

	/* lights */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat[]){0, 0, 1, 1});
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat[]){1, 1, 0, 1});
	
	glEnable(GL_COLOR_MATERIAL);
	return 0;
}

void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera_display(&cam, screenW, screenH);

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION,
			(GLfloat[]){cam.pos.x, cam.pos.y, cam.pos.z, 1});
	glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat[]){0, 0, 0, 1});
	glPopMatrix();

	for (int i = 0; i < 400; i++)
		meteor_render(meteors[i], cam.pos);
	
	glutSwapBuffers();

	camera_update(&cam, timer.deltatime);
	timer_update(&timer);
	printf("%f\n", timer.deltatime);
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
	//model_load(&meteor_model, "./meteor.obj");
	for (int i = 0; i < 200; i++)
		meteors[i] = meteor_init(&meteor_model);
	glutMainLoop();
	return 0;
}

