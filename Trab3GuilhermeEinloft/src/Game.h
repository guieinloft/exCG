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
	Mouse mouse;
	Keys keys;

	Entity *entities[ENTITY_NUM];
	int entity_num;
	Tank *tank;

	FrameController fp;
	float deltaTime;
	
	Vector2 b_out[B_SEG];
	Vector2 b_in[B_SEG];

	int score;

	bool paused;
	bool lost;
	bool won;
	bool checkValidPosition(int i);

public:
	Game();
	void update();
	void pressKey(int key);
	void releaseKey(int key);
	void checkMouse(int button, int state, int wheel, int direction, int x, int y);
	void loadLevel(const char *path, bool hard = false);
	void generateEntities();
};

#endif
