#include "Effect.h"
#include "../Image.h"
#include "../Button.h"

Effect::Effect(int x, int y) {
    this->x = x;
    this->y = y;
    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
    params[3] = 0;
    btOK = new Button(x + 16, y + 255 - 48, 96, 32);
    btCancel = new Button(x + 142, y + 255 - 48, 96, 32);
    btOK->changeText("OK");
    btCancel->changeText("CANCELAR");
}

Effect::~Effect() {
    delete btOK;
    delete btCancel;
}

void Effect::apply(Image *image) {
    (void)image;
}

void Effect::renderMenu() {
    btOK->Render();
    btCancel->Render();
}

bool Effect::checkMouse(Mouse mouse, Image *image) {
    if (btOK->checkClick(mouse) == 1) {
        apply(image);
        return true;
    }
    else if (btCancel->checkClick(mouse) == 1)
        return true;
    return false;
}

void Effect::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    btOK->changePosition(x + 16, y + 255 - 48);
    btCancel->changePosition(x + 142, y + 255 - 48);
}
