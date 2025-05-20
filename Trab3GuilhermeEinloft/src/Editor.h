#ifndef EDITOR_H
#define EDITOR_H

#include "Button.h"
#include "Vector2.h"
#include "b_spline.h"
#include "Entity.h"

class Editor {
protected:
	//botoes do menu principal
	Button *bt_add;
	Button *bt_edit;
	Button *bt_remove;
	Button *bt_save;
	Button *bt_back;

	//botao para adicionar cada entidade
	Button *bt_add_entities[ETYPE_FIXED_NUM];

	//pontos de controle da curva b-spline
	Vector2 p_out[12];
	Vector2 p_in[12];
	//aproximacao da curva b-spline
	Vector2 b_out[B_SEG];
	Vector2 b_in[B_SEG];
	//pontos atuais da curva externa e interna para edicao
	int curPointOuter;
	int curPointInner;

	//array de entidades
	Entity *entities[ENTITY_FIXED_NUM];
	//array de colisoes das entidades
	bool collisions[ENTITY_FIXED_NUM];

	//caminho da pista
	char *levelpath;
	//mensagem de erro
	char errortext[256];

	//mouse
	Mouse mouse;

	//menu atual
	int curMenu;
	//updates de cada menu
	int update_main();
	int update_add();
	int update_edit();
	int update_remove();

	//entidade atual
	int curEntity;

	//helpers para manipular as entidades
	void addEntity();
	bool moveEntity();
	bool removeEntity();
	
	//helpers para editar pontos de controle
	bool movePointOuter();
	bool movePointInner();

public:
	Editor();
	//carrega pista
	void loadLevel(char *path);
	//salva pista (so salva se nao houver colisoes entre as entidades)
	void saveLevel();
	//update (retorna 0 se continua, 1 se sai)
	int update();
	//tratador de mouse
	void checkMouse(int button, int state, int wheel, int direction, int x, int y);
};

#endif
