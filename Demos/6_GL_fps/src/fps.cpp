/**
*   Programa para ilustrar:
*   - Uso de fontes
*   - cálculo de FPS (frames per second)
*
*   Autor: Cesar Tadeu Pozzer
*   UFSM - 15/06/2007
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
**/

#include <GL/glut.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Frames.h"
#include "GLFont.h"

GLFont	* font;
Frames  * frames;
float     fps = 100.0;  //valor inicial
float     ang = 0;

//faz a geracao da cena. Esta funcao eh chamada apenas no inicio da execucao.
//o objetivo do controle de FPS é garantir que o objeto seja animado sempre na mesma velocidade, independente do FPS da aplicacao.
void init()
{
   glClearColor(0, 0, 0, 1);
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);

   frames  = new Frames();
   font    = new GLFont();
}

////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{
   float abertura = 45.0;
   float znear  = 1;
   float zfar   = 2000;
   float aspect = 1;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );
   gluPerspective(abertura, aspect, znear, zfar);
   glMatrixMode(GL_MODELVIEW);

   glColor3f(1,1,1);
   glLoadIdentity();
   glTranslated(0, 0, -10);
   glRotated(ang, 0, 1, 0);
   glutWireTeapot(2);

   fps = frames->getFrames();
   ang += 1/fps * 20; //controle do FPS -> altera a velocidade de animacao do objeto.

   //Sleep(1000);

   static char text[50];
   font->startText();
   glColor3f(1,1,1);
   sprintf(text, "FPS:  %.0f", fps);
   font->print(20, 460, text);
   font->endText();


   glutSwapBuffers();
}


////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char**argv)
{
   glutInit(&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

   glutInitWindowSize (700, 500);
   glutInitWindowPosition (450, 10);
   glutCreateWindow ("FPS Demo - A velocidade de rotacao é fixa, independente do FPS");
   init();

   glutDisplayFunc(display);
   glutIdleFunc(display);

   glutMainLoop();
   return 0;
}
