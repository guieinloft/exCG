#include "Tool.h"
#include "Move.h"

#include "../gl_canvas2d.h"
#include "../Layer.h"
#include "../Image.h"
#include "../colors.h"
#include "../Button.h"
#include "../Slider.h"

Move::Move(int sw, int sh) {
}

void Move::renderOptions(int sw, int sh) {
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(0, sh - 80, sw, sh);
}

void Move::checkOptions(int sw, int sh, Mouse mouse) {
}

void Move::execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg) {
    if (mouse.l) {
        layer->set_x(layer->get_x() + mouse.x - mouse.xp);
        layer->set_y(layer->get_y() + mouse.y - mouse.yp);
    }
}

void Move::changePosition(int sw, int sh) {
}
