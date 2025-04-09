#include "Tool.h"
#include "Rotate.h"

#include "../gl_canvas2d.h"
#include "../Layer.h"
#include "../Image.h"
#include "../colors.h"
#include "../Button.h"
#include "../Slider.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159
#endif

Rotate::Rotate() {
}

void Rotate::renderOptions(int sw, int sh) {
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(0, sh - 80, sw, sh);
}

bool Rotate::checkOptions(int sh, Mouse mouse) {
    return mouse.y > sh - 80;
}

void Rotate::execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg) {
    int real_x = canvas->get_x() + layer->get_x();
    int real_y = canvas->get_y() + layer->get_y();
    int w = layer->getImage()->get_w();
    int h = layer->getImage()->get_h();
    int c_x = real_x + w / 2;
    int c_y = real_y + h / 2;
    if (mouse.button == 0 && mouse.state == 0) {
        params[0] = 1;
        params[1] = atan2(mouse.y - c_y, mouse.x - c_x) * 180 / M_PI;
    }
    if (mouse.l) {
        params[2] = atan2(mouse.y - c_y, mouse.x - c_x) * 180 / M_PI;
    }
    if (mouse.button == 0 && mouse.state == 1) {
        params[0] = 0;
        int offx, offy;
        layer->getImage()->rotate((params[2] - params[1]) * M_PI/180, &offx, &offy);
        layer->set_x(layer->get_x() + offx);
        layer->set_y(layer->get_y() + offy);
        canvas->update();
    }
    (void)fg;
    (void)bg;
}

void Rotate::changePosition(int sh) {
    (void)sh;
}

void Rotate::renderBorder(Canvas *canvas, Layer *layer) {
    int real_x = canvas->get_x() + layer->get_x();
    int real_y = canvas->get_y() + layer->get_y();
    int w = layer->getImage()->get_w();
    int h = layer->getImage()->get_h();
    int c_x = real_x + w / 2;
    int c_y = real_y + h / 2;
    CV::color(0.0f, 0.0f, 0.0f);
    CV::rect(real_x, real_y - 1, real_x + w + 1, real_y + h);
    CV::color(1.0f, 1.0f, 1.0f);
    CV::rect(real_x - 1, real_y - 2, real_x + w + 2, real_y + h + 1);
    if (params[0]) {
        CV::color(0.0f, 0.0f, 0.0f);
        CV::circle(c_x, c_y, 31, 32);
        CV::color(1.0f, 1.0f, 1.0f);
        CV::circle(c_x, c_y, 32, 32);
        CV::line(c_x, c_y, c_x + 32 * cos(params[1]*M_PI/180), c_y + 32 * sin(params[1]*M_PI/180));
        CV::line(c_x, c_y, c_x + 32 * cos(params[2]*M_PI/180), c_y + 32 * sin(params[2]*M_PI/180));
    }
}
