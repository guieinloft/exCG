#include "EffHSLAdjust.h"
#include "../Image.h"
#include "../Button.h"
#include "../Slider.h"
#include "../colors.h"
#include "../gl_canvas2d.h"

#include <stdio.h>

EffHSLAdjust::EffHSLAdjust(int x, int y) : Effect(x, y) {
    sl_h = new Slider(x, y + 64);
    sl_s = new Slider(x, y + 112);
    sl_l = new Slider(x, y + 160);
    sl_h->changeParam(128);
    sl_s->changeParam(128);
    sl_l->changeParam(128);
    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
}

EffHSLAdjust::~EffHSLAdjust() {
    delete btOK;
    delete btCancel;
    delete sl_h;
    delete sl_s;
    delete sl_l;
}

void EffHSLAdjust::apply(Image *image) {
    /*
    int w = image->get_w();
    int h = image->get_h();
    rgb_color pixel;
    hsl_color pixel_hsl;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image->get_pixel(j, i, &(pixel.r), &(pixel.g), &(pixel.b), &(pixel.a));
            pixel_hsl = rgb_to_hsl(pixel);
            pixel_hsl.h = fmod(pixel_hsl.h + 360 + params[0], 360.0);
            pixel_hsl.s = truncate(pixel_hsl.s + params[1]/127.0, 0, 1);
            pixel_hsl.l = truncate(pixel_hsl.l + params[2]/127.0, 0, 1);
            pixel = hsl_to_rgb(pixel_hsl);
            image->put_pixel(j, i, pixel.r, pixel.g, pixel.b, pixel.a, false);
        }
    }
    */
    //fiz isso pq no codeblocks não funcionava (vai entender bicho...)
    (void)image;
}

void EffHSLAdjust::renderMenu() {
    btOK->Render();
    btCancel->Render();
    CV::text(x, y + 16, "Ajuste HSL:");
    CV::text(x, y + 48, "Matiz:");
    CV::text(x, y + 96, "Saturacao:");
    CV::text(x, y + 144, "Luminosidade:");
    sl_h->Render();
    sl_s->Render();
    sl_l->Render();
}

bool EffHSLAdjust::checkMouse(Mouse mouse, Image *image) {
    sl_h->checkMouse(mouse);
    sl_s->checkMouse(mouse);
    sl_l->checkMouse(mouse);
    params[0] = (int)sl_h->getParam() * 360.0 / 255.0 - 180;
    params[1] = (int)sl_s->getParam() - 128;
    params[2] = (int)sl_l->getParam() - 128;
    if (btOK->checkClick(mouse) == 1) {
        apply(image);
        sl_h->changeParam(128);
        sl_s->changeParam(128);
        sl_l->changeParam(128);
        params[0] = 0;
        params[1] = 0;
        params[2] = 0;
        return true;
    }
    else if (btCancel->checkClick(mouse) == 1)
        return true;
    return false;
}

void EffHSLAdjust::changePosition(int x, int y) {
    this->x = x;
    this->y = y;
    btOK->changePosition(x + 16, y + 255 - 48);
    btCancel->changePosition(x + 142, y + 255 - 48);
    sl_h->changePosition(x, y + 64);
    sl_s->changePosition(x, y + 112);
    sl_l->changePosition(x, y + 160);
}
