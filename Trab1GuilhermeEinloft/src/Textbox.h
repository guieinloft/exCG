#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Button.h"
#include "Mouse.h"

class Textbox {
    public:
    Textbox(int x, int y);
    void checkMouse(Mouse mouse);
    bool checkKeyboard(int key);
    void Render();
    void changePosition(int x, int y);
    void reset();
    char *getText();

    private:
    char text[256];
    int current_char;
    int x, y;
    Button *button;
};

#endif
