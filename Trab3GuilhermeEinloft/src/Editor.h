#ifndef EDITOR_H
#define EDITOR_H

#include "Button.h"
#include "Vector2.h"
#include "b_spline.h"
#include "Entity.h"

class Editor {
protected:
	Button *bt_add;
	Button *bt_edit;
	Button *bt_remove;
	Button *bt_save;
	Button *bt_back;

	Button *bt_add_entities[ETYPE_FIXED_NUM];

	Vector2 p_out[12];
	Vector2 p_in[12];
	Vector2 b_out[B_SEG];
	Vector2 b_in[B_SEG];
	int curPointOuter;
	int curPointInner;

	Entity *entities[ENTITY_FIXED_NUM];
	bool collisions[ENTITY_FIXED_NUM];

	char *levelpath;
	char errortext[256];

	Mouse mouse;

	int curMenu;
	int update_main();
	int update_add();
	int update_edit();
	int update_remove();

	int curEntity;

	void addEntity();
	bool moveEntity();
	bool movePointOuter();
	bool movePointInner();
	bool removeEntity();

public:
	Editor();
	void loadLevel(char *path);
	void saveLevel();
	int update();
	void checkMouse(int button, int state, int wheel, int direction, int x, int y);
	void pressKey(int key);
};

#endif
