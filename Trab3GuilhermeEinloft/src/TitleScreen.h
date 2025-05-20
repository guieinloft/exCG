#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "Button.h"
#include "Textbox.h"
#include "Mouse.h"

//telas do menu
enum TSScreen {
	TS_MAIN,
	TS_PLAY,
	TS_EDIT,
	TS_DIFF
};

class TitleScreen {
protected:
	//botoes da tela principal
	Button *bt_play;
	Button *bt_edit;
	Button *bt_back;

	//botoes da tela de jogo
	Button *bt_level1;
	Button *bt_level2;
	Button *bt_level3;

	//botoes da tela de jogo e de edicao
	Textbox *tx_level_name;
	Button *bt_custom_level;

	//botoes da tela de dificuldade do jogo
	Button *bt_easy;
	Button *bt_hard;

	//mouse
	Mouse mouse;

	//tela atual
	TSScreen curScreen;

	//caminho da fase
	char levelpath[512];
	//texto de erro
	char errortext[512];
	//dificuldade
	bool difficulty;
	
	//checagem de mouse para os botoes de cada tela
	int checkMouseMAIN();
	int checkMousePLAY();
	int checkMouseEDIT();
	int checkMouseDIFF();

	//imagem do titulo do jogo
	Image img_title;

public:
	TitleScreen();
	//renderizador
	void render();
	//tratador de tecla
	void pressKey(int key);
	//tratador de mouse (retorna 0 para continuar, 1 ou 2 para sair)
	int checkMouse(int button, int state, int wheel, int direction, int x, int y);
	//pega caminho da fase a ser carregada
	char *getLevelPath();
	//pega dificuldade da fase (jogo)
	bool getDifficulty();
	//reseta a textbox
	void reset();
};

#endif
