#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "Button.h"
#include "Textbox.h"
#include "Mouse.h"

enum TSScreen {
	TS_EXIT = 0,
	TS_MAIN,
	TS_PLAY,
	TS_EDIT,
	TS_DIFF
};

class TitleScreen {
protected:
	Button *bt_play;
	Button *bt_edit;
	Button *bt_back;

	Button *bt_level1;
	Button *bt_level2;
	Button *bt_level3;

	Textbox *tx_level_name;
	Button *bt_custom_level;

	Button *bt_easy;
	Button *bt_hard;

	Mouse mouse;

	TSScreen curScreen;

	char levelpath[512];
	char errortext[512];
	bool difficulty;
	
	int checkMouseMAIN();
	int checkMousePLAY();
	int checkMouseEDIT();
	int checkMouseDIFF();

	Image img_title;

public:
	TitleScreen();
	void render();
	void pressKey(int key);
	int checkMouse(int button, int state, int wheel, int direction, int x, int y);
	char *getLevelPath();
	bool getDifficulty();
	void reset();
};

#endif
