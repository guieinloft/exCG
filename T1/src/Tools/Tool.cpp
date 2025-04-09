#include "Tool.h"

#include "../gl_canvas2d.h"
#include "../Mouse.h"
#include "../Canvas.h"
#include "../colors.h"

Tool::Tool() {
    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
    params[3] = 0;
}

Tool::~Tool() {
}

void Tool::renderOptions(int sw, int sh) {
    CV::color(0.25f, 0.25f, 0.25f);
    CV::rectFill(0, sh - 80, sw, sh);
}

bool Tool::checkOptions(int sh, Mouse mouse) {
    return mouse.y > sh - 80;
}

void Tool::changePosition(int sh) {
    (void)sh;
}

void Tool::execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg) {
    (void)mouse;
    (void)canvas;
    (void)layer;
    (void)fg;
    (void)bg;
    printf("\nNAO IMPLEMENTADO");
}

void Tool::renderBorder(Canvas *canvas, Layer *layer) {
    int real_x = canvas->get_x() + layer->get_x();
    int real_y = canvas->get_y() + layer->get_y();
    int w = layer->getImage()->get_w();
    int h = layer->getImage()->get_h();
    CV::color(0.0f, 0.0f, 0.0f);
    CV::rect(real_x, real_y - 1, real_x + w + 1, real_y + h);
    CV::color(1.0f, 1.0f, 1.0f);
    CV::rect(real_x - 1, real_y - 2, real_x + w + 2, real_y + h + 1);
}
