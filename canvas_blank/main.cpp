/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         02/2025
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.1
//
//  Instruções:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gl_canvas2d.h"

#include "b_spline.h"

#include "Vector3.h"
#include "cube.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

Vector3 in[8] = {
	(Vector3){-1, -1, -1},
	(Vector3){-1,  1, -1},
	(Vector3){ 1,  1, -1},
	(Vector3){ 1, -1, -1},
	(Vector3){-1, -1,  1},
	(Vector3){-1,  1,  1},
	(Vector3){ 1,  1,  1},
	(Vector3){ 1, -1,  1}
};

Vector2 out[8];
float ang = 0;
float tx = 0;
float ty = 0;
float tz = 3;
float d = 50;
float r = 5;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
	CV::translate(screenWidth/2, screenHeight/2);
	CV::color(0, 0, 0);
	Vector3 p;
	for (int i = 0; i < 8; i++) {
		p = in[i];
		p = rotY(p, ang);
		p = translate(p, tx, ty, tz);
		out[i] = proj(p, d);
	}
	cube_draw(out);
	ang += 0.01;
   usleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
	switch (key) {
	case 'w':
		ty += 0.1;
		break;
	case 's':
		ty -= 0.1;
		break;
	case 'd':
		tx += 0.1;
		break;
	case 'a':
		tx -= 0.1;
		break;
	}
	printf("\n%.0f" , d);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "PROJETO EM BRANCO");
   CV::run();
}
