#include "EffInvert.h"
#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "../colors.h"
#include "../gl_canvas2d.h"

#include <stdio.h>

EffInvert::EffInvert(int x, int y) : Effect(x, y) {
    sl_thres = new Slider(x, y + 64);
    btType[0] = new Button(x, y + 112, 32, 32);
    btType[1] = new Button(x+32, y + 112, 32, 32);
    btType[0]->changeText("<");
    btType[1]->changeText(">");
}

EffInvert::~EffInvert() {
    delete btOK;
    delete btCancel;
    delete sl_thres;
    delete btType[0];
    delete btType[1];
}

void EffInvert::apply(Image *image) {
    int w = image->get_w();
    int h = image->get_h();
    rgb_color pixel;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image->get_pixel(j, i, &(pixel.r), &(pixel.g), &(pixel.b), &(pixel.a));
            if ((params[1] && pixel.r >= params[0]) || (!params[1] && pixel.r <= params[0]))
                pixel.r = 255 - pixel.r;
            if ((params[1] && pixel.g >= params[0]) || (!params[1] && pixel.g <= params[0]))
                pixel.g = 255 - pixel.g;
            if ((params[1] && pixel.b >= params[0]) || (!params[1] && pixel.b <= params[0]))
                pixel.b = 255 - pixel.b;
            image->put_pixel(j, i, pixel.r, pixel.g, pixel.b, pixel.a, false);
        }
    }
}

void EffInvert::renderMenu() {
    btOK->Render();
    btCancel->Render();
    CV::text(x, y + 16, "Inverter Cores / Solarizar");
    CV::text(x, y + 48, "Limiar:");
    CV::text(x, y + 96, "Tipo:");
    sl_thres->Render();
    btType[0]->Render();
    btType[1]->Render();
}

bool EffInvert::checkMouse(Mouse mouse, Image *image) {
    sl_thres->checkMouse(mouse);
    params[0] = (int)sl_thres->getParam();
    int bt_status = btType[0]->checkClick(mouse);
    if (bt_status == 1) params[1] = 0;
    bt_status = btType[1]->checkClick(mouse);
    if (bt_status == 1) params[1] = 1;
    btType[0]->select(!params[1]);
    btType[1]->select(params[1]);
    if (btOK->checkClick(mouse) == 1) {
        apply(image);
        params[0] = params[1] = 0;
        sl_thres->changeParam(0);
        return true;
    }
    else if (btCancel->checkClick(mouse) == 1)
        return true;
    return false;
}

void EffInvert::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    btOK->changePosition(x + 16, y + 255 - 48);
    btCancel->changePosition(x + 142, y + 255 - 48);
    sl_thres->changePosition(x, y + 64);
    btType[0]->changePosition(x, y + 112);
    btType[1]->changePosition(x+32, y + 112);
}
