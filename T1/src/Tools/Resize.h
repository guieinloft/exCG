#ifndef TOOL_RESIZE_H
#define TOOL_RESIZE_H

#include "Tool.h"
#include "../colors.h"
#include "../Slider.h"
#include "../Button.h"

enum ResizeOptions {
    RESIZE_NO,
    RESIZE_PX = 0x01,
    RESIZE_PY = 0x02,
    RESIZE_MX = 0x04,
    RESIZE_MY = 0x08
};

class Resize : public Tool {
    public:
    Resize(int sw, int sh);
    void renderOptions(int sw, int sh);
    bool checkOptions(int sw, int sh, Mouse mouse);
    void execute(Mouse mouse, Canvas *canvas, Layer *layer, rgb_color *fg, rgb_color *bg);
    void changePosition(int sw, int sh);
    void renderBorder(Canvas *canvas, Layer *layer);
};

#endif
