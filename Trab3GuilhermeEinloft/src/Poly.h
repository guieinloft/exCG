#ifndef POLY__H
#define POLY__H

#include "Vector2.h"
#include "gl_canvas2d.h"

class Poly {
public:
	//array de pontos
	Vector2 *v;
	//tamanho
	int size;
	//chama polygonFill
	void fill();
	//rotaciona a partir de um pivo
	void rot(double angle, Vector2 pivot);
	//desloca a partir de um vetor direcao
	void move(Vector2 dir);
	//cria poligono regular com [size] lados
	void createShape(Vector2 center, float radius, int size);
};


#endif
