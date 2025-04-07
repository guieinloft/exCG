#include "EffGamma.h"
#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "../colors.h"
#include "../gl_canvas2d.h"

#include <stdio.h>

EffGamma::EffGamma(int x, int y) : Effect(x, y) {
    sl_gamma = new Slider(x, y + 64);
    sl_gamma->changeParam(128);
    params[0] = 128;
}

EffGamma::~EffGamma() {
    delete btOK;
    delete btCancel;
    delete sl_gamma;
}

void EffGamma::apply(Image *image) {
    int w = image->get_w();
    int h = image->get_h();
    float gamma = pow(2, params[0] / 128.0 * 3.0);
    gamma = 1.0 / gamma;
    rgb_color pixel;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image->get_pixel(j, i, &(pixel.r), &(pixel.g), &(pixel.b), &(pixel.a));
            pixel.r = 255 * pow(pixel.r / 255.0, gamma);
            pixel.g = 255 * pow(pixel.g / 255.0, gamma);
            pixel.b = 255 * pow(pixel.b / 255.0, gamma);
            image->put_pixel(j, i, pixel.r, pixel.g, pixel.b, pixel.a, false);
        }
    }
}

void EffGamma::renderMenu() {
    btOK->Render();
    btCancel->Render();
    CV::text(x, y + 16, "Correcao de Gama:");
    CV::text(x, y + 48, "Valor Gama:");
    sl_gamma->Render();
}

bool EffGamma::checkMouse(Mouse mouse, Image *image) {
    sl_gamma->checkMouse(mouse);
    params[0] = (int)sl_gamma->getParam() - 128;
    if (btOK->checkClick(mouse) == 1) {
        apply(image);
        params[0] = 128;
        sl_gamma->changeParam(128);
        return true;
    }
    else if (btCancel->checkClick(mouse) == 1)
        return true;
    return false;
}

void EffGamma::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    btOK->changePosition(x + 16, y + 255 - 48);
    btCancel->changePosition(x + 142, y + 255 - 48);
    sl_gamma->changePosition(x, y + 64);
}
