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

void Tool::renderOptions(int sw, int sh) {
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(0, sh - 80, sw, sh);
}

void Tool::checkOptions(int sw, int sh, Mouse mouse) {
}

void Tool::changePosition(int sw, int sh) {
}

void Tool::execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg) {
}

