#include "Tool.h"
#include "Move.h"

#include "../gl_canvas2d.h"
#include "../Layer.h"
#include "../Image.h"
#include "../colors.h"
#include "../Button.h"
#include "../Slider.h"

Move::Move() {
}

void Move::renderOptions(int sw, int sh) {
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(0, sh - 80, sw, sh);
}

bool Move::checkOptions(int sh, Mouse mouse) {
    return mouse.y > sh - 80;
}

void Move::execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg) {
    int real_x = canvas->get_x() + layer->get_x();
    int real_y = canvas->get_y() + layer->get_y();
    int w = layer->getImage()->get_w();
    int h = layer->getImage()->get_h();
    if (mouse.x >= real_x && mouse.x < real_x + w &&
        mouse.y >= real_y && mouse.y < real_y + h) {
        if (mouse.l) {
            layer->set_x(layer->get_x() + mouse.x - mouse.xp);
            layer->set_y(layer->get_y() + mouse.y - mouse.yp);
            canvas->update();
        }
    }
    (void)fg;
    (void)bg;
}

void Move::changePosition(int sh) {
    (void)sh;
}
