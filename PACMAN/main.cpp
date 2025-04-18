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
//  Instru��es:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gl_canvas2d.h"

#include "Pacman.h"
#include "ghosts/Ghost.h"
#include "ghosts/Blinky.h"
#include "ghosts/Inky.h"
#include "ghosts/Pinky.h"
#include "ghosts/Clyde.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

Pacman *pac = NULL;
Blinky *blinky = NULL;
Inky *inky = NULL;
Pinky *pinky = NULL;
Clyde *clyde = NULL;

char map[32][30];

char str_pontos[8];

void read_map(char map[32][30]) {
    FILE *fmap = fopen("map.txt", "r");
    if (fmap == NULL) {
        printf("fudeu\n");
        return;
    }
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 30; j++) {
            do {
                map[i][j] = fgetc(fmap);
            } while (map[i][j] == '\n');
        }
    }
    fclose(fmap);
}

void render_map(char map[32][30]) {
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 30; j++) {
            if (map[i][j] == '|') {
                CV::color(0, 0, 1);
                CV::rectFill(j * 16, i * 16, j * 16 + 16, i * 16 + 16);
            }
        }
    }
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 30; j++) {
            if (map[i][j] != '|' && map[i][j] != '_') {
                CV::color(0, 0, 0);
                CV::rectFill(j * 16 - 8, i * 16 - 8, j * 16 + 24, i * 16 + 24);
            }
        }
    }
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 30; j++) {
            if (map[i][j] == '.') {
                CV::color(1, 0.8, 0.6);
                CV::circleFill(j * 16 + 8, i * 16 + 8, 2, 4);
            }
            else if (map[i][j] == '+') {
                CV::color(1, 0.8, 0.6);
                CV::circleFill(j * 16 + 8, i * 16 + 8, 4, 8);
            }
            else if (map[i][j] == '-') {
                CV::color(0.5, 0.5, 0.5);
                CV::rectFill(j * 16 - 8, i * 16 + 8, j * 16 + 24, i * 16 + 16);
            }
        }
    }
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
    render_map(map);
    pac->pac_render();
    blinky->Render();
    inky->Render();
    pinky->Render();
    clyde->Render();
    CV::color(0, 0, 0, 1);
    CV::rectFill(0, 224, 32, 272);
    CV::rectFill(448, 224, 512, 272);
    CV::color(0, 0, 0, 0.5);
    CV::rectFill(32, 224, 48, 272);
    CV::rectFill(432, 224, 448, 272);

    CV::color(1, 1, 1);
    CV::text(12, 12, "PONTOS:");
    sprintf(str_pontos, "%d", pac->get_score());
    CV::text(96, 12, str_pontos);

    usleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
    CV::clear(0, 0, 0);
    pac->move(map);
    blinky->move(map, pac);
    inky->move(map, pac, blinky);
    pinky->move(map, pac);
    clyde->move(map, pac);
    
    int super = pac->get_super();
    if (super == 300) {
        blinky->set_state(2);
        inky->set_state(2);
        pinky->set_state(2);
        clyde->set_state(2);
    }
    else if (super == 1) {
        printf("\nACABOU");
        blinky->set_state(0);
        inky->set_state(0);
        pinky->set_state(0);
        clyde->set_state(0);
        pac->set_invincibility(INVINCIBILITY);
    }
    if (pac->get_state() == -1) {
        if (pac->get_lives() > 0) {
            sleep(1);
            pac->reset(15, 24, false);
        }
        else {
            CV::text(300, 12, "PRESSIONE ENTER");
        }
    }
    else {
        CV::text(300, 12, "VIDAS:");
        sprintf(str_pontos, "%d", pac->get_lives());
        CV::text(372, 12, str_pontos);
    }
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    pac->handle_input(key);
    if (key == 13) {
        read_map(map);
        pac->reset(15, 24, true);
        blinky->reset(15, 12);
        blinky->set_times(420, 1200, 420, 1200, 300, 1200, 300);
        inky->reset(13, 15);
        inky->set_times(420, 1200, 420, 1200, 300, 1200, 300);
        pinky->reset(15, 15);
        pinky->set_times(420, 1200, 420, 1200, 300, 1200, 300);
        clyde->reset(17, 15);
        clyde->set_times(420, 1200, 420, 1200, 300, 1200, 300);
    }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

}

int main(void)
{
    pac = new Pacman(15, 24);
    blinky = new Blinky(15, 12, 29, -2);
    blinky->set_times(420, 1200, 420, 1200, 300, 1200, 300);
    inky = new Inky(13, 15, 29, 33);
    inky->set_times(420, 1200, 420, 1200, 300, 1200, 300);
    pinky = new Pinky(15, 15, 0, -2);
    pinky->set_times(420, 1200, 420, 1200, 300, 1200, 300);
    clyde = new Clyde(17, 15, 0, 33);
    clyde->set_times(420, 1200, 420, 1200, 300, 1200, 300);
    read_map(map);
    CV::init(&screenWidth, &screenHeight, "PAC-MAN");
    CV::run();
}
