#include "EffBriCon.h"
#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "../colors.h"
#include "../gl_canvas2d.h"

#include <stdio.h>

EffBriCon::EffBriCon(int x, int y) : Effect(x, y) {
    sl_bri = new Slider(x, y + 64);
    sl_con = new Slider(x, y + 112);
    sl_bri->changeParam(128);
    sl_con->changeParam(128);
}

EffBriCon::~EffBriCon() {
    delete sl_bri;
    delete sl_con;
}

void EffBriCon::apply(Image *image) {
    int w = image->get_w();
    int h = image->get_h();
    rgb_color pixel;
    float factor = 259.0 * (params[1] + 255) / 255.0 / (259 - params[1]);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image->get_pixel(j, i, &(pixel.r), &(pixel.g), &(pixel.b), &(pixel.a));
            pixel.r = truncate((int)pixel.r + params[0], 0, 255);
            pixel.r = truncate(factor * ((float)pixel.r - 128) + 128, 0.f, 255.f);
            pixel.g = truncate((int)pixel.g + params[0], 0, 255);
            pixel.g = truncate(factor * ((float)pixel.g - 128) + 128, 0.f, 255.f);
            pixel.b = truncate((int)pixel.b + params[0], 0, 255);
            pixel.b = truncate(factor * ((float)pixel.b - 128) + 128, 0.f, 255.f);
            image->put_pixel(j, i, pixel.r, pixel.g, pixel.b, pixel.a, false);
        }
    }
}

void EffBriCon::renderMenu() {
    btOK->Render();
    btCancel->Render();
    CV::text(x, y + 16, "Brilho e Contraste");
    CV::text(x, y + 48, "Brilho:");
    CV::text(x, y + 96, "Contraste:");
    sl_bri->Render();
    sl_con->Render();
}

bool EffBriCon::checkMouse(Mouse mouse, Image *image) {
    sl_bri->checkMouse(mouse);
    sl_con->checkMouse(mouse);
    params[0] = (int)sl_bri->getParam() - 128;
    params[1] = (int)sl_con->getParam() - 128;
    if (btOK->checkClick(mouse) == 1) {
        apply(image);
        params[0] = params[1] = 0;
        sl_bri->changeParam(128);
        sl_con->changeParam(128);
        return true;
    }
    else if (btCancel->checkClick(mouse) == 1)
        return true;
    return false;
}

void EffBriCon::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    btOK->changePosition(x + 16, y + 255 - 48);
    btCancel->changePosition(x + 142, y + 255 - 48);
    sl_bri->changePosition(x, y + 64);
    sl_con->changePosition(x, y + 112);
}
