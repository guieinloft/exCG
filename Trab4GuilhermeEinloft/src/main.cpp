/*
 * main.cpp
 * Requisitos:
 * - Edição da curva Bezier em tempo real
 * - Atualização instantânea do modelo
 * - Parametrização do número de vértices do objeto
 * - Translação no eixo Y para fazer molas
 * - Visualização do objeto em diferentes posições/ângulos
 * - Projeções ortográfica e em perspectiva
 * Extras:
 * - Adição e remoção de pontos na curva
 * - Geração e exibição de normais de cada face
 * - Back-face culling (remoção de faces ocultas)
 * - Z-buffer com iluminação por pixel (Phong)
 * - Salvamento e carregamento de curvas
 */

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

#include "Mouse.h"
#include "Button.h"

#include "edit.h"
#include "preview.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

Mouse smouse;
bool shift = 0, ctrl = 0;

Vector3 in[MAX_RES][MAX_RES];
Vector3 out[MAX_RES][MAX_RES];
Vector3 out_proj[MAX_RES][MAX_RES];
int m = 16, n = 16;
float d = 400;
bool edit = 0;
Button bt_edit(0, 0, 128, 32);

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.

void render_bg()
{
	CV::translate(screenWidth / 2, screenHeight / 2);
	CV::color(0.75, 0.75, 0.75);
	float halfSW = screenWidth >> 1;
	float halfSH = screenHeight >> 1;
	for (int i = 0; i < halfSW; i += SNAP_RES) {
		CV::line(i, -halfSH, i, halfSH);
		CV::line(-i, -halfSH, -i, halfSH);
	}
	for (int i = 0; i < halfSH; i += SNAP_RES) {
		CV::line(-halfSW, i, halfSW, i);
		CV::line(-halfSW, -i, halfSW, -i);
	}
	CV::color(0.5, 0.5, 0.5);
	CV::line(-screenWidth / 2, 0, screenWidth / 2, 0);
	CV::line(0, -screenHeight / 2, 0, screenHeight / 2);
}

void render()
{
	render_bg();
	if (edit)
		edit_render(in, out_proj, m, n, d, screenWidth, screenHeight);
	else
		prev_render(in, out, out_proj, m, n, d,
				screenWidth, screenHeight);
	bt_edit.changePosition(screenWidth / 2 - 132, screenHeight / 2 - 36);
	bt_edit.Render();
	usleep(10000); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
	shift |= (key == 212 || key == 213);
	ctrl |= (key == 214);
	if (key == 'e') {
		edit = !edit;
		if (edit)
			bt_edit.changeText("VISUALIZAR");
		else {
			bt_edit.changeText("EDITAR");
			prev_update(in, out, out_proj, m, n, d);
		}
	}
	if (!edit)
		prev_check_keypress(key, shift, ctrl);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
	shift &= (key != 212 && key != 213);
	ctrl &= (key != 214);
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
	if (edit)
		edit_check_mouse(smouse, in, &m, &n, d, shift, ctrl);
	else
		prev_check_mouse(smouse);
	bt_edit.checkClick(smouse);
	if (bt_edit.isPressed()) {
		edit = !edit;
		if (edit) {
			bt_edit.changeText("VISUALIZAR");
		} else {
			bt_edit.changeText("EDITAR");
			prev_update(in, out, out_proj, m, n, d);
		}
	}
}

int main(void)
{
	bt_edit.changeText("EDITAR");
	edit_init(in, m, n, d);
	prev_init();
	prev_update(in, out, out_proj, m, n, d);
	CV::init(&screenWidth, &screenHeight, "Trabalho 4 - Guilherme Einloft");
	CV::run();
}
