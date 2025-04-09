#include "Tool.h"
#include "Picker.h"

#include "../gl_canvas2d.h"
#include "../Layer.h"
#include "../Image.h"
#include "../colors.h"
#include "../Button.h"
#include "../Slider.h"

Picker::Picker(int sw, int sh) {
}

void Picker::renderOptions(int sw, int sh) {
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(0, sh - 80, sw, sh);
}

bool Picker::checkOptions(int sw, int sh, Mouse mouse) {
    return mouse.y > sh - 80;
}

void Picker::execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg) {
    int canvas_x = canvas->get_x();
    int canvas_y = canvas->get_y();
    int canvas_w = canvas->get_w();
    int canvas_h = canvas->get_h();
    if (mouse.x < canvas_x || mouse.x > canvas_x + canvas_w) return;
    if (mouse.y < canvas_y || mouse.y > canvas_y + canvas_h) return;
    int real_x = canvas_x + layer->get_x();
    int real_y = canvas_y + layer->get_y();
    if (mouse.l) {
        layer->getImage()->get_pixel(mouse.x - real_x, mouse.y - real_y,
            &fg->r, &fg->g, &fg->b, &fg->a);
    }
    else if (mouse.r) {
        layer->getImage()->get_pixel(mouse.x - real_x, mouse.y - real_y,
            &bg->r, &bg->g, &bg->b, &bg->a);
    }
}

void Picker::changePosition(int sw, int sh) {
}
