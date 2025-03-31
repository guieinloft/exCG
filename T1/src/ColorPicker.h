#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

#include "colors.h"
#include "Slider.h"
#include "Button.h"

class ColorPicker {
    public:
    ColorPicker(int x, int y);
    void Render();
    void checkMouse(Mouse mouse);
    rgb_color *getFGColor();
    rgb_color *getBGColor();
    void changeCurColor(rgb_color color);
    void changePosition(int x, int y);

    private:
    int x;
    int y;
    rgb_color colors[2];
    bool currentColor;
    Slider *sliders[4];
    Button *buttons[2];
};

#endif
