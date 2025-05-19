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
#include <time.h>

#include "gl_canvas2d.h"
#include "collisions.h"

#include "Game.h"
#include "TitleScreen.h"
#include "Editor.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1280, screenHeight = 720;

Game game;
TitleScreen title;
Editor editor;

int curScreen = 0;
int curScreen_prev = 0;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
	curScreen_prev = curScreen;
	switch (curScreen) {
	case 0:
		title.render();
		break;
	case 1:
		curScreen = !(game.update());
		break;
	case 2:
		curScreen = 2 * !(editor.update());
		break;
	}
	//sleep(1); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
	switch (curScreen) {
	case 0:
		title.pressKey(key);
		break;
	case 1:
		game.pressKey(key);
		break;
	}
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
	switch (curScreen) {
	case 1:
		game.releaseKey(key);
		break;
	}
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
	curScreen_prev = curScreen;
	switch (curScreen) {
	case 0:
		curScreen = title.checkMouse(button, state, wheel, direction, x, y);
		if (curScreen == 1) {
			game.loadLevel(title.getLevelPath(), title.getDifficulty());
			title.reset();
		}
		if (curScreen == 2) {
			editor.loadLevel(title.getLevelPath());
			title.reset();
		}
		break;
	case 1:
		game.checkMouse(button, state, wheel, direction, x, y);
		break;
	case 2:
		editor.checkMouse(button, state, wheel, direction, x, y);
		break;
	}
}

int main(void)
{
	srand(time(0));
	CV::init(&screenWidth, &screenHeight, "PROJETO EM BRANCO");
	CV::run();
}
