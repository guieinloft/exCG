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

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
bool mouseL = 0, mouseR = 0;

Image *img;
float img_x = 0, img_y = 0, img_sx = 1, img_sy = 1;
int diameter = 5; //diametro do pincel
int resampling = 128; //número de vezes que o pincel é desenhado por frame
uint8_t pr = 0, pg = 0, pb = 0, po = 255, opacity = 255;

void render_pattern() {
    for (int i = 0; i < screenHeight/8; i++) {
        for (int j = 0; j < screenWidth/8; j++) {
            if ((i + j) & 1) {
                CV::color(0.5, 0.5, 0.5);
            }
            else {
                CV::color(0.75, 0.75, 0.75);
            }
            CV::rectFill(8 * j, 8 * i, 8 * j + 8, 8 * i + 8);
        }
    }
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
   render_pattern();
   img->render_scaled(img_x, img_y, img_sx * img->get_w(), img_sy * img->get_h(), opacity);
   //img->render(img_x, img_y);
   
   usleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   //printf("\nTecla: %d" , key);
   switch (key) {
       case 200:
       img_x -= 8;
       break;

       case 201:
       img_y -= 8;
       break;

       case 202:
       img_x += 8;
       break;

       case 203:
       img_y += 8;
       break;

       case 204:
       img_sx *= 2;
       img_sy *= 2;
       break;

       case 205:
       img_sx /= 2;
       img_sy /= 2;
       break;

       case 120:
       img->flip_h();
       break;

       case 122:
       img->flip_v();
       break;

       case 13:
       img->resize((img->get_w() * img_sx), (img->get_h() * img_sy));
       printf("\nNOVAS DIMENSÕES: %dx%d", img->get_w(), img->get_h());
       img_sx = 1;
       img_sy = 1;
       break;

       case 43:
       diameter++;
       break;

       case 45:
       diameter--;
       break;

       case 114:
       pr += 32;
       break;

       case 103:
       pg += 32;
       break;

       case 98:
       pb += 32;
       break;

       case 'o':
       po -= 32;
       break;

       case 'O':
       opacity -= 32;
       break;
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   //printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
   if (button == 0) mouseL = !state;
   else if (button == 2) mouseR = !state;
   for (int i = 0; i < resampling; i++) {
       if (mouseL) img->paint_circle((x * (resampling - i) + mouseX * i) / resampling - img_x, (y * (resampling - i) + mouseY * i) / resampling - img_y, diameter, pr, pg, pb, po, false);
       else if (mouseR) img->paint_circle((x * (resampling - i) + mouseX * i) / resampling - img_x, (y * (resampling - i) + mouseY * i) / resampling - img_y, diameter, pr, pg, pb, 0, false);
   }

   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;
}

int main(int argc, char *argv[])
{
   if (argc < 2) {
       printf("Por favor digite a imagem como argumento!\n");
       return 0;
   }

   bool a = 254;
   bool b = 255;

   printf("\n%d %d\n\n", a, b);

   img = new Image();
   img->bmp_load(argv[1]);
   CV::init(&screenWidth, &screenHeight, "PROJETO EM BRANCO");
   CV::run();
   img->close_image();
   return 0;
}
