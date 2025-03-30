#include "ColorPicker.h"
#include "Slider.h"
#include "colors.h"
#include "gl_canvas2d.h"

ColorPicker::ColorPicker(int x, int y) {
    this->x = x;
    this->y = y;
    for (int i = 0; i < 4; i++) {
        sliders[i] = new Slider(x + 8, y + 40 + 40 * i);
    }
    buttons[0] = new Button(x + 8, y + 200, 128, 32);
    buttons[1] = new Button(x + 136, y + 200, 128, 32);

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

void ColorPicker::checkMouse(Mouse mouse) {
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
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(x, y, x + 272, y + 272);
    for (int i = 0; i < 4; i++) {
        sliders[i]->Render();
    }
    for (int i = 0; i < 2; i++) {
        CV::color(colors[i].r/255.0, colors[i].g/255.0, colors[i].b/255.0);
        CV::rectFill(x + 8 + i * 128, y + 200, x + 72 + i * 128, y + 232);
        CV::color(colors[i].r/255.0*colors[i].a/255.0, colors[i].g/255.0*colors[i].a/255.0, colors[i].b/255.0*colors[i].a/255.0);
        CV::rectFill(x + 72 + i * 128, y + 200, x + 136 + i * 128, y + 232);
    }
    
    CV::color(0.5, 0.5, 0.5);
    CV::rect(x + 7 + 128 * currentColor, y + 199, x + 137 + 128 * currentColor, y + 233);
    CV::rect(x + 6 + 128 * currentColor, y + 198, x + 138 + 128 * currentColor, y + 234);
    
    //buttons[0]->Render();
    //buttons[1]->Render();
}

rgb_color ColorPicker::getFGColor() {
    return colors[0];
}

rgb_color ColorPicker::getBGColor() {
    return colors[1];
}

void ColorPicker::changeCurColor(rgb_color color) {
    colors[currentColor] = color;
}

void ColorPicker::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    for (int i = 0; i < 4; i++) {
        sliders[i]->changePosition(x + 8, y + 40 + 40 * i);
    }
    buttons[0]->changePosition(x + 8, y + 200);
    buttons[1]->changePosition(x + 136, y + 200);
}
