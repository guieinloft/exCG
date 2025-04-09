#ifndef SLIDER_H
#define SLIDER_H

#include "Button.h"

class Slider {
    public:
    Slider(int x, int y);
    void checkMouse(Mouse mouse);
    void Render();
    void changePosition(int x, int y);
    uint8_t getParam();
    void changeParam(uint8_t new_param);
    bool isSelected();

    protected:
    int x;
    int y;
    uint8_t param;
    Button *button;
};

#endif
