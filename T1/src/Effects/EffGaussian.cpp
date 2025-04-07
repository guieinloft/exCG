#include "EffGaussian.h"
#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "../colors.h"
#include "../gl_canvas2d.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159
#endif
#ifndef M_E
#define M_E 2.71828
#endif

EffGaussian::EffGaussian(int x, int y) : Effect(x, y) {
    sl_radius = new Slider(x, y + 64);
}

EffGaussian::~EffGaussian() {
    delete btOK;
    delete btCancel;
    delete sl_radius;
}

void EffGaussian::apply(Image *image) {
    image->blur(params[0]);
}

void EffGaussian::renderMenu() {
    btOK->Render();
    btCancel->Render();
    CV::text(x, y + 16, "Desfoque Gaussiano");
    CV::text(x, y + 48, "Raio:");
    sl_radius->Render();
}

bool EffGaussian::checkMouse(Mouse mouse, Image *image) {
    sl_radius->checkMouse(mouse);
    params[0] = (int)sl_radius->getParam();
    if (btOK->checkClick(mouse) == 1) {
        apply(image);
        params[0] = 0;
        sl_radius->changeParam(0);
        return true;
    }
    else if (btCancel->checkClick(mouse) == 1)
        return true;
    return false;
}

void EffGaussian::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    btOK->changePosition(x + 16, y + 255 - 48);
    btCancel->changePosition(x + 142, y + 255 - 48);
    sl_radius->changePosition(x, y + 64);
}
