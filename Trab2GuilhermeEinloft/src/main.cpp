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
#include "Image.h"
#include "Slider.h"
#include "Mouse.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

Mouse smouse;

Image *img;
Image *img_2;

Slider *q_sliders[8];
uint8_t quality[8] = {1, 1, 1, 1, 1, 1, 1, 1};
Button *apply;

const char path[] = "Trab2GuilhermeEinloft/images/jellybeans.bmp";

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
    CV::color(1, 1, 1);
    img->render(0, 0, 255);
    img_2->render(img->get_w(), 0, 255);
    for (int i = 0; i < 8; i++) {
        q_sliders[i]->Render();
        char q_text[8];
        sprintf(q_text, "%d", quality[i]);
        CV::color(0, 0, 0);
        CV::text(i * 48, img->get_h() + 160, q_text);
    }
    apply->Render();

   usleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
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
    for (int i = 0; i < 8; i++) {
        q_sliders[i]->checkMouse(smouse);
        quality[i] = 1 + (q_sliders[i]->getParam() >> 1);
    }
    int bt_status = apply->checkClick(smouse);
    if (bt_status == 1) {
        apply->select(true);
        img_2->copy(img);
        img_2->compress(quality);
    }
    else if (bt_status == 0) {
        apply->select(false);
    }
}

int main(void)
{
    img = new Image();
    img_2 = new Image();
    img->bmp_load(path);
    img_2->copy(img);
    for (int i = 0; i < 8; i++) {
        q_sliders[i] = new Slider(16 + 272 * (i >> 2), img->get_h() + 16 + (i % 4) * 32);
    }
    apply = new Button(16, img->get_h() + 172, 128, 32);
    apply->changeText("APLICAR");
   CV::init(&screenWidth, &screenHeight, "PROJETO EM BRANCO");
   CV::run();
}
