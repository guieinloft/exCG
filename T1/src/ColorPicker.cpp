#include "ColorPicker.h"
#include "Slider.h"
#include "colors.h"
#include "gl_canvas2d.h"

ColorPicker::ColorPicker(int x, int y) {
    this->x = x;
    this->y = y;
    for (int i = 0; i < 4; i++) {
        sliders[i] = new Slider(x, y + 32 + 40 * i);
    }
    buttons[0] = new Button(x, y + 192, 128, 32);
    buttons[1] = new Button(x + 128, y + 192, 128, 32);

    colors[0].r = 0;
    colors[0].g = 0;
    colors[0].b = 0;
    colors[0].a = 0;
    colors[1].r = 255;
    colors[1].g = 255;
    colors[1].b = 255;
    colors[1].a = 255;
    currentColor = 0;
    buttons[0]->select(true);
}

ColorPicker::~ColorPicker() {
    for (int i = 0; i < 4; i++) {
        delete sliders[i];
    }
    delete buttons[0];
    delete buttons[1];
}

void ColorPicker::checkMouse(Mouse mouse) {
    sliders[0]->changeParam(colors[currentColor].r);
    sliders[1]->changeParam(colors[currentColor].g);
    sliders[2]->changeParam(colors[currentColor].b);
    sliders[3]->changeParam(colors[currentColor].a);
    for (int i = 0; i < 4; i++) {
        sliders[i]->checkMouse(mouse);
    }
    colors[currentColor].r = sliders[0]->getParam();
    colors[currentColor].g = sliders[1]->getParam();
    colors[currentColor].b = sliders[2]->getParam();
    colors[currentColor].a = sliders[3]->getParam();

    if (buttons[0]->checkClick(mouse) == 1) {
        buttons[0]->select(true);
        buttons[1]->select(false);
        currentColor = 0;
    }
    if (buttons[1]->checkClick(mouse) == 1) {
        buttons[1]->select(true);
        buttons[0]->select(false);
        currentColor = 1;
    }
    sliders[0]->changeParam(colors[currentColor].r);
    sliders[1]->changeParam(colors[currentColor].g);
    sliders[2]->changeParam(colors[currentColor].b);
    sliders[3]->changeParam(colors[currentColor].a);
}

void ColorPicker::Render() {
    for (int i = 0; i < 4; i++) {
        sliders[i]->Render();
    }
    for (int i = 0; i < 2; i++) {
        CV::color(colors[i].r/255.0, colors[i].g/255.0, colors[i].b/255.0);
        CV::rectFill(x + i * 128, y + 192, x + 64 + i * 128, y + 224);
        CV::color(colors[i].r/255.0*colors[i].a/255.0, colors[i].g/255.0*colors[i].a/255.0, colors[i].b/255.0*colors[i].a/255.0);
        CV::rectFill(x + 64 + i * 128, y + 192, x + 128 + i * 128, y + 224);
    }
    
    CV::color(0.5, 0.5, 0.5);
    CV::rect(x - 1 + 128 * currentColor, y + 191, x + 129 + 128 * currentColor, y + 225);
    CV::rect(x - 2 + 128 * currentColor, y + 190, x + 130 + 128 * currentColor, y + 226);
    
    //buttons[0]->Render();
    //buttons[1]->Render();
}

rgb_color *ColorPicker::getFGColor() {
    return &colors[0];
}

rgb_color *ColorPicker::getBGColor() {
    return &colors[1];
}

void ColorPicker::changeCurColor(rgb_color color) {
    colors[currentColor] = color;
}

void ColorPicker::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    for (int i = 0; i < 4; i++) {
        sliders[i]->changePosition(x, y + 32 + 40 * i);
    }
    buttons[0]->changePosition(x, y + 192);
    buttons[1]->changePosition(x + 128, y + 192);
}
