#ifndef TOOL_ROTATE_H
#define TOOL_ROTATE_H

#include "Tool.h"
#include "../colors.h"
#include "../Slider.h"
#include "../Button.h"

class Rotate : public Tool {
    public:
    Rotate(int sw, int sh);
    void renderOptions(int sw, int sh);
    void checkOptions(int sw, int sh, Mouse mouse);
    void execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg);
    void changePosition(int sw, int sh);
    void renderBorder(Canvas *canvas, Layer *layer);
};

#endif
