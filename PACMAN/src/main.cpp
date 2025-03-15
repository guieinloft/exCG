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

#include "Pacman.h"
#include "ghosts/Ghost.h"
#include "ghosts/Blinky.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

Pacman *pac = NULL;
Blinky *blinky = NULL;

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
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
    render_map(map);
    blinky->Render();
    pac->pac_render();

    CV::color(1, 1, 1);
    CV::text(12, 12, "PONTOS:");
    sprintf(str_pontos, "%d", pac->get_score());
    CV::text(96, 12, str_pontos);

    usleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
    CV::clear(0, 0, 0);
    pac->move(map);
    blinky->move(map, pac);

    if (pac->get_super()) {
        printf("\nSUPER");
    }
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
   pac->handle_input(key);
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

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

}

int main(void)
{
    pac = new Pacman(15, 24);
    blinky = new Blinky(15, 12, 29, -2);
    read_map(map);
    CV::init(&screenWidth, &screenHeight, "PAC-MAN");
    CV::run();
}
