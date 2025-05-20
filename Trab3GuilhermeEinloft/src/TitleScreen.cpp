#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"
#include "TitleScreen.h"

bool checkValidPathPlay(const char *levelpath, char *levelpath2) {
	if (strcmp(levelpath, "") == 0)
		return 0;
	strcpy(levelpath2, "Trab3GuilhermeEinloft/levels/");
	strcat(levelpath2, levelpath);
	FILE *level = fopen(levelpath2, "r");
	if (level != NULL) {
		fclose(level);
		return 1;
	}
	return 0;
}

bool checkValidPathEdit(const char *levelpath, char *levelpath2) {
	if (strcmp(levelpath, "") == 0)
		return 0;
	strcpy(levelpath2, "Trab3GuilhermeEinloft/levels/");
	strcat(levelpath2, levelpath);
	return 1;
}

TitleScreen::TitleScreen() {
	curScreen = TS_MAIN;
	bt_play = new Button(576, 360, 128, 32);
	bt_play->changeText("JOGAR");
	bt_edit = new Button(576, 400, 128, 32);
	bt_edit->changeText("EDITAR");
	
	bt_level1 = new Button(576, 240, 128, 32);
	bt_level1->changeText("PISTA 1");
	bt_level2 = new Button(576, 280, 128, 32);
	bt_level2->changeText("PISTA 2");
	bt_level3 = new Button(576, 320, 128, 32);
	bt_level3->changeText("PISTA 3");
	
	tx_level_name = new Textbox(512, 400);
	bt_custom_level = new Button(768, 400, 32, 32);
	bt_custom_level->changeText("OK");

	bt_easy = new Button(576, 360, 128, 32);
	bt_easy->changeText("NORMAL");
	bt_hard = new Button(576, 400, 128, 32);
	bt_hard->changeText("DIFICIL");
	
	bt_back = new Button(16, 672, 128, 32);
	bt_back->changeText("VOLTAR");
	strcpy(errortext, "");

	img_title.bmp_load("Trab3GuilhermeEinloft/images/tankgame.bmp");
}

int TitleScreen::checkMouseMAIN() {
	if (bt_play->checkClick(mouse) == 1) {
		strcpy(errortext, "");
		curScreen = TS_PLAY;
	}
	else if (bt_edit->checkClick(mouse) == 1) {
		strcpy(errortext, "");
		curScreen = TS_EDIT;
	}
	else if (bt_back->checkClick(mouse) == 1)
		curScreen = TS_MAIN;

	return 0;
}

int TitleScreen::checkMousePLAY() {
	if (bt_level1->checkClick(mouse) == 1) {
		strcpy(levelpath, "Trab3GuilhermeEinloft/levels/base/level1.trk");
		curScreen = TS_DIFF;
	}
	else if (bt_level2->checkClick(mouse) == 1) {
		strcpy(levelpath, "Trab3GuilhermeEinloft/levels/base/level2.trk");
		curScreen = TS_DIFF;
	}
	else if (bt_level3->checkClick(mouse) == 1) {
		strcpy(levelpath, "Trab3GuilhermeEinloft/levels/base/level3.trk");
		curScreen = TS_DIFF;
	}
	else if (bt_custom_level->checkClick(mouse) == 1) {
		if (checkValidPathPlay(tx_level_name->getText(), levelpath)) {
			curScreen = TS_DIFF;
			strcpy(errortext, "");
		} else {
			strcpy(errortext, "Digite um nome de pista valido!");
		}
	}
	else if (bt_back->checkClick(mouse) == 1)
		curScreen = TS_MAIN;
	tx_level_name->checkMouse(mouse);

	return 0;
}

int TitleScreen::checkMouseEDIT() {
	if (bt_custom_level->checkClick(mouse) == 1) {
		if (checkValidPathEdit(tx_level_name->getText(), levelpath)) {
			strcpy(errortext, "");
			return 2;
		}
		strcpy(errortext, "Digite um nome de pista valido!");
		return 0;
	}
	else if (bt_back->checkClick(mouse) == 1)
		curScreen = TS_MAIN;
	tx_level_name->checkMouse(mouse);
	return 0;
}

int TitleScreen::checkMouseDIFF() {
	if (bt_easy->checkClick(mouse) == 1) {
		difficulty = 0;
		return 1;
	}
	else if (bt_hard->checkClick(mouse) == 1) {
		difficulty = 1;
		return 1;
	}
	else if (bt_back->checkClick(mouse) == 1)
		curScreen = TS_PLAY;

	return 0;
}

void TitleScreen::render() {
	img_title.render(512, 64, 255);
	switch (curScreen) {
	case TS_MAIN:
		bt_play->Render();
		bt_edit->Render();
		break;
	case TS_PLAY:
		bt_level1->Render();
		bt_level2->Render();
		bt_level3->Render();
		CV::text(512, 380, "FASE PERSONALIZADA");
		tx_level_name->Render();
		bt_custom_level->Render();
		CV::text(512, 460, errortext);
		break;
	case TS_EDIT:
		CV::text(512, 380, "FASE PERSONALIZADA");
		tx_level_name->Render();
		bt_custom_level->Render();
		CV::text(512, 460, errortext);
		break;
	case TS_DIFF:
		bt_easy->Render();
		bt_hard->Render();
		break;
	default:
		break;
	}
	bt_back->Render();
}

void TitleScreen::pressKey(int key) {
	if (curScreen == TS_PLAY || curScreen == TS_EDIT) {
		tx_level_name->checkKeyboard(key);
	}
}

int TitleScreen::checkMouse(int button, int state, int wheel, int direction,
		int x, int y) {
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
	switch (curScreen) {
	case TS_MAIN:
		return checkMouseMAIN();
	case TS_PLAY:
		return checkMousePLAY();
	case TS_EDIT:
		return checkMouseEDIT();
	case TS_DIFF:
		return checkMouseDIFF();
	}
	return 0;
}

char *TitleScreen::getLevelPath() {
	return levelpath;
}

bool TitleScreen::getDifficulty() {
	return difficulty;
}

void TitleScreen::reset() {
	tx_level_name->reset();
	curScreen = TS_MAIN;
}
