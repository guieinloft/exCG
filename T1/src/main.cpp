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
#include "Button.h"
#include "ToolList.h"
#include "LayerList.h"
#include "Canvas.h"
#include "Mouse.h"
#include "ColorPicker.h"
#include "colors.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

Mouse smouse; //variaveis globais do mouse para poder exibir dentro da render().

ToolList *tool_list;
LayerList *layer_list;
ColorPicker *color_picker;
Canvas *canvas;

uint8_t active_layer;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
    canvas->Render(screenWidth, screenHeight, layer_list->getLayers(), layer_list->getNLayers());
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(screenWidth-272, 0, screenWidth, screenHeight);
    tool_list->Render();
    color_picker->Render();
    color_picker->changePosition(screenWidth - 272, 0);
    layer_list->RenderList();
    layer_list->changePosition(screenWidth - 264, 272);

   usleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    smouse.xp = smouse.x;
    smouse.yp = smouse.y;
    smouse.x = x;
    smouse.y = y;
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

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
   tool_list->checkMouse(smouse, canvas, layer_list->getActiveLayer(), color_picker->getFGColor(), color_picker->getBGColor());
   color_picker->checkMouse(smouse);
   layer_list->checkMouse(smouse);
}

int main(void)
{
   tool_list = new ToolList();
   canvas = new Canvas(640, 480);
   color_picker = new ColorPicker(screenWidth - 272, 0);
   layer_list = new LayerList(screenWidth - 264, 272);
   CV::init(&screenWidth, &screenHeight, "PROJETO EM BRANCO");
   CV::run();
}
