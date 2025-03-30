#ifndef BUTTON_H
#define BUTTON_H

#include "Image.h"
#include "Mouse.h"
#include <stdlib.h>

class Button {
    public:
    Button(int x, int y, int w, int h);
    int checkClick(Mouse mouse);
    void Render();
    bool isPressed();
    bool isSelected();
    void select(bool s);
    void changeIcon();
    void loadIcons(char *path1, char *path2 = NULL);
    void changeText(char *text);
    void changePosition(int x, int y);

    protected:
    int x;
    int y;
    int w;
    int h;
    Image icon1;
    Image icon2;
    bool iconCur;
    char text[256];
    bool selected;
    bool pressed;
    bool hovered;
};

#endif
