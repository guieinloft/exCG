#include "Button.h"
#include "Slider.h"
#include "gl_canvas2d.h"

Slider::Slider(int x, int y) {
    this->x = x;
    this->y = y;
    this->param = 0;
    this->button = new Button(x - 4, y - 15, 8, 32);
}

Slider::~Slider() {
    delete button;
}

void Slider::checkMouse(Mouse mouse) {
    int bt_status = button->checkClick(mouse);
    switch (bt_status) {
        case 1:
        button->select(true);
        break;
        case 0:
        button->select(false);
        break;
        default:
        break;
    }
    if (button->isSelected()) {
        if (mouse.x < this->x) 
            param = 0;
        else if (mouse.x > this->x + 255)
            param = 255;
        else param = mouse.x - this->x;
    }
}

void Slider::Render() {
    button->changePosition(x - 4 + param, y - 15);
    CV::color(0.5, 0.5, 0.5);
    CV::rectFill(x, y, x + 255, y + 1);
    button->Render();
}

void Slider::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
}

uint8_t Slider::getParam() {
    return param;
}

void Slider::changeParam(uint8_t new_param) {
    this->param = new_param;
}

bool Slider::isSelected() {
    return button->isSelected();
}
