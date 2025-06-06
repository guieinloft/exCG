/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//			02/2025
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
#include "Vector2.h"
#include "Vector3.h"

#include "bezier.h"
#include "surface.h"

#include "Keys.h"
#include "Mouse.h"

#include "edit.h"
#include "preview.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

Mouse smouse;
Keys keys;
bool shift = 0, ctrl = 0;

Vector3 in[MAX_RES][MAX_RES];
Vector2 out[MAX_RES][MAX_RES];
int m = 16, n = 16;

float d = 300;

bool edit = 0;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.

void render_bg() {
	CV::translate(screenWidth / 2, screenHeight / 2);
	CV::color(0.5, 0.5, 0.5);
	CV::line(-screenWidth / 2, 0, screenWidth / 2, 0);
	CV::line(0, -screenHeight / 2, 0, screenHeight / 2);
}

void render()
{
	render_bg();
	if (edit)
		edit_render(in, out, m, n, d, screenWidth, screenHeight);
	else
		prev_render(in, out, m, n, d, screenWidth, screenHeight);
	usleep(10000); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
	keys.shift |= (key == 212 || key == 213);
	keys.ctrl |= (key == 214);
	switch (key) {
	case 201:
		keys.k_up = true;
		break;
	case 203:
		keys.k_down = true;
		break;
	case 200:
		keys.k_left = true;
		break;
	case 202:
		keys.k_right = true;
		break;
	case 204:
		keys.k_pgup = true;
		break;
	case 205:
		keys.k_pgdown = true;
		break;
	case 'e':
		edit = !edit;
		break;
	}
	printf("\nPressionou: %d" , key);
	if (!edit)
		prev_check_keys(keys);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
	keys.shift &= (key != 212 && key != 213);
	keys.ctrl &= (key != 214);
	switch (key) {
	case 201:
		keys.k_up = false;
		break;
	case 203:
		keys.k_down = false;
		break;
	case 200:
		keys.k_left = false;
		break;
	case 202:
		keys.k_right = false;
		break;
	case 204:
		keys.k_pgup = false;
		break;
	case 205:
		keys.k_pgdown = false;
		break;
	}
	printf("\nLiberou: %d" , key);
	if (!edit)
		prev_check_keys(keys);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
	smouse.xp = smouse.x;
	smouse.yp = smouse.y;
	smouse.x = x - screenWidth/2;
	smouse.y = y - screenHeight/2;
	smouse.button = button;
	smouse.state = state;
	smouse.wheel = wheel;
	smouse.direction = direction;
	switch (button) {
		case 0:
		smouse.l = !state;
		break;
		case 1:
		smouse.m = !state;
		break;
		case 2:
		smouse.r = !state;
		break;
	}
	printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
	if (edit)
		edit_check_mouse(smouse, in, &m, &n, d);
	else
		prev_check_mouse(smouse);
}

int main(void)
{
	edit_init(in, m, n, d);
	prev_init();
	CV::init(&screenWidth, &screenHeight, "PROJETO EM BRANCO");
	CV::run();
}
