#ifndef B_SPLINE_H
#define B_SPLINE_H

#include "Vector2.h"

//numero de segmentos para aproximar a curva
#define B_SEG 256
#define B_MASK 255 //B_SEG - 1
//usado para executar "& B_MASK" ao inves de "% B_SEG"

//gera a aproximacao da curva a partir dos pontos de controle
void b_spline_generate(Vector2 points[], int n_points, Vector2 b_spline[B_SEG]);
//desenha a curva como eh apresentada no jogo
void b_spline_draw(Vector2 b_out[B_SEG], Vector2 b_in[B_SEG]);
//desenha os pontos de controle e as ligacoes entre eles
void b_spline_points_draw(Vector2 b_out[B_SEG], Vector2 b_in[B_SEG]);

#endif
