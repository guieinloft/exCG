#include "Tool.h"

#include "../gl_canvas2d.h"
#include "../Mouse.h"
#include "../Canvas.h"

Tool::Tool() {
    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
}

void Tool::renderOptions(int sw, int sh) {
    CV::color(0.25, 0.25, 0.25);
    CV::rectFill(0, sh - 64, sw, sh);
}

void Tool::checkOptions(int sw, int sh, Mouse mouse) {
}

void Tool::execute(Mouse mouse, Canvas *canvas) {
    printf("\nEXECUTE");
}

