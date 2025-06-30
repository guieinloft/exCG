/*******************************************************************
*
*	Descrição: Exemplo de utilização da biblioteca glfw - Canvas2D
*
*	Autor: Alex Thomas Almeida Frasson (2012)
*          Cesar Pozzer (2024)
*
*	Universidade Federal de Santa Maria
*
*   -------------------------------------------
*
*   OBSERVACAO IMPORTANTE:
*   As libs devem estar na seguinte ordem (LINKER SETTINS)
*   1) glfw3dll.a
*   2) libglfw3.a
*   3) libopengl32.a

*******************************************************************/


#include <GLFW/glfw3.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "Bola.h"
#include "Relogio.h"
#include "Botao.h"

#include <stdlib.h>
#include <stdio.h>


void render(void);
void cursorPosCallback(GLFWwindow *w, double x, double y);
void mouseButtonCallback(GLFWwindow *w, int button, int action, int mods);
void scrollCallBack(GLFWwindow *w, double xoffset, double yoffset);
void keyCallBack(GLFWwindow *w, int, int, int, int);

void init(void);

int screenWidth = 900, screenHeight = 900;
int mouseX, mouseY;

Bola    *b = NULL;
Relogio *r = NULL;
Botao   *bt = NULL; //se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.

bool tecla = 0;

void DesenhaLinhaDegrade()
{
   Vector2 p;
   for(float i=0; i<350; i++)
   {
	  CV::color(i/200, i/200, i/200);
	  p.set(i+100, 240);
	  CV::point(p);
   }

   //desenha paleta de cores predefinidas na Canvas2D.
   for(int idx = 0; idx < 14; idx++)
   {
	  CV::color(idx);
      CV::translate(20 + idx*30, 100);
	  CV::rectFill(Vector2(0,0), Vector2(30, 30));
   }
   CV::translate(0, 0);
}


void render(void)
{
   bt->Render();
   b->anima();

   DesenhaLinhaDegrade();

   CV::circleFill(0,0, 30,30);
	printf("\ntecla: %d", tecla);

}


void cursorPosCallback(GLFWwindow *w, double x, double y)
{
    mouseX = (int)x;
    mouseY = CV::ConvertMouseCoord((int)y);
    printf("\ncursor do mouse: (%d, %d)", mouseX, mouseY );
}

void mouseButtonCallback(GLFWwindow *w, int button, int action, int mods)
{
    printf("\nbotao do mouse");
    if( bt->Colidiu(mouseX, mouseY) )
    {
       printf("\nClicou no botao\n");
    }
}

void scrollCallBack(GLFWwindow *w, double xoffset, double yoffset)
{
    printf("\nscrool do mouse");
}

//callback de keyboard
void keyCallBack(GLFWwindow *w, int a, int b, int c, int d)
{
	tecla = c;
    printf("\nkeyboard %d %d %d %d", a, b, c, d);
}

int main()
{

   b = new Bola();
   r = new Relogio();
   bt = new Botao(200, 400, 140, 50, "Sou um botao");

   CV::init(screenWidth, 600, "Titulo da Janela: Canvas 2D GLFW ");
   CV::run();

   return 0;
}

