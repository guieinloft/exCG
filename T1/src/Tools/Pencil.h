#ifndef TOOL_PENCIL_H
#define TOOL_PENCIL_H

#include "Tool.h"
#include "../colors.h"

class Pencil : public Tool {
    public:
    Pencil() : Tool() {}
    void renderOptions(int sw, int sh);
    void checkOptions(int sw, int sh, Mouse mouse);
    void execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color fg, rgb_color bg);
};

#endif
