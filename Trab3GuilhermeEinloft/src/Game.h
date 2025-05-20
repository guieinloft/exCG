#ifndef GAME_H
#define GAME_H

#include "Entity.h"
#include "Tank.h"
#include "Mouse.h"
#include "Keys.h"
#include "FrameController.h"
#include "Vector2.h"
#include "b_spline.h"

class Game {
protected:
	//struct de mouse e teclas
	Mouse mouse;
	Keys keys;

	//array de entidades
	Entity *entities[ENTITY_NUM];
	//numero atual de entidades
	int entity_num;
	//ponteiro para o tanque (entities[0])
	Tank *tank;

	//controle de fps
	FrameController fp;
	float deltaTime;
	
	//pontos de controle das curvas b-spline
	Vector2 points_out[12];
	Vector2 points_in[12];
	//aproximacao das curvas b-spline
	Vector2 b_out[B_SEG];
	Vector2 b_in[B_SEG];

	//pontuacao
	int score;

	//pause
	bool paused;
	//perdeu
	bool lost;
	//ganhou
	bool won;
	//checa posicao do barril gerado com GenerateEntities()
	bool checkValidPosition(int i);

public:
	Game();
	//update
	//retorna 0 se continua, 1 se volta ao titulo
	int update();
	//tratador de tecla
	void pressKey(int key);
	//tratador de teclas soltas
	void releaseKey(int key);
	//tratador de mouse
	void checkMouse(int button, int state, int wheel, int direction, int x, int y);
	//carregador de fase (normal ou dificil)
	void loadLevel(const char *path, bool hard = false);
	//gerador aleatorio de barris
	void generateEntities();
};

#endif
