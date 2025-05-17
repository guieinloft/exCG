#include "Game.h"
#include "collisions.h"
#include "Tank.h"
#include "Barrel.h"
#include "Chaser.h"
#include "Tower.h"
#include "Powerup.h"

#include <stdlib.h>

#define GENERATE_INTERVAL 1000

Game::Game() {
	mouse.state = -2;
	paused = 1;
	lost = 0;
	won = 0;
	score = 0;
}

void Game::loadLevel(const char *path, bool hard) {
	FILE *level = fopen(path, "r");
	Vector2 points_out[12], points_in[12];
	for (int i = 0; i < 12; i++) {
		fscanf(level, "%f %f %f %f", &points_out[i].x, &points_out[i].y,
			&points_in[i].x, &points_in[i].y);
	}
	b_spline_generate(points_out, 12, b_out);
	b_spline_generate(points_in, 12, b_in);
	EntityType type;
	Vector2 center;
	for (int i = 0; i < ENTITY_FIXED_NUM; i++) {
		if (fscanf(level, "%d", &type) == EOF)
			break;
		fscanf(level, "%f %f", &center.x, &center.y);
		switch (type) {
		case ETYPE_TANK:
			entities[i] = new Tank(center);
			tank = (Tank*)entities[i];
			break;
		case ETYPE_BARREL:
			entities[i] = new Barrel(center, 24);
			break;
		case ETYPE_CHASER:
			entities[i] = new Chaser(center, hard);
			break;
		case ETYPE_TOWER:
			entities[i] = new Tower(center, hard);
			break;
		case ETYPE_POWERUP_SUPER:
			entities[i] = new Powerup(center, PWUP_SUPER);
			break;
		case ETYPE_POWERUP_SHIELD:
			entities[i] = new Powerup(center, PWUP_SHIELD);
			break;
		case ETYPE_POWERUP_HEALTH:
			entities[i] = new Powerup(center, PWUP_HEALTH);
			break;
		}
	}
	fclose(level);
	generateEntities();
	entity_num = 32;
}

void Game::update() {
	deltaTime = fp.getDeltaTime();
	CV::color(0.5, 0.5, 0.5);
	CV::rectFill(0, 0, 1280, 720);
	b_spline_draw(b_out, b_in);
	CV::color(1, 1, 1);
	char score_str[20];
	sprintf(score_str, "PONTOS: %d", score * 100);
	CV::text(10, 64, score_str);
	if (paused) {
		CV::text(10, 48, "PAUSADO");
		return;
	} else if (lost) {
		CV::text(10, 48, "VOCE PERDEU!");
		return;
	} else if (won) {
		CV::text(10, 48, "VOCE GANHOU!");
		return;
	}
	for (int i = 0; i < ENTITY_NUM; i++) {
		if (entities[i] == nullptr)
			continue;
		entities[i]->render();
		if (entities[i]->move(tank->getCenter(), deltaTime, entities)) {
			delete entities[i];
			entities[i] = nullptr;
		}
	}
	checkCollisions(entities, &score, &entity_num);
	checkCollisionsBorder(entities, b_out, b_in, &score, &entity_num);
	if (checkTankCollisions(tank, entities, &score, &entity_num)) {
		//lost = true;
	}
	if (int dir = collideTankBorder(tank, b_out, b_in)) {
		if (tank->hit_border(dir)) {
			//lost = true;
		}
	}
	if (entity_num == 1)
		won = true;
}

void Game::pressKey(int key) {
	switch (key) {
		case 200:
			keys.k_left = true;
			break;
		case 201:
			keys.k_up = true;
			break;
		case 202:
			keys.k_right = true;
			break;
		case 203:
			keys.k_down = true;
			break;
		case 'a':
			keys.k_left = true;
			break;
		case 'w':
			keys.k_up = true;
			break;
		case 'd':
			keys.k_right = true;
			break;
		case 's':
			keys.k_down = true;
			break;
		case 13:
			paused = !paused;
			break;
	}
	tank->checkKeys(keys);
}

void Game::releaseKey(int key) {
	switch (key) {
		case 200:
			keys.k_left = false;
			break;
		case 201:
			keys.k_up = false;
			break;
		case 202:
			keys.k_right = false;
			break;
		case 203:
			keys.k_down = false;
			break;
		case 'a':
			keys.k_left = false;
			break;
		case 'w':
			keys.k_up = false;
			break;
		case 'd':
			keys.k_right = false;
			break;
		case 's':
			keys.k_down = false;
			break;
	}
	tank->checkKeys(keys);
}

void Game::checkMouse(int button, int state, int wheel, int direction, int x, int y) {
	mouse.xp = mouse.x;
	mouse.yp = mouse.y;
	mouse.x = x;
	mouse.y = y;
	mouse.button = button;
	mouse.state = state;
	mouse.wheel = wheel;
	mouse.direction = direction;
	switch (button) {
		case 0:
		mouse.l = !state;
		break;
		case 1:
		mouse.m = !state;
		break;
		case 2:
		mouse.r = !state;
		break;
	}
	tank->checkMouse(mouse, entities);
}

bool Game::checkValidPosition(int i) {
	if (checkCollisionBorder(entities[i], b_out, b_in))
		return false;

	for (int j = 0; j < i; j++) {
		if (entities[j] == nullptr)
			continue;
		if (checkCollision(entities[i], entities[j])) {
			return false;
		}
	}
	return true;
}

void Game::generateEntities() {
	bool valid_position = true;
	int init = 0;
	while (init < ENTITY_FIXED_NUM) {
		if (entities[init] == nullptr)
			break;
		init++;
	}
	for (int i = init; i < ENTITY_FIXED_NUM; i++) {
		do {
			if (entities[i] != nullptr)
				delete entities[i];
			float barrelx, barrely;
			barrelx = (float)(rand() % 1280);
			barrely = (float)(rand() % 720);
			entities[i] = new Barrel((Vector2){barrelx, barrely}, 24 + (rand() & 3));
			valid_position = checkValidPosition(i);
		} while (!valid_position);
	}
}
