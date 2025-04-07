#include "EffGrayscale.h"
#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "../colors.h"
#include "../gl_canvas2d.h"

#include <stdio.h>

EffGrayscale::EffGrayscale(int x, int y) : Effect(x, y) {
    sl_r = new Slider(x, y + 64);
    sl_g = new Slider(x, y + 112);
    sl_b = new Slider(x, y + 160);
    sl_r->changeParam(76);
    sl_g->changeParam(150);
    sl_b->changeParam(29);
    params[0] = 76;
    params[1] = 150;
    params[2] = 29;
}

EffGrayscale::~EffGrayscale() {
    delete sl_r;
    delete sl_g;
    delete sl_b;
}

void EffGrayscale::apply(Image *image) {
    int w = image->get_w();
    int h = image->get_h();
    rgb_color pixel;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image->get_pixel(j, i, &(pixel.r), &(pixel.g), &(pixel.b), &(pixel.a));
            int intensity = pixel.r * (params[0]/255.0) + pixel.g * (params[1]/255.0) + pixel.b * (params[2]/255.0);
            pixel.r = truncate(intensity, 0, 255);
            pixel.g = truncate(intensity, 0, 255);
            pixel.b = truncate(intensity, 0, 255);
            image->put_pixel(j, i, pixel.r, pixel.g, pixel.b, pixel.a, false);
        }
    }
}

void EffGrayscale::renderMenu() {
    btOK->Render();
    btCancel->Render();
    CV::text(x, y + 16, "Escala de Cinza:");
    CV::text(x, y + 48, "Vermelho:");
    CV::text(x, y + 96, "Verde:");
    CV::text(x, y + 144, "Azul:");
    sl_r->Render();
    sl_g->Render();
    sl_b->Render();
}

bool EffGrayscale::checkMouse(Mouse mouse, Image *image) {
    sl_r->checkMouse(mouse);
    sl_g->checkMouse(mouse);
    sl_b->checkMouse(mouse);
    params[0] = (int)sl_r->getParam();
    params[1] = (int)sl_g->getParam();
    params[2] = (int)sl_b->getParam();
    if (btOK->checkClick(mouse) == 1) {
        apply(image);
        sl_r->changeParam(76);
        sl_g->changeParam(150);
        sl_b->changeParam(29);
        params[0] = 76;
        params[1] = 150;
        params[2] = 29;
        return true;
    }
    else if (btCancel->checkClick(mouse) == 1)
        return true;
    return false;
}

void EffGrayscale::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    btOK->changePosition(x + 16, y + 255 - 48);
    btCancel->changePosition(x + 142, y + 255 - 48);
    sl_r->changePosition(x, y + 64);
    sl_g->changePosition(x, y + 112);
    sl_b->changePosition(x, y + 160);
}
